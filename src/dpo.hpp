#pragma once 

#include <vector>

#include "digraph.hpp"

namespace cfg::dpo {

class DPO {
public:
    DPO(const digraph::Digraph& cfg);

public:
    const digraph::Digraph& dt() const noexcept;
    const std::vector<int>& idom() const noexcept;

private:
    void fillDT_();

private:
    std::vector<int> idom_;
    digraph::Digraph dt_;
};

} // namespace cfg::dpo