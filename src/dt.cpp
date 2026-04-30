#include "dt.hpp"

namespace cfg::dt {

DominatorTree::DominatorTree(
    const dominator::Dominators& doms, 
    const digraph::Digraph& cfg) : dt_(cfg.V()), idom_(cfg.V(), -1)   
{
    for (int i = 0; i < dt_.V(); ++i) {
        auto domset = doms.domset(i);
        domset.erase(i);
        if (domset.empty()) {
            continue;
        }

        int j = domset.size() == 1 ? 
                    *domset.begin() : 
                    cfg.closest(domset, i);
        dt_.createEdge(j, i);
        idom_[i] = j;
    }
}

const digraph::Digraph& DominatorTree::dt() const noexcept {
    return dt_;    
}

const std::vector<int>& DominatorTree::idom() const noexcept {
    return idom_;
}

}