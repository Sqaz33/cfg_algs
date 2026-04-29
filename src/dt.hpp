#pragma once

#include "digraph.hpp"
#include "dominator.hpp"

namespace cfg::dt {

class DominatorTree {
public:
    DominatorTree(
        const dominator::Dominators& doms, 
        const digraph::DIGraph& cfg);

public:
    const digraph::DIGraph& dt() const noexcept;
    const std::vector<int>& idom() const noexcept;

private:
    digraph::DIGraph dt_;
    std::vector<int> idom_;
};

} // namespace cfg::random_cfg 