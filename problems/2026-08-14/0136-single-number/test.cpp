#include <iostream>
#include <vector>

#include "solution.cpp"

static void check(std::vector<int> nums, int expected) {
    int got = Solution().singleNumber(nums);
    if (got != expected) {
        std::cerr << "FAIL: expected " << expected << ", got " << got << "\n";
        std::exit(1);
    }
}

int main() {
    // 示例 1、2、3
    check({2, 2, 1}, 1);
    check({4, 1, 2, 1, 2}, 4);
    check({1}, 1);

    // 只出现一次的元素在末尾
    check({1, 1, 2}, 2);

    // 负数
    check({-1, -1, 2}, 2);
    check({1, -2, 1, -2, 3}, 3);

    // 只出现一次的是 0
    check({0, 0, 5}, 5);
    check({0}, 0);

    // 边界值
    check({-30000, 30000, -30000}, 30000);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}