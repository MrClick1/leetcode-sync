#include <cmath>
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

static void check(const std::vector<int>& input1,
                  const std::vector<int>& input2,
                  double expected) {
    std::vector<int> nums1 = input1;
    std::vector<int> nums2 = input2;
    double got = Solution().findMedianSortedArrays(nums1, nums2);

    if (std::fabs(got - expected) > 1e-9) {
        std::cerr << "FAIL: expected " << expected << ", got " << got
                  << "\n  nums1: ";
        printVector(input1);
        std::cerr << "\n  nums2: ";
        printVector(input2);
        std::cerr << "\n";
        std::exit(1);
    }
}

int main() {
    // 示例
    check({1, 3}, {2}, 2.0);
    check({1, 2}, {3, 4}, 2.5);

    // 一个数组为空
    check({}, {1}, 1.0);
    check({}, {2, 3}, 2.5);
    check({4, 5, 6}, {}, 5.0);

    // 单元素数组
    check({1}, {2}, 1.5);
    check({2}, {1, 3}, 2.0);

    // 重复元素和 0
    check({1, 1}, {1, 1}, 1.0);
    check({0, 0}, {0, 0}, 0.0);
    check({1, 2, 2}, {2, 2, 3}, 2.0);

    // 负数与数值边界
    check({-5, -3, -1}, {-2}, -2.5);
    check({-1000000}, {1000000}, 0.0);

    // 两个数组的数值区间分离或交错
    check({1, 2, 3}, {10, 11, 12}, 6.5);
    check({1, 3, 8}, {2, 7, 10, 11}, 7.0);
    check({1}, {2, 3, 4, 5, 6}, 3.5);

    // 较大交错输入
    std::vector<int> evens;
    std::vector<int> odds;
    evens.reserve(1000);
    odds.reserve(1000);
    for (int i = 0; i < 1000; ++i) {
        evens.push_back(2 * i);
        odds.push_back(2 * i + 1);
    }
    check(evens, odds, 999.5);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
