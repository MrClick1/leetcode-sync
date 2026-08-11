#include <vector>
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    void setZeroes(vector<vector<int>>& matrix) {
        int m = (int)matrix.size();     // 行数
        int n = (int)matrix[0].size();  // 列数
        bool firstRowZero = false;      // 第一行除了第一个元素外是否有 0
        
        // 检查第一行是否原始有0
        for (int j = 0; j < n; j++) {
            if (matrix[0][j] == 0) {
                firstRowZero = true;
                break;
            }
        }
        
        // 遍历矩阵，但不遍历第一行，会对第一行第一个元素进行标记，导致产生新的0，在列置0时会多余
        for (int i = 1; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (matrix[i][j] == 0) {
                    matrix[i][0] = 0;
                    matrix[0][j] = 0;
                }
            }
        }

        // 在原地进行修改
        // 遍历行，看第一个元素是否为0，为0，当前行全置0
        for (int i = 1; i < m; i++) {
            if (matrix[i][0] == 0) {
                for (int j = 0; j < n; j++) {
                    matrix[i][j] = 0;
                }
            }
        }

        // 遍历列，看第一个元素是否为0，为0，当前列全置0
        for (int j = 0; j < n; j++) {
            if (matrix[0][j] == 0) {
                for (int i = 0; i < m; i++) {
                    matrix[i][j] = 0;
                }
            }
        }

        // 第一行的第一个元素跳过了，补齐，根据前面的信息判断是原来第一行就有0还是后加的
        if (firstRowZero) {
            for (int j = 0; j < n; j++) {
                matrix[0][j] = 0;
            }
        }
    } 
};
