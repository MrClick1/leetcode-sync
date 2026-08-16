#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    int findDuplicate(vector<int>& nums) {
        int slow = 0;
        int fast = 0;

        while (1) {
            slow = nums[slow];
            fast = nums[nums[fast]];

            if (slow == fast) {
                break;
            }
        }

        int slow2 = 0;
        while (1) {
            slow2 = nums[slow2];
            fast = nums[fast];
            if (slow2 == fast) {
                return slow2;
            }
        }

        return 0;
    }
};
