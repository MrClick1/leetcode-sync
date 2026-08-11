#include <iostream>
#include <vector>

#include "solution.cpp"

static void printMatrix(const std::vector<std::vector<int>>& m) {
    std::cerr << "[";
    for (size_t i = 0; i < m.size(); ++i) {
        if (i) std::cerr << ",";
        std::cerr << "[";
        for (size_t j = 0; j < m[i].size(); ++j) {
            if (j) std::cerr << ",";
            std::cerr << m[i][j];
        }
        std::cerr << "]";
    }
    std::cerr << "]";
}

static void check(const std::vector<std::vector<int>>& input,
                  const std::vector<std::vector<int>>& expected) {
    std::vector<std::vector<int>> got = input;
    Solution().rotate(got);
    if (got != expected) {
        std::cerr << "FAIL:\n  input:    ";
        printMatrix(input);
        std::cerr << "\n  expected: ";
        printMatrix(expected);
        std::cerr << "\n  got:      ";
        printMatrix(got);
        std::cerr << "\n";
        std::exit(1);
    }
}

int main() {
    // 示例 1
    check({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}},
          {{7, 4, 1}, {8, 5, 2}, {9, 6, 3}});

    // 示例 2
    check({{5, 1, 9, 11}, {2, 4, 8, 10}, {13, 3, 6, 7}, {15, 14, 12, 16}},
          {{15, 13, 2, 5}, {14, 3, 4, 1}, {12, 6, 8, 9}, {16, 7, 10, 11}});

    // 边界：1x1
    check({{1}}, {{1}});

    // 边界：2x2
    check({{1, 2}, {3, 4}}, {{3, 1}, {4, 2}});

    // 负数
    check({{-1, -2}, {-3, -4}}, {{-3, -1}, {-4, -2}});

    // 5x5 递增矩阵
    check({{1, 2, 3, 4, 5},
           {6, 7, 8, 9, 10},
           {11, 12, 13, 14, 15},
           {16, 17, 18, 19, 20},
           {21, 22, 23, 24, 25}},
          {{21, 22, 23, 24, 25},
           {16, 17, 18, 19, 20},
           {11, 12, 13, 14, 15},
           {6, 7, 8, 9, 10},
           {1, 2, 3, 4, 5}});

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
