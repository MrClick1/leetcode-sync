#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        int m = (int)matrix.size();     // 行数
        int n = (int)matrix[0].size();  // 列数
        
        int top = 0, bottom = m-1, left = 0, right = n-1;

        vector<int> result;

        while (top <= bottom && left <= right) {
            if (top > bottom || left > right) break;
            // 向右
            for (int j = left; j <= right; j++) {
                result.push_back(matrix[top][j]);
            }
            top++;
            
            if (top > bottom || left > right) break;
            // 向下
            for (int i = top; i <= bottom; i++) {
                result.push_back(matrix[i][right]);
            }
            right--;
            
            if (top > bottom || left > right) break;
            // 向左
            for (int j = right; j >= left; j--) {
                result.push_back(matrix[bottom][j]);
            }
            bottom--;
            
            if (top > bottom || left > right) break;
            // 向上
            for (int i = bottom; i >= top; i--) {
                result.push_back(matrix[i][left]);
            }
            left++;
        }

        return result;
    }
};
