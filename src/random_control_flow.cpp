#include "random_control_flow.hpp"

#include <algorithm>
#include <random>
#include <vector>

namespace cfg::random_cfg {

RandomCFG::RandomCFG(int vertex) : graph_(vertex) {
    // vertex and degree in pair
    std::vector<std::pair<int, int>> degrees(vertex);

    int idx = 0;
    std::transform(
        degrees.begin(), degrees.end(), degrees.begin(), 
        [&idx](auto p) { return std::pair<int, int>(idx++, 2); });

    std::random_device rd;
    std::mt19937 gen(rd());
    
    for (int i = 0; i < vertex * 2 && degrees.size(); ++i) {
        std::uniform_int_distribution<> dist(0, degrees.size() - 1);

        int uIdx = dist(gen);
        int vIdx = dist(gen);
 
        if (uIdx == vIdx && degrees[uIdx].second == 1) {
            while (uIdx == vIdx) {
                vIdx = dist(gen);
            }
        } 

        int u = degrees[uIdx].first;
        int v = degrees[vIdx].first;

        graph_.createEdge(u, v);

        --degrees[uIdx].second;
        --degrees[vIdx].second;
        bool eraseU = false;
        if (degrees[uIdx].second == 0) {
            eraseU = true;
            degrees.erase(degrees.begin() + uIdx);
        }

        if (uIdx != vIdx) {
            if (vIdx > uIdx && eraseU) {
                --vIdx;
            }
            if (degrees[vIdx].second == 0) {
                degrees.erase(degrees.begin() + uIdx);
            }
        }
        
    }
}

const digraph::DIGraph& RandomCFG::cfg() const noexcept {
    return graph_;
}

}