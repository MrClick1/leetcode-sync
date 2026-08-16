#include <iostream>
#include <vector>

#include "solution.cpp"

static void printMatrix(const std::vector<std::vector<int>>& m) {
    std::cerr << "[";
    for (size_t i = 0; i < m.size(); ++i) {
        if (i) std::cerr << " ";
        std::cerr << "[";
        for (size_t j = 0; j < m[i].size(); ++j) {
            if (j) std::cerr << ",";
            std::cerr << m[i][j];
        }
        std::cerr << "]";
        if (i + 1 < m.size()) std::cerr << "\n";
    }
    std::cerr << "]";
}

static void check(const std::vector<std::vector<int>>& input,
                  const std::vector<std::vector<int>>& expected) {
    std::vector<std::vector<int>> got = input;
    Solution().setZeroes(got);
    if (got != expected) {
        std::cerr << "FAIL:\n  input:\n";
        printMatrix(input);
        std::cerr << "\n  expected:\n";
        printMatrix(expected);
        std::cerr << "\n  got:\n";
        printMatrix(got);
        std::cerr << "\n";
        std::exit(1);
    }
}

int main() {
    // 示例 1
    check({{1, 1, 1}, {1, 0, 1}, {1, 1, 1}},
          {{1, 0, 1}, {0, 0, 0}, {1, 0, 1}});

    // 示例 2
    check({{0, 1, 2, 0}, {3, 4, 5, 2}, {1, 3, 1, 5}},
          {{0, 0, 0, 0}, {0, 4, 5, 0}, {0, 3, 1, 0}});

    // 边界：1x1
    check({{0}}, {{0}});
    check({{5}}, {{5}});

    // 边界：单行 / 单列
    check({{0, 1}}, {{0, 0}});
    check({{1}, {0}}, {{0}, {0}});

    // 没有 0：矩阵不变
    check({{1, 2}, {3, 4}}, {{1, 2}, {3, 4}});

    // 陷阱 1：0 在 (0,1)，第一行/第一列会被用作标记
    check({{0, 1}, {1, 1}}, {{0, 0}, {0, 1}});

    // 陷阱 3：0 在第一行但不在第一列（列 0 不应该被清零）
    check({{1, 0}, {1, 1}}, {{0, 0}, {1, 0}});

    // 陷阱 3b：3x3，0 在 (0,2)，只有第一行和第 2 列清零
    check({{2, 1, 0}, {1, 3, 1}, {1, 1, 1}},
          {{0, 0, 0}, {1, 3, 0}, {1, 1, 0}});

    // 陷阱 2：0 在 (2,2)，最后一行/最后一列清零，注意不要把标记行误清
    check({{1, 1, 1}, {1, 1, 1}, {1, 1, 0}},
          {{1, 1, 0}, {1, 1, 0}, {0, 0, 0}});

    // 边界：极值负数 + 0
    check({{-2147483648, 2147483647}, {0, -1}},
          {{0, 2147483647}, {0, 0}});

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
