#pragma once

#include "digraph.hpp"

#include <set>

namespace cfg::dominator {

class Dominators {
public:
    Dominators(const digraph::DIGraph& cfg);

public:
    const std::set<int>& domset(int v) const;

private:
    std::vector<std::set<int>> domsets_;
};

} // namespace cfg::dominator