#pragma once

#include <cstddef>
#include <vector>

#include "digraph.hpp"

namespace cfg::digraph::scc {

// алгос Косараю    
// O(n)
class SCC {
public:
    SCC(const Digraph& g);

public:
    const std::vector<std::size_t>& id() const noexcept;

private:
    void dfs_(const Digraph& g, int v);

private:
    std::vector<bool> marked_;
    std::vector<std::size_t> id_;
    std::size_t count_ = 0;
};

} // namespace cfg::digraph::scc

