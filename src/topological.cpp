#include "topological.hpp"

#include <stack>

namespace cfg::digraph::topological {

void Topological::dfs_(const Digraph& g, int v) {
    std::stack<int> stack;
    stack.push(v);
    marked_[v] = true;
    while (!stack.empty()) {
        auto v = stack.top();
        for (auto w : g.adjs(v)) {
            if (!marked_[w]) {
                marked_[w] = true;
                stack.push(w);
            }
        }     
        if (stack.top() == v) {
            stack.pop();
            order_.push_back(v);
        }
    }
}

Topological::Topological(const digraph::Digraph& g) :
    marked_(g.V(), false)
{
    for (int s = 0; s < g.V(); ++s) {
        if (!marked_[s]) dfs_(g, s);
    }
}

const std::vector<int>& Topological::order() const noexcept {
    return order_;
}

} // namespace cfg::digraph::topological 

