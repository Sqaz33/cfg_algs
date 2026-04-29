#pragma once

#include "digraph.hpp"

namespace cfg::dj {

class DefJoin {
public:
    DefJoin(
        const digraph::DIGraph& cfg, 
        const digraph::DIGraph& dt);

public:
    const digraph::DIGraph& dj() const noexcept;

private:
    digraph::DIGraph dj_;
};

} // namespace cfg::dj