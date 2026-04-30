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

namespace cfg::random_cfg {

class RandomCFG {
public:
    RandomCFG(int vertex = 10);

public:
    const digraph::Digraph& cfg() const noexcept;

private:
    void gen_(int vertex);
    void linkComponentsWithEntry_();

private:
    digraph::Digraph graph_;
};

} // namespace cfg::random_cfg 