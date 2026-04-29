#pragma once

#include <vector>
#include <ostream>
#include <set>

namespace cfg::digraph {

class DIGraph {
public:
    DIGraph(int V);

public: 
    void createEdge(int u, int v); 
    const std::vector<int>& preds(int v) const;
    const std::vector<int>& adjs(int v) const;
    void printDot(std::ostream& out) const;
    std::size_t V() const noexcept;
    std::size_t E() const noexcept;
    int closest(const std::set<int>& s, int v) const;

private:
    std::vector<std::vector<int>> V_;
    std::vector<std::vector<int>> preds_;
    std::size_t E_ = 0;
};

} // namespace cfg::digraph