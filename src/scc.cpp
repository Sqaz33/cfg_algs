#include "scc.hpp"

#include <stack>
#include <ranges>

#include "topological.hpp"


namespace cfg::digraph::scc {

SCC::SCC(const digraph::Digraph& g) :
    marked_(g.V(), false)
    , id_(g.V())
{
    topological::Topological t(g.reverse());
    for (auto s : std::ranges::reverse_view(t.order())) {
        if (!marked_[s]) { 
            dfs_(g, s);
            ++count_;
        }
    }
}

void SCC::dfs_(const digraph::Digraph& g, int src) {
    using namespace digraph;

    std::stack<int> stack;
    stack.push(src);
    marked_[src] = true;

    while (!stack.empty()) {
        auto v = stack.top(); stack.pop();
        auto&& adj = g.adjs(v);
        id_[v] = count_; 
        for (auto w : adj) {
            if (!marked_[w]) {
                marked_[w] = true;
                stack.push(w);
            }
        }   
    }
}

const std::vector<std::size_t>& SCC::id() const noexcept {
    return id_;
} 

} // namespace cfg::digraph::scc

