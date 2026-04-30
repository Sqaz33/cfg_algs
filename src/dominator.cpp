#include "dominator.hpp"

#include <ranges>

static 
std::set<int> intersect(const std::set<int>& a, const std::set<int>& b) {
    std::set<int> res;

    const std::set<int>* minSz;
    const std::set<int>* maxSz;
    if (a.size() > b.size()) {
        minSz = &b;
        maxSz = &a;
    } else {
        minSz = &a;
        maxSz = &b;
    }

    for (int x : *minSz) {
        if (maxSz->contains(x)) {
            res.insert(x);
        }
    }

    return res;
}

namespace cfg::dominator {

Dominators::Dominators(const digraph::Digraph& cfg) : domsets_(cfg.V()) {
    auto r = std::views::iota(0, int(cfg.V()) + 1);
    std::set<int> all(r.begin(), r.end());

    for (auto&& doms : domsets_) {
        doms = all;
    }
    domsets_[0] = {0};

    bool changed = true;
    while (changed) {
        changed = false;
        for (int i = 0; i < cfg.V(); ++i) {

            std::set<int> temp;

            auto&& preds = cfg.preds(i);
            if (!preds.empty()) {
                temp = domsets_[*preds.begin()];
            }

            for (int j : preds) {
                temp = intersect(temp, domsets_[j]);
            }

            temp.insert(i);
            if (domsets_[i] != temp) {
                changed = true;
                domsets_[i] = std::move(temp);
            }
        }
    }
}

const std::set<int>& Dominators::domset(int v) const {
    return domsets_[v];
}

} // namespace cfg::dominator