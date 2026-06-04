#include "dpo.hpp"

#include <ranges>

namespace cfg::dpo {

DPO::DPO(const digraph::Digraph& cfg) : 
    idom_(cfg.V(), -1), 
    dt_(cfg.V()) 
{

    auto po = cfg.po();
    std::vector<int> poCount(cfg.V());

    int count = 0;
    for (auto&& b : po) {
        poCount[b] = count++;
    }

    idom_[0] = 0;

    bool changed = true;
    while (changed) {
        changed = false;

        for (auto&& bb : po | std::views::reverse) {
            if (0 == bb) {
                continue;
            }

            int Q;
            for (auto&& p : cfg.preds(bb)) {
                if (idom_[p] != -1) { 
                    Q = p; 
                    break;
                }
            } 

            for (auto&& p : cfg.preds(bb)) {
                int P = p;
                if (-1 == idom_[P]) continue;
                while (Q != P) {
                    if (poCount[Q] < poCount[P]) {
                        Q = idom_[Q];
                    } else {
                        P = idom_[P];
                    }
                }
            }
        
            if (idom_[bb] != Q) {
                idom_[bb] = Q; 
                changed = true;
            }
        }
    }

    idom_[0] = -1;
    fillDT_();
}

const digraph::Digraph& DPO::dt() const noexcept {
    return dt_;
}

const std::vector<int>& DPO::idom() const noexcept {
    return idom_;
}

void DPO::fillDT_() {
    for (int i = 0; i < idom_.size(); ++i) {
        if (-1 == idom_[i]) continue;
        dt_.createEdge(idom_[i], i);
    }
}

} // namespace cfg::dpo