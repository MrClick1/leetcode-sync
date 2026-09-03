#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    int largestRectangleArea(vector<int>& heights) {
        int n = static_cast<int>(heights.size());

        vector<int> left(n), right(n);

        stack<int> stk1;    // 保存下标，对应高度从栈底到栈顶严格递增

        for (int i = 0; i < n; i++) {
            int height = heights[i];    // 以当前柱子的高度作为矩形高度

            while (!stk1.empty() && heights[stk1.top()] >= height) {
                stk1.pop();
            }

            // 栈顶是左边第一个严格更矮的柱子的下标
            if (!stk1.empty()) {
                left[i] = stk1.top();
            } else {
                left[i] = -1;   // 左边没有更矮的柱子，矩形可延伸到下标 0
            }

            stk1.push(i);
        }

        // 从右向左找第一个严格更矮的柱子
        stack<int> stk2;    // 保存下标，对应高度从栈底到栈顶严格递增
        for (int i = n - 1; i >= 0; i--) {
            int height = heights[i];

            while (!stk2.empty() && heights[stk2.top()] >= height) {
                stk2.pop();
            }

            if (!stk2.empty()) {
                right[i] = stk2.top();
            } else {
                right[i] = n;    // 右边没有更矮的柱子，矩形可延伸到下标 n - 1
            }

            stk2.push(i);
        }

        // 计算矩形面积最大值
        int maxArea = 0;
        for (int i = 0; i < n; i++) {
            int height = heights[i];

            int width = right[i] - left[i] - 1; // 两个更矮的边界柱子都不计入宽度

            int area = height * width;
            maxArea = max(maxArea, area);
        }

        return maxArea;
    }
};
