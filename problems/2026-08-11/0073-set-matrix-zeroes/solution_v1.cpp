#include <vector>
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    void setZeroes(vector<vector<int>>& matrix) {
        int m = (int)matrix.size();     // 行数
        int n = (int)matrix[0].size();  // 列数

        vector<int> rows;
        vector<int> cols;

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (matrix[i][j] == 0) {
                    rows.push_back(i);
                    cols.push_back(j);
                }
            }
        }

        // 在原地进行修改
        for (int i = 0; i < (int)rows.size(); i++) {
            int row = rows[i];
            for (int col = 0; col < n; col++) {
                matrix[row][col] = 0;
            }
        }

        for (int j = 0; j < (int)cols.size(); j++) {
            int col = cols[j];
            for (int row = 0; row < m; row++) {
                matrix[row][col] = 0;
            }
        }
    } 
};
