#include "dt.hpp"

namespace cfg::dt {

DominatorTree::DominatorTree(
    const dominator::Dominators& doms, 
    const digraph::DIGraph& cfg) : dt_(cfg.V()), idom_(cfg.V(), -1)   
{
    for (int i = 0; i < dt_.V(); ++i) {
        auto domset = doms.domset(i);
        domset.erase(i);
        if (domset.empty()) {
            continue;
        }

        if (domset.size() == 1) {
            dt_.createEdge(*domset.begin(), i);
            idom_[i] = *domset.begin();
        } else {
            int j = cfg.closest(domset, i);
            dt_.createEdge(j, i);
            idom_[i] = j;
        }
    }
}

const digraph::DIGraph& DominatorTree::dt() const noexcept {
    return dt_;    
}

const std::vector<int>& DominatorTree::idom() const noexcept {
    return idom_;
}

}