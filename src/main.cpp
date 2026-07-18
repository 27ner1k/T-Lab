#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <cstdio>
#include <iomanip> 
#include <algorithm>

int main(int argc, char* argv[]) {

    std::ifstream f(argv[1]);
    std::ofstream bin("edges.bin", std::ios::binary);
    std::string line;
    std::getline(f, line);

    std::unordered_map<int32_t, int32_t> to_new_id;
    std::vector<int32_t> to_back_id;
    std::vector<int32_t> degree_vertex;
    int32_t now = 0;
    
    while (std::getline(f, line)) { 
        std::stringstream ss(line);
        std::string s_from, s_to;
        std::getline(ss, s_from, ',');
        std::getline(ss, s_to, ',');
        int32_t from = std::stoi(s_from);
        int32_t to = std::stoi(s_to);
        if (to_new_id.find(from) == to_new_id.end()){
            to_new_id[from] = now;
            to_back_id.push_back(from);
            now += 1;
            degree_vertex.push_back(0);
        }
        if (to_new_id.find(to) == to_new_id.end()){
            to_new_id[to] = now;
            to_back_id.push_back(to);
            now += 1;
            degree_vertex.push_back(0);
        }
        degree_vertex[to_new_id[from]] += 1;
        int32_t nf = to_new_id[from];
        int32_t nt = to_new_id[to];
        bin.write(reinterpret_cast<const char*>(&nf), sizeof(nf));
        bin.write(reinterpret_cast<const char*>(&nt), sizeof(nt));
    }
    f.close();
    bin.close();
    

    int32_t to_back_id_size = to_back_id.size();
    std::vector<double> vertex_rank(to_back_id_size, double (1 / double(to_back_id_size)));
    std::vector<double> new_vertex_rank(to_back_id_size, double (1 / double(to_back_id_size)));
    constexpr double damping_vertex = 0.85;
    constexpr int32_t edges_per_block = 65536;
    std::vector<int32_t> buf(edges_per_block * 2);
    

    for (int iter = 0; iter < 200; ++iter) {
        double dangling_sum = 0;
        #pragma omp parallel for reduction(+:dangling_sum)
        for (int i = 0; i < to_back_id_size; ++i){
            if (degree_vertex[i] == 0){
                dangling_sum += vertex_rank[i];
            }
        }

        double base = (1.0 - damping_vertex) / to_back_id_size + damping_vertex * dangling_sum / to_back_id_size;
        #pragma omp parallel for
        for (int i = 0; i < to_back_id_size; ++i)
            new_vertex_rank[i] = base;

        std::ifstream bin("edges.bin", std::ios::binary);
        while (bin) {
            bin.read(reinterpret_cast<char*>(buf.data()), edges_per_block * 2 * sizeof(int32_t));
            long long got = bin.gcount() / (2 * sizeof(int32_t));
            #pragma omp parallel for
            for (long long i = 0; i < got; ++i) {
                int32_t new_from = buf[2 * i];
                int32_t new_to = buf[2 * i + 1];
                double contrib = damping_vertex * vertex_rank[new_from] / degree_vertex[new_from];
                #pragma omp atomic
                new_vertex_rank[new_to] += contrib;
            }
        }
        bin.close();
        vertex_rank = new_vertex_rank;
        if (iter == 99) std::cerr << "dangling_sum: " << dangling_sum << "\n";
    }

    std::ofstream out(argv[2]);
    out << std::fixed << std::setprecision(10);
    out << "vertex,rank" << std::endl;

    std::vector<std::pair<int32_t, double>> result(to_back_id_size);

    for (int i = 0; i < to_back_id_size; ++i) {
        result[i] = {to_back_id[i], vertex_rank[i]};
    }
    std::sort(result.begin(), result.end());
    for (const auto& [vertex, rank] : result) {
        out << vertex << "," << rank << std::endl;
    }
    std::remove("edges.bin");

    return 0;
}