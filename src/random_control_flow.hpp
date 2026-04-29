// Matveev S.A. 2026

/**
 * В реальном графе потока управление
 *  1. скорее всего нет изолированных вершин
 *  2. почти все вершины достижимы из entry
 *  3. маленькая степень вершниы
 *  4. ...
 *  ...
 * 
 * я пока делаю по 3
*/

#pragma once

#include "digraph.hpp"

namespace bb_graphs::random_cfg {

class RandomCFG {
public:
    RandomCFG();

public:

    const digraph::DIGraph& cfg() const noexcept;

private:
    digraph::DIGraph graph_;
};

} // namespace bb_graphs::random_cfg 