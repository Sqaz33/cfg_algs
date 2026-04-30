#pragma once

#include "digraph.hpp"

namespace cfg::dj {

class DefJoin {
public:
    DefJoin(
        const digraph::Digraph& cfg, 
        const digraph::Digraph& dt);

public:
    const digraph::Digraph& dj() const noexcept;

private:
    digraph::Digraph dj_;
};

} // namespace cfg::dj