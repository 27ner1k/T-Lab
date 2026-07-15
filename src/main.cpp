#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>

int main(){

    std::ifstream f("tests/data_2_dangling.csv");
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
    std::cout << lenght << "\n";
    f.close();

    std::unordered_map<int32_t, int32_t> to_new_id;
    std::vector<int32_t> to_back_id;
    int now = 0;
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
    std::cout << now << std::endl;
    return 0;
}