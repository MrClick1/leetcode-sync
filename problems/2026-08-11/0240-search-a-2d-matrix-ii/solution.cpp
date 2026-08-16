#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        int m = matrix.size();
        int n = matrix[0].size();

        int row = 0, col = n -1;

        while (row < m && col >= 0) {
            if (matrix[row][col] > target) {
                col--;
            }else if (matrix[row][col] < target) {
                row++;
            }else {
                return true;
            }
        }

        return false;
    }
};