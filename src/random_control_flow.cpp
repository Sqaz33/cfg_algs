#include "random_control_flow.hpp"

#include <algorithm>
#include <random>
#include <vector>

#include "scc.hpp"

namespace cfg::random_cfg {

RandomCFG::RandomCFG(int vertex) : graph_(vertex) {
   gen_(vertex);
   linkComponentsWithEntry_();
}

const digraph::Digraph& RandomCFG::cfg() const noexcept {
    return graph_;
}

void RandomCFG::gen_(int vertex) {
    // vertex and degree in pair
    std::vector<std::pair<int, int>> degrees(vertex);
    int degree = 2;
    int saturation = 2;

    int idx = 0;
    std::transform(
        degrees.begin(), degrees.end(), degrees.begin(), 
        [&idx, degree](auto p) { return std::pair<int, int>(idx++, degree); });

    std::random_device rd;
    std::mt19937 gen(rd());
    
    for (int i = 0; i < vertex * saturation && degrees.size() > 1; ++i) {
        std::uniform_int_distribution<> dist(0, degrees.size() - 1);

        int uIdx = dist(gen);
        int vIdx = dist(gen);

        while ((uIdx == vIdx && degrees[uIdx].second == 1) || degrees[vIdx].first == 0) {
            vIdx = dist(gen);
            uIdx = dist(gen);
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
                degrees.erase(degrees.begin() + vIdx);
            }
        }
        
    }
}

void RandomCFG::linkComponentsWithEntry_() {
    digraph::scc::SCC scc(graph_);
    auto&& id = scc.id();
    int pred = 0;

    for (int i = 0; i < id.size(); pred = i, ++i) {
        if (id[pred] != id[i]) {
            graph_.createEdge(pred, i);
        }
    }
}

}