#include "df.hpp"

namespace cfg::df {

DominanceFrontier::DominanceFrontier(
    const std::vector<int>& idom, 
    const digraph::DIGraph& cfg) : dft_(cfg.V()), df_(cfg.V())
{
    for (int i = 0; i < cfg.V(); ++i) {
        for (int p : cfg.preds(i)) {
            int r = p;
            while (r != idom[i]) {
                dft_.createEdge(r, i);
                df_[r].insert(i);
                r = idom[r];
            }
        }
    }
}

const digraph::DIGraph& DominanceFrontier::dft() const noexcept {
    return dft_;
}

const std::vector<std::set<int>>& DominanceFrontier::df() const noexcept {
    return df_;
}

} // namespace cfg::df
