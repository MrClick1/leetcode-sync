#include <iostream>
#include <vector>

#include "solution.cpp"

static void check(std::vector<std::vector<int>> grid, int expected) {
    int got = Solution().orangesRotting(grid);
    if (got != expected) {
        std::cerr << "FAIL: expected " << expected << ", got " << got << "\n";
        std::exit(1);
    }
}

int main() {
    // 示例 1
    check({{2, 1, 1}, {1, 1, 0}, {0, 1, 1}}, 4);

    // 示例 2（有橘子永远烂不了）
    check({{2, 1, 1}, {0, 1, 1}, {1, 0, 1}}, -1);

    // 示例 3（一开始就没有新鲜橘子）
    check({{0, 2}}, 0);

    // 边界：只有一个橘子
    check({{2}}, 0);
    check({{1}}, -1);
    check({{0}}, 0);

    // 边界：全烂 / 全空
    check({{2, 2}, {2, 2}}, 0);
    check({{0, 0}, {0, 0}}, 0);

    // 单个新鲜橘子相邻
    check({{1, 2}}, 1);

    // 两个腐烂源同时扩散（中心需要 2 分钟）
    check({{2, 1, 1}, {1, 1, 1}, {1, 1, 2}}, 2);

    // 新鲜橘子被墙隔开，永远烂不了
    check({{1, 0}, {0, 2}}, -1);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}