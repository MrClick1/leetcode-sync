#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    int majorityElement(vector<int>& nums) {
        int n = (int)nums.size();
        int count = 0;
        int candidate;

        for (int i = 0; i < n; i++) {
            if (count == 0) {
                candidate = nums[i];
                count++;
            }else if (nums[i] != candidate) {
                count--;
            }else {
                count++;
            }
        }

        return candidate;
    }
};