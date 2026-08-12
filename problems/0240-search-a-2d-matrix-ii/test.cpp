#include <iostream>
#include <vector>

#include "solution.cpp"

static void check(std::vector<std::vector<int>> matrix, int target,
                  bool expected) {
    bool got = Solution().searchMatrix(matrix, target);
    if (got != expected) {
        std::cerr << "FAIL: target=" << target
                  << " expected " << (expected ? "true" : "false")
                  << ", got " << (got ? "true" : "false") << "\n";
        std::exit(1);
    }
}

int main() {
    const std::vector<std::vector<int>> m = {
        {1, 4, 7, 11, 15},
        {2, 5, 8, 12, 19},
        {3, 6, 9, 16, 22},
        {10, 13, 14, 17, 24},
        {18, 21, 23, 26, 30}};

    // 示例 1、2
    check(m, 5, true);
    check(m, 20, false);

    // 边界：1x1
    check({{5}}, 5, true);
    check({{5}}, 3, false);

    // 边界：单行
    check({{1, 2, 3}}, 2, true);
    check({{1, 2, 3}}, 4, false);

    // 边界：单列
    check({{1}, {2}, {3}}, 2, true);
    check({{1}, {2}, {3}}, 0, false);

    // 比所有元素都小 / 都大
    check({{1, 2}, {3, 4}}, 0, false);
    check({{1, 2}, {3, 4}}, 5, false);

    // 负数
    check({{-5, -3}, {-2, 0}}, -3, true);
    check({{-5, -3}, {-2, 0}}, -1, false);

    // 最大值/最小值
    check({{-1000000000, 1000000000}}, 1000000000, true);
    check({{-1000000000, 1000000000}}, -1000000000, true);
    check({{-1000000000, 1000000000}}, 0, false);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}