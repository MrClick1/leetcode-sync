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

static void check(const std::vector<int>& input, int expected) {
    std::vector<int> nums = input;
    int got = Solution().findDuplicate(nums);

    if (nums != input) {
        std::cerr << "FAIL: solution modified nums\n  before: ";
        printVector(input);
        std::cerr << "\n  after:  ";
        printVector(nums);
        std::cerr << "\n";
        std::exit(1);
    }

    if (got != expected) {
        std::cerr << "FAIL: expected " << expected << ", got " << got
                  << "\n  input: ";
        printVector(input);
        std::cerr << "\n";
        std::exit(1);
    }
}

int main() {
    // 示例
    check({1, 3, 4, 2, 2}, 2);
    check({3, 1, 3, 4, 2}, 3);
    check({3, 3, 3, 3, 3}, 3);

    // 最小规模
    check({1, 1}, 1);

    // 重复数位于数值边界
    check({1, 2, 3, 4, 1}, 1);
    check({5, 1, 2, 3, 4, 5}, 5);

    // 重复数出现在不同位置
    check({2, 1, 2}, 2);
    check({6, 1, 4, 3, 2, 5, 6}, 6);
    check({4, 2, 1, 3, 4}, 4);

    // 较大输入
    std::vector<int> large;
    large.reserve(100001);
    for (int value = 1; value <= 100000; ++value) {
        large.push_back(value);
    }
    large.push_back(54321);
    std::swap(large[0], large[70000]);
    std::swap(large[12345], large[99999]);
    check(large, 54321);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
