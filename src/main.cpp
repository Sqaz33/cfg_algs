#include "random_control_flow.hpp"

#include <fstream>

int main() {
    cfg::random_cfg::RandomCFG rcfg;
    
    std::ofstream ofs("cfg.dot", std::ios::trunc | std::ios::out);

    auto&& graph = rcfg.cfg();
    graph.printDot(ofs);
}