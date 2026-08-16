#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    int partition(vector<int>& nums) {
        int n = (int)nums.size();
        int pivot = nums[n-1];
        
        int left = 0;
        int right = n-1;

        while (left < right) {
            while (left < right && nums[left] <= nums[right]) {
                left++;
            }
            if (left < right) {
                nums[right] = nums[left];
            }

            while (left < right && nums[left] <= nums[right]) {
                right--;
            }
            if (left < right) {
                nums[left] = nums[right];
            }
        }
        nums[left] = pivot;

        return left;
    }

    int findKth(vector<int>& nums, int k) {
        int n = (int)nums.size();
        int left = 0;
        int right = n-1;

        while (true) {
            int pos = partition(nums);
            if (pos == k) {
                return nums[pos];
            } else if (pos < n-k) {
                left = pos+1;
            } else {
                right = pos-1;
            }
        }
    }

    int majorityElement(vector<int>& nums) {
        vector<int> nums_copy(nums);

        int n = (int)nums.size();
        int left = 0;
        int right = n-1;

        return 0;
    }
};