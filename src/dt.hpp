#pragma once

#include "digraph.hpp"
#include "dominator.hpp"

namespace cfg::dt {

class DominatorTree {
public:
    DominatorTree(
        const dominator::Dominators& doms, 
        const digraph::Digraph& cfg);

public:
    const digraph::Digraph& dt() const noexcept;
    const std::vector<int>& idom() const noexcept;

private:
    digraph::Digraph dt_;
    std::vector<int> idom_;
};

} // namespace cfg::random_cfg 