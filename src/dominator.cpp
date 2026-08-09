#include "dominator.hpp"

#include <ranges>
#include <queue>

static 
std::set<int> intersect(const std::set<int>& a, const std::set<int>& b) {
    std::set<int> res;
    auto&& minSz = a.size() > b.size() ? b : a;
    auto&& maxSz = a.size() > b.size() ? a : b;

    for (int x : minSz) {
        if (maxSz.contains(x)) {
            res.insert(x);
        }
    }

    return res;
}

namespace cfg::dominator {

Dominators::Dominators(const digraph::Digraph& cfg) : domsets_(cfg.V()) {
    std::vector<bool> marked(cfg.V(), false);
    std::queue<int> queue;
    queue.push(0);
    while (!queue.empty()) {
        int v = queue.front();
        queue.pop();
        marked[v] = true;
        for (int u : cfg.adjs(v)) {
            if (!marked[u]) {
                queue.push(u);
            }
        }
    }

    auto r = std::views::iota(0, int(cfg.V()) + 1);
    const std::set<int> all(r.begin(), r.end());

    int v = 0;
    for (auto&& doms : domsets_) {
        if (marked[v++]) {
            doms = all;
        }
    }
    domsets_[0] = {0};

    bool changed = true;
    while (changed) {
        changed = false;
        for (int i = 0; i < cfg.V(); ++i) {
            if (!marked[i]) {
                continue;
            }
            std::set<int> temp;

            auto&& preds = cfg.preds(i);
            if (!preds.empty()) {
                temp = domsets_[*preds.begin()];
            } 

            for (int j : preds) {
                if (marked[j]) {
                    temp = intersect(temp, domsets_[j]);
                }
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