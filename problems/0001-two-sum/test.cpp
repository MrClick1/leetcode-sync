#include <iostream>
#include <vector>

#include "solution.cpp"

// 校验一组用例：结果必须是两个不同下标，且两数之和等于 target
static void check(const std::vector<int>& nums, int target,
                  const std::vector<int>& got) {
    if (got.size() != 2) {
        std::cerr << "FAIL: 返回值不是两个下标 (size=" << got.size() << ")\n";
        std::exit(1);
    }
    const int i = got[0];
    const int j = got[1];
    if (i == j || i < 0 || j < 0 ||
        static_cast<size_t>(i) >= nums.size() ||
        static_cast<size_t>(j) >= nums.size()) {
        std::cerr << "FAIL: 下标不合法 [" << i << ", " << j << "]\n";
        std::exit(1);
    }
    if (nums[i] + nums[j] != target) {
        std::cerr << "FAIL: nums[" << i << "] + nums[" << j
                  << "] = " << nums[i] + nums[j] << " != " << target << "\n";
        std::exit(1);
    }
}

int main() {
    // 示例用例
    {
        std::vector<int> nums = {2, 7, 11, 15};
        check(nums, 9, twoSum(nums, 9));
    }
    {
        std::vector<int> nums = {3, 2, 4};
        check(nums, 6, twoSum(nums, 6));
    }
    {
        std::vector<int> nums = {3, 3};
        check(nums, 6, twoSum(nums, 6));
    }

    // 边界：长度为 2、目标由两个最大值组成
    {
        std::vector<int> nums = {1000000000, 1000000000};
        check(nums, 2000000000, twoSum(nums, 2000000000));
    }
    // 边界：负数
    {
        std::vector<int> nums = {-1, -2, -3, -4, -5};
        check(nums, -8, twoSum(nums, -8));
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
