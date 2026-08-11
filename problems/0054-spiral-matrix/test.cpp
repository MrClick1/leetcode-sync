#include <iostream>
#include <vector>

#include "solution.cpp"

static void printVector(const std::vector<int>& v) {
    std::cerr << "[";
    for (size_t i = 0; i < v.size(); ++i) {
        if (i) std::cerr << ",";
        std::cerr << v[i];
    }
    std::cerr << "]";
}

static void check(const std::vector<std::vector<int>>& input,
                  const std::vector<int>& expected) {
    std::vector<std::vector<int>> m = input;
    std::vector<int> got = Solution().spiralOrder(m);
    if (got != expected) {
        std::cerr << "FAIL:\n  input:    [";
        for (size_t i = 0; i < input.size(); ++i) {
            if (i) std::cerr << ",";
            std::cerr << "[";
            for (size_t j = 0; j < input[i].size(); ++j) {
                if (j) std::cerr << ",";
                std::cerr << input[i][j];
            }
            std::cerr << "]";
        }
        std::cerr << "]\n  expected: ";
        printVector(expected);
        std::cerr << "\n  got:      ";
        printVector(got);
        std::cerr << "\n";
        std::exit(1);
    }
}

int main() {
    // 示例 1
    check({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}},
          {1, 2, 3, 6, 9, 8, 7, 4, 5});

    // 示例 2
    check({{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}},
          {1, 2, 3, 4, 8, 12, 11, 10, 9, 5, 6, 7});

    // 边界：1x1
    check({{1}}, {1});

    // 边界：单行
    check({{1, 2, 3, 4}}, {1, 2, 3, 4});

    // 边界：单列
    check({{1}, {2}, {3}, {4}}, {1, 2, 3, 4});

    // 边界：2x2
    check({{1, 2}, {3, 4}}, {1, 2, 4, 3});

    // 边界：2x3（行少列多）
    check({{1, 2, 3}, {4, 5, 6}}, {1, 2, 3, 6, 5, 4});

    // 边界：3x2（行多列少）
    check({{1, 2}, {3, 4}, {5, 6}}, {1, 2, 4, 6, 5, 3});

    // 完整 4x4 螺旋
    check({{1, 2, 3, 4},
           {5, 6, 7, 8},
           {9, 10, 11, 12},
           {13, 14, 15, 16}},
          {1, 2, 3, 4, 8, 12, 16, 15, 14, 13, 9, 5, 6, 7, 11, 10});

    // 负数
    check({{-1, -2}, {-3, -4}}, {-1, -2, -4, -3});

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
