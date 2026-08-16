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
    Solution().nextPermutation(got);

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
    check({1, 2, 3}, {1, 3, 2});
    check({3, 2, 1}, {1, 2, 3});
    check({1, 1, 5}, {1, 5, 1});

    // 边界：单元素 / 两元素
    check({1}, {1});
    check({1, 2}, {2, 1});
    check({2, 1}, {1, 2});

    // 全部相同
    check({2, 2, 2}, {2, 2, 2});

    // 重复元素
    check({1, 5, 1}, {5, 1, 1});
    check({1, 1, 2, 2}, {1, 2, 1, 2});
    check({2, 2, 0, 1}, {2, 2, 1, 0});

    // 需要调整较长后缀
    check({1, 3, 2}, {2, 1, 3});
    check({2, 3, 1}, {3, 1, 2});
    check({1, 2, 3, 6, 5, 4}, {1, 2, 4, 3, 5, 6});

    // 数值边界
    check({0, 100, 100}, {100, 0, 100});

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
