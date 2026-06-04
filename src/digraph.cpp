#include "digraph.hpp"

#include <string>
#include <queue>

#include "graphviz.hpp"

namespace {

void dfpo(
    const cfg::digraph::Digraph& g, 
    int v, 
    std::vector<int>& res, 
    std::vector<bool>& marked) 
{
    marked[v] = true;
    for (int u : g.adjs(v)) {
        if (!marked[u]) {
            dfpo(g, u, res, marked);
        }
    }
    res.push_back(v);
}

} // namespace 

namespace cfg::digraph {

Digraph::Digraph(int V) : V_(V), preds_(V) {} 

void Digraph::createEdge(int u, int v) {
    V_[u].insert(v);
    preds_[v].insert(u);
    ++E_;
}

const std::set<int>& Digraph::preds(int v) const {
    return preds_[v];
}

const std::set<int>& Digraph::adjs(int v) const {
    return V_[v];
}

void Digraph::printDot(std::ostream& out) const {
    graphviz::GraphViz gv(false, true, "CFG");

    std::vector<graphviz::VertexType> vs(V_.size());
    
    for (int i = 0; i < V_.size(); ++i) {
        vs[i] = gv.addVertex(std::to_string(i));
    }

    for (int u = 0; u < V_.size(); ++u) {
        for (int v : V_[u]) {
            gv.addEdge(vs[u], vs[v]);
        }
    }

    gv.printDOT(out);
}

std::size_t Digraph::V() const noexcept {
    return V_.size();
}

std::size_t Digraph::E() const noexcept {
    return E_;
}

int Digraph::closest(const std::set<int>& s, int v) const {
    std::queue<int> q;
    q.push(v);
    std::vector<bool> mark(V(), false);

    while (!q.empty()) {
        int v = q.front();
        q.pop();
        mark[v] = true;

        if (s.contains(v)) {
            return v;
        }

        for (int u : preds(v)) {
            if (!mark[u]) {
                q.push(u);
            }
        }
    }

    return -1;
}

Digraph Digraph::reverse() const {
    Digraph d(*this);
    std::swap(d.preds_, d.V_);
    return d;
}

std::vector<int> Digraph::po() const {
    std::vector<int> res;
    res.reserve(V_.size());
    std::vector<bool> marked(V_.size(), false);
    dfpo(*this, 0, res, marked);
    return res;
}

} // namespace cfg::digraph
