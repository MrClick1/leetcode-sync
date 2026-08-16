#include <cstdlib>
#include <iostream>
#include <vector>

#include "solution.cpp"

static void printVector(const std::vector<int>& nums) {
    std::cerr << "[";
    for (std::size_t i = 0; i < nums.size(); ++i) {
        if (i > 0) {
            std::cerr << ",";
        }
        std::cerr << nums[i];
    }
    std::cerr << "]";
}

static void check(const std::vector<int>& input,
                  const std::vector<int>& expected) {
    std::vector<int> got = input;
    Solution().sortColors(got);

    if (got != expected) {
        std::cerr << "FAIL:\n  input:    ";
        printVector(input);
        std::cerr << "\n  expected: ";
        printVector(expected);
        std::cerr << "\n  got:      ";
        printVector(got);
        std::cerr << "\n";
        std::exit(1);
    }
}

int main() {
    // 示例
    check({2, 0, 2, 1, 1, 0}, {0, 0, 1, 1, 2, 2});
    check({2, 0, 1}, {0, 1, 2});

    // 边界：单元素
    check({0}, {0});
    check({1}, {1});
    check({2}, {2});

    // 已经有序 / 完全逆序
    check({0, 0, 1, 1, 2, 2}, {0, 0, 1, 1, 2, 2});
    check({2, 2, 1, 1, 0, 0}, {0, 0, 1, 1, 2, 2});

    // 全部相同
    check({0, 0, 0, 0}, {0, 0, 0, 0});
    check({1, 1, 1, 1}, {1, 1, 1, 1});
    check({2, 2, 2, 2}, {2, 2, 2, 2});

    // 只包含两种颜色
    check({2, 0, 2, 0}, {0, 0, 2, 2});
    check({1, 0, 1, 0}, {0, 0, 1, 1});
    check({2, 1, 2, 1}, {1, 1, 2, 2});

    // 混合排列
    check({1, 2, 0, 1, 2, 0, 1}, {0, 0, 1, 1, 1, 2, 2});
    check({2, 0, 0, 2, 1, 1, 0}, {0, 0, 0, 1, 1, 2, 2});

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
