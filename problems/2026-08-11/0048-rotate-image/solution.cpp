#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    void rotate(vector<vector<int>>& matrix) {
        int n = (int)matrix.size();

        for (int i = 0; i < n; i++) {
            for (int j = i; j < n; j++) {
                swap(matrix[i][j], matrix[j][i]);
            }
        }

        int half = n / 2;       // 4 / 2 == 2 || 3 / 2 == 1

        for (int i = 0; i < n; i++) {
            for (int j = 0; j <= half - 1; j++) {
                swap(matrix[i][j], matrix[i][n-1-j]);
            }
        }
    }
};
