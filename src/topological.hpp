#pragma once

#include <stack>

#include "digraph.hpp"

namespace cfg::digraph::topological {

class Topological {
public:
    Topological(const Digraph& g);

public:
    const std::vector<int>& order() const noexcept;

private:
    void dfs_(const Digraph& g, int v);

private:
    std::vector<bool> marked_; 
    std::vector<int> order_;
};

} // namespace cfg::digraph::topological 
