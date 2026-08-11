#include <iostream>
#include <vector>

#include "solution.cpp"

static void check(std::vector<std::vector<char>> grid, int expected) {
    int got = Solution().numIslands(grid);
    if (got != expected) {
        std::cerr << "FAIL: 期望 " << expected << " 实际 " << got << "\n";
        std::exit(1);
    }
}

static std::vector<std::vector<char>> makeGrid(
    const std::vector<std::string>& rows) {
    std::vector<std::vector<char>> g;
    for (const auto& r : rows)
        g.push_back(std::vector<char>(r.begin(), r.end()));
    return g;
}

int main() {
    // 示例 1
    check(makeGrid({"11110", "11010", "11000", "00000"}), 1);

    // 示例 2
    check(makeGrid({"11000", "11000", "00100", "00011"}), 3);

    // 边界：1x1
    check(makeGrid({"1"}), 1);
    check(makeGrid({"0"}), 0);

    // 边界：单行
    check(makeGrid({"101"}), 2);

    // 边界：单列
    check(makeGrid({"1", "0", "1"}), 2);

    // 全水 / 全陆地
    check(makeGrid({"00", "00"}), 0);
    check(makeGrid({"11", "11"}), 1);

    // 对角线不相邻
    check(makeGrid({"10", "01"}), 2);

    // 直角相连 + 独立岛
    check(makeGrid({"110", "010", "001"}), 2);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}