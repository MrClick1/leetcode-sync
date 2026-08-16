#include <iostream>
#include <vector>

#include "solution.cpp"

static void check(std::vector<int> nums, int expected) {
    int got = Solution().majorityElement(nums);
    if (got != expected) {
        std::cerr << "FAIL: expected " << expected << ", got " << got << "\n";
        std::exit(1);
    }
}

int main() {
    // 示例 1、2
    check({3, 2, 3}, 3);
    check({2, 2, 1, 1, 1, 2, 2}, 2);

    // 边界：单元素 / 两元素
    check({1}, 1);
    check({2, 2}, 2);

    // 全部相同
    check({5, 5, 5, 5}, 5);

    // 多数元素在开头
    check({1, 1, 2, 1}, 1);
    check({1, 1, 2}, 1);

    // 负数
    check({-1, -1, 2, -1}, -1);

    // 边界值
    check({-1000000000, 1000000000, -1000000000}, -1000000000);

    // n=3 的多数
    check({1, 2, 1}, 1);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
