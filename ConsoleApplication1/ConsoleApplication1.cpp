#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <stack>

struct File {
    std::string name;
    std::string content;
    std::set<std::string> dependencies;
};

std::map<std::string, File> files;

bool topologicalSort(std::map<std::string, std::set<std::string>>& adjacency_list, std::vector<std::string>& order) {
    std::map<std::string, int> in_degree;
    for (auto& entry : adjacency_list) {
        in_degree[entry.first] = 0;
    }
    for (auto& entry : adjacency_list) {
        for (auto& dep : entry.second) {
            in_degree[dep]++;
        }
    }
    std::stack<std::string> no_incoming_edges;
    for (auto& entry : in_degree) {
        if (entry.second == 0) {
            no_incoming_edges.push(entry.first);
        }
    }
    while (!no_incoming_edges.empty()) {
        std::string v = no_incoming_edges.top();
        no_incoming_edges.pop();
        order.push_back(v);
        for (auto& neighbor : adjacency_list[v]) {
            in_degree[neighbor]--;
            if (in_degree[neighbor] == 0) {
                no_incoming_edges.push(neighbor);
            }
        }
    }
    return order.size() == in_degree.size();
}

int main() {
    files["Folder 1/File 1-1"] = { "Folder 1/File 1-1", "Lorem ipsum dolor sit amet...", {"Folder 2/File 2-1"} };
    files["Folder 2/File 2-1"] = { "Folder 2/File 2-1", "Phasellus eget tellus...", {} };
    files["Folder 2/File 2-2"] = { "Folder 2/File 2-2", "In pretium dictum lacinia...", {"Folder 1/File 1-1", "Folder 2/File 2-1"} };

    std::map<std::string, std::set<std::string>> dependencies;
    for (auto& entry : files) {
        for (auto& dep : entry.second.dependencies) {
            dependencies[entry.first].insert(dep);
        }
    }

    std::vector<std::string> order;
    if (!topologicalSort(dependencies, order)) {
        std::cout << "Cycle detected! Unable to create dependency order." << std::endl;
    }
    else {
        std::string concatenated_content;
        for (auto& file_path : order) {
            concatenated_content += files[file_path].content;
        }

        std::ofstream output_file("concatenated_files.txt");
        if (output_file.is_open()) {
            output_file << concatenated_content;
            output_file.close();
            std::cout << "Concatenated content of all files has been written to 'concatenated_files.txt'." << std::endl;
        }
        else {
            std::cout << "Failed to open file for writing." << std::endl;
        }
    }

    return 0;
}
