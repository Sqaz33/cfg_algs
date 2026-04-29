#pragma once

#include "digraph.hpp"
#include "dt.hpp"

namespace cfg::df {

class DominanceFrontier {
public:
    DominanceFrontier(
        const std::vector<int>& idom, 
        const digraph::DIGraph& cfg);

public:
    const digraph::DIGraph& dft() const noexcept;
    const std::vector<std::set<int>>& df() const noexcept;

private:
    digraph::DIGraph dft_;
    std::vector<std::set<int>> df_;
};

} // namespace cfg::df