#pragma once

#include <vector>
#include <ostream>

namespace bb_graphs::digraph {

class DIGraph {
public:
    DIGraph(int V);

public: 
    void createEdge(int u, int v); 
    const std::vector<int>& preds(int v) const;
    const std::vector<int>& adjs(int v) const;
    void printDot(std::ostream& out) const;

private:
    std::vector<std::vector<int>> V_;
    std::vector<std::vector<int>> preds_;
};

} // namespace bb_graphs::digraph