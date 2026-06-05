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

private:
    digraph::Digraph graph_;
};

} // namespace cfg::random_cfg 