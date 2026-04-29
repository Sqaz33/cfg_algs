#include "digraph.hpp"

#include <string>
#include <queue>

#include "graphviz.hpp"

namespace cfg::digraph {

DIGraph::DIGraph(int V) : V_(V), preds_(V) {} 

void DIGraph::createEdge(int u, int v) {
    V_[u].push_back(v);
    preds_[v].push_back(u);
    ++E_;
}

const std::vector<int>& DIGraph::preds(int v) const {
    return preds_[v];
}

const std::vector<int>& DIGraph::adjs(int v) const {
    return V_[v];
}

void DIGraph::printDot(std::ostream& out) const {
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

std::size_t DIGraph::V() const noexcept {
    return V_.size();
}

std::size_t DIGraph::E() const noexcept {
    return E_;
}

int DIGraph::closest(const std::set<int>& s, int v) const {
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

} // namespace cfg::digraph