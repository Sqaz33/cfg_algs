#include <fstream>

#include "random_control_flow.hpp"
#include "dominator.hpp"
#include "dt.hpp"
#include "df.hpp"
#include "dj.hpp"

int main() {
    cfg::random_cfg::RandomCFG rcfg;
    
    std::ofstream cfgOfs("cfg.dot", std::ios::trunc | std::ios::out);
    auto&& cfg = rcfg.cfg();

    // cfg::digraph::Digraph cfg(8);
    // cfg.createEdge(0, 1);

    // cfg.createEdge(1, 4);
    // cfg.createEdge(1, 2);

    // cfg.createEdge(2, 3);

    // cfg.createEdge(3, 1);

    // cfg.createEdge(4, 5);
    // cfg.createEdge(4, 7);

    // cfg.createEdge(5, 1);
    // cfg.createEdge(5, 6);
    // cfg.createEdge(5, 2);

    // cfg.createEdge(7, 6);

    // cfg.createEdge(6, 3);


    cfg.printDot(cfgOfs);
    cfgOfs.close();

    cfg::dominator::Dominators doms(cfg);
    cfg::dt::DominatorTree dt(doms, cfg);

    std::ofstream dtOfs("dt.dot", std::ios::trunc | std::ios::out);
    dt.dt().printDot(dtOfs);
    dtOfs.close();

    cfg::df::DominanceFrontier df(dt.idom(), cfg);
    std::ofstream dfOfs("df.dot", std::ios::trunc | std::ios::out);
    df.dft().printDot(dfOfs);

    cfg::dj::DefJoin dj(cfg, dt.dt());
    std::ofstream djOfs("dj.dot", std::ios::trunc | std::ios::out);
    dj.dj().printDot(djOfs);
}
// ./cfg_algs;  dot -Tpng dt.dot -o dt.png; dot -Tpng cfg.dot -o cfg.png; dot -Tpng df.dot -o df.png; dot -Tpng dj.dot -p dj.png