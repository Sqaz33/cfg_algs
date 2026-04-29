#include "dj.hpp"

namespace cfg::dj {

DefJoin::DefJoin(
    const digraph::DIGraph& cfg, 
    const digraph::DIGraph& dt) : dj_(dt)
{
    for (int i = 0; i < cfg.V(); ++i) {
        auto&& dtAdjs = dt.adjs(i);
        for (auto j : cfg.adjs(i)) {
            if (!dtAdjs.contains(j)) {
                dj_.createEdge(i, j);
            }
        } 
    }
}

const digraph::DIGraph& DefJoin::dj() const noexcept {
    return dj_;
}

} // namespace cfg::dj
