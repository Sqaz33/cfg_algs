#pragma once

#include "digraph.hpp"
#include "dt.hpp"

namespace cfg::df {

class DominanceFrontier {
public:
    DominanceFrontier(
        const std::vector<int>& idom, 
        const digraph::Digraph& cfg);

public:
    const digraph::Digraph& dft() const noexcept;
    const std::vector<std::set<int>>& df() const noexcept;

private:
    digraph::Digraph dft_;
    std::vector<std::set<int>> df_;
};

} // namespace cfg::df