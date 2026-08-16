#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    void nextPermutation(vector<int>& nums) {
        // 1. 先找到第一个升序的数对
        int i = (int)nums.size()-2;

        while (i >= 0 && nums[i] >= nums[i+1]) {
            i--;
        }

        // 2. 找到升序的数对，找到第一个比 nums[i] 大的数
        if (i >= 0) {
            int j = (int)nums.size()-1;
            while (j >= 0 && nums[j] <= nums[i]) {
                j--;
            }
            swap(nums[i], nums[j]);
        }

        // 3. 反转 nums[i+1] 到 nums[n-1]
        reverse(nums.begin()+i+1, nums.end());
    }
};
