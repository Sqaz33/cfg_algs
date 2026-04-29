#include "digraph.hpp"

#include <string>

#include "graphviz.hpp"

namespace cfg::digraph {

DIGraph::DIGraph(int V) : V_(V), preds_(V) {} 

void DIGraph::createEdge(int u, int v) {
    V_[u].push_back(v);
    preds_[v].push_back(u);
}

const std::vector<int>& DIGraph::preds(int v) const {
    return preds_[v];
}

const std::vector<int>& DIGraph::adjs(int v) const {
    return V_[v];
}

void DIGraph::printDot(std::ostream& out) const {
    auto gv = graphviz::createGraphViz(false, true, "CFG");

    std::vector<graphviz::VertexType> vs(V_.size());
    
    for (int i = 0; i < V_.size(); ++i) {
        vs[i] = gv->addVertex(std::to_string(i));
    }

    for (int u = 0; u < V_.size(); ++u) {
        for (int v : V_[u]) {
            gv->addEdge(vs[u], vs[v]);
        }
    }

    gv->printDOT(out);
}

} // namespace cfg::digraph