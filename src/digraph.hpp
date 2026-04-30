#pragma once

#include <vector>
#include <ostream>
#include <set>

namespace cfg::digraph {

class Digraph {
public:
    Digraph(int V);

public: 
    void createEdge(int u, int v); 
    const std::set<int>& preds(int v) const;
    const std::set<int>& adjs(int v) const;
    void printDot(std::ostream& out) const;
    std::size_t V() const noexcept;
    std::size_t E() const noexcept;
    int closest(const std::set<int>& s, int v) const;
    Digraph reverse() const;

private:
    std::vector<std::set<int>> V_;
    std::vector<std::set<int>> preds_;
    std::size_t E_ = 0;
};

} // namespace cfg::digraph