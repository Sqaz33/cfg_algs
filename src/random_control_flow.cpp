#include "random_control_flow.hpp"

#include <algorithm>
#include <random>
#include <set>
#include <vector>

namespace cfg::random_cfg {

namespace {

struct Loop {
    int header{};
    int latch{};
    int after{};
};

} // namespace

RandomCFG::RandomCFG(int vertex) : graph_(vertex) {
    gen_(vertex);
}

const digraph::Digraph& RandomCFG::cfg() const noexcept {
    return graph_;
}

void RandomCFG::gen_(int vertex) {
    if (vertex <= 1) {
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    auto randInt = [&gen](int left, int right) {
        std::uniform_int_distribution<int> dist(left, right);
        return dist(gen);
    };

    std::set<std::pair<int, int>> emitted;

    auto addEdge = [&](int u, int v) {
        if (u < 0 || v < 0 || u >= vertex || v >= vertex || u == v) {
            return;
        }

        const auto [_, inserted] = emitted.emplace(u, v);

        if (inserted) {
            graph_.createEdge(u, v);
        }
    };

    std::vector<Loop> loops;

    auto intersectsExistingLoop = [&loops](int header, int latch) {
        for (const auto& loop : loops) {
            // Запрещаем пересечение циклов и слишком плотное наложение интервалов.
            // Это сохраняет CFG похожим на структурированную программу.
            if (!(latch + 1 < loop.header || header > loop.latch + 1)) {
                return true;
            }
        }

        return false;
    };

    // 1. Планируем циклы.
    //
    // Цикл задается интервалом:
    //
    //     header ... latch after
    //
    // Обратная дуга будет только одна:
    //
    //     latch -> header
    //
    // Выходы из цикла будут вести в after.
    if (vertex >= 6) {
        const int maxLoops = std::max(1, vertex / 10 + 1);
        const int desiredLoops = randInt(1, maxLoops);

        for (int attempt = 0;
             attempt < vertex * 12 && static_cast<int>(loops.size()) < desiredLoops;
             ++attempt) {

            const int header = randInt(1, vertex - 4);

            const int maxLen = std::min(8, vertex - 2 - header);

            if (maxLen < 2) {
                continue;
            }

            const int latch = header + randInt(2, maxLen);

            if (latch >= vertex - 1) {
                continue;
            }

            if (intersectsExistingLoop(header, latch)) {
                continue;
            }

            loops.push_back(Loop{
                .header = header,
                .latch = latch,
                .after = latch + 1,
            });
        }

        std::sort(
            loops.begin(),
            loops.end(),
            [](const Loop& lhs, const Loop& rhs) {
                return lhs.header < rhs.header;
            }
        );
    }

    std::vector<int> latchToHeader(vertex, -1);

    for (const auto& loop : loops) {
        latchToHeader[loop.latch] = loop.header;
    }

    // 2. Строим основной прямой поток.
    //
    // Почти все вершины получают ребро i -> i + 1.
    // Исключение: latch цикла. У latch вместо обычного fallthrough сначала
    // создается обратная дуга latch -> header, а выход latch -> after добавляется
    // только если он выбран как один из loop exits.
    for (int i = 0; i + 1 < vertex; ++i) {
        if (latchToHeader[i] != -1) {
            continue;
        }

        addEdge(i, i + 1);
    }

    // 3. Добавляем циклы.
    //
    // Инварианты:
    // - у каждого цикла есть header;
    // - ровно одна обратная дуга latch -> header;
    // - от 1 до 3 выходных дуг из интервала цикла наружу;
    // - все выходы ведут в after, то есть в нормальную точку продолжения.
    for (const auto& loop : loops) {
        addEdge(loop.latch, loop.header); // единственная обратная дуга цикла

        addEdge(loop.header, loop.after); // первый обязательный выход

        const int extraExits = randInt(0, 2);

        std::vector<int> exitSources;

        for (int v = loop.header + 1; v <= loop.latch; ++v) {
            exitSources.push_back(v);
        }

        std::shuffle(exitSources.begin(), exitSources.end(), gen);

        int added = 0;

        for (int source : exitSources) {
            if (added >= extraExits) {
                break;
            }

            addEdge(source, loop.after);
            ++added;
        }
    }

    auto containingLoop = [&loops](int vertexId) {
        for (int i = 0; i < static_cast<int>(loops.size()); ++i) {
            const auto& loop = loops[i];

            if (loop.header <= vertexId && vertexId <= loop.latch) {
                return i;
            }
        }

        return -1;
    };

    auto isIllegalTarget = [&loops, &containingLoop](int source, int target) {
        const int sourceLoop = containingLoop(source);

        for (int i = 0; i < static_cast<int>(loops.size()); ++i) {
            const auto& loop = loops[i];

            // Нельзя прыгать в середину чужого цикла.
            // Вход в цикл должен идти через header.
            if (loop.header < target && target <= loop.latch && sourceLoop != i) {
                return true;
            }
        }

        return false;
    };

    std::vector<int> outDegree(vertex, 0);

    for (const auto& [u, v] : emitted) {
        (void)v;
        ++outDegree[u];
    }

    // 4. Добавляем forward-ветвления.
    //
    // Это имитирует if, short-circuit, early-exit внутри линейного кода.
    // Принципиально:
    // - только forward edges;
    // - latch не получает случайных дополнительных ребер;
    // - внутрь чужого цикла прыгать нельзя;
    // - степень выхода сильно ограничена, чтобы не получить "паутину".
    int branchBudget = std::max(0, vertex / 3);

    for (int attempt = 0; attempt < vertex * 5 && branchBudget > 0; ++attempt) {
        const int source = randInt(0, vertex - 3);

        if (latchToHeader[source] != -1) {
            continue;
        }

        if (outDegree[source] >= 2) {
            continue;
        }

        const int sourceLoop = containingLoop(source);

        const int low = source + 2;
        int high = -1;

        if (sourceLoop != -1) {
            // Внутри цикла случайные ветвления остаются внутри этого же цикла.
            // Выходы наружу уже контролируемо добавлены выше.
            high = loops[sourceLoop].latch;
        } else {
            // Вне циклов добавляем короткие forward-прыжки, а не ребра через
            // половину графа.
            high = std::min(vertex - 1, source + randInt(2, 6));
        }

        if (low > high) {
            continue;
        }

        for (int retry = 0; retry < 8; ++retry) {
            const int target = randInt(low, high);

            if (isIllegalTarget(source, target)) {
                continue;
            }

            if (emitted.find(std::pair<int, int>{source, target}) != emitted.end()) {
                continue;
            }

            addEdge(source, target);
            ++outDegree[source];
            --branchBudget;

            break;
        }
    }
}
} // namespace cfg::random_cfg