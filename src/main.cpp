#include <fstream>
#include <string>
#include <memory>
#include <iostream>

#include "random_control_flow.hpp"
#include "dominator.hpp"
#include "dt.hpp"
#include "df.hpp"
#include "dj.hpp"
#include "dpo.hpp"

int main(int argc, char** argv) {
    int V = 7;
    std::unique_ptr<cfg::digraph::Digraph> bufCFG;

    if (2 == argc) {
        V = std::stoi(argv[1]);
        cfg::random_cfg::RandomCFG rcfg(V);
        bufCFG = std::make_unique<cfg::digraph::Digraph>(rcfg.cfg());
    } else if (3 == argc) {
        V = std::stoi(argv[1]);
        bufCFG = std::make_unique<cfg::digraph::Digraph>(V);
        int E;
        std::cin >> E;
        while (E--) {
            int u, v;
            std::cin >> u >> v;
            bufCFG->createEdge(u, v);
        }
    } else {
        return 1;
    }

    auto&& cfg = *bufCFG.get();

    std::ofstream cfgOfs("cfg.dot", std::ios::trunc | std::ios::out);
    cfg.printDot(cfgOfs);
    cfgOfs.close();

    cfg::dominator::Dominators doms(cfg);
    cfg::dt::DominatorTree dt(doms, cfg);

    cfg::dpo::DPO dpo(cfg);
    std::ofstream dpoOfs("dpo.dot", std::ios::trunc | std::ios::out);
    dpo.dt().printDot(dpoOfs);
    dpoOfs.close();

    std::ofstream dtOfs("dt.dot", std::ios::trunc | std::ios::out);
    dt.dt().printDot(dtOfs);
    dtOfs.close();

    cfg::df::DominanceFrontier df(dt.idom(), cfg);
    std::ofstream dfOfs("df.dot", std::ios::trunc | std::ios::out);
    df.dft().printDot(dfOfs);

    cfg::dj::DefJoin dj(cfg, dt.dt());
    std::ofstream djOfs("dj.dot", std::ios::trunc | std::ios::out);
    dj.dj().printDot(djOfs);

    return 0;
}
// ./cfg_algs;  dot -Tpng dt.dot -o dt.png; dot -Tpng cfg.dot -o cfg.png; dot -Tpng df.dot -o df.png; dot -Tpng dj.dot -o dj.png