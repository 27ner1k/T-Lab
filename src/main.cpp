#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>

int main(int argc, char* argv[]) {

    std::ifstream f(argv[1]);
    std::string line;
    std::getline(f, line);
    std::vector<std::pair<int32_t, int32_t>> edges(0);
    size_t lenght = 0;
    while (std::getline(f, line)) { 
        std::stringstream ss(line);
        std::string from, to;
        std::getline(ss, from, ',');
        std::getline(ss, to, ',');
        edges.push_back(std::pair<int32_t, int32_t>{std::stoi(from), std::stoi(to)});
        lenght += 1;
    }
    f.close();

    std::unordered_map<int32_t, int32_t> to_new_id;
    std::vector<int32_t> to_back_id;
    int32_t now = 0;
    for (auto[from, to] : edges){
        if (to_new_id.find(from) == to_new_id.end()){
            to_new_id[from] = now;
            to_back_id.push_back(from);
            now += 1;
        }
        if (to_new_id.find(to) == to_new_id.end()){
            to_new_id[to] = now;
            to_back_id.push_back(to);
            now += 1;
        }
    }
    
    int32_t to_back_id_size = to_back_id.size();
    std::vector<int32_t> degree_vertex(to_back_id_size, 0);
    for (auto[from, to] : edges){
        degree_vertex[to_new_id[from]] += 1;
    }
    std::vector<double> vertex_rank(to_back_id_size, double (1 / double(to_back_id_size)));
    std::vector<double> new_vertex_rank(to_back_id_size, double (1 / double(to_back_id_size)));
    constexpr double damping_vertex = 0.85;

    for (int iter = 0; iter < 100; ++iter) {
        double dangling_sum = 0;
        for (int i = 0; i < to_back_id_size; ++i){
            if (degree_vertex[i] == 0){
                dangling_sum += vertex_rank[i];
            }
        }

        double base = (1.0 - damping_vertex) / to_back_id_size + damping_vertex * dangling_sum / to_back_id_size;
        for (int i = 0; i < to_back_id_size; ++i)
            new_vertex_rank[i] = base;

        for (auto [from, to] : edges) {
            int new_from = to_new_id[from];
            int new_to = to_new_id[to];
            new_vertex_rank[new_to] += damping_vertex * vertex_rank[new_from] / degree_vertex[new_from];
        }
        vertex_rank = new_vertex_rank;
    }

   std::ofstream out(argv[2]);
   out << "vertex,rank\n";
   for (int i = 0; i < to_back_id_size; ++i) {
        out << to_back_id[i] << "," << vertex_rank[i] << "\n";
   }


    return 0;
}