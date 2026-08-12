#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    int numIslands(vector<vector<char>>& grid) {
        int result = 0;

        int m = grid.size();
        int n = grid[0].size();

        const vector<pair<int, int>> directions(
            {
                {-1, 0},    // 上
                {1, 0},     // 下
                {0, -1},    // 左
                {0, 1}      // 右
            }
        );

        vector<vector<bool>> visited(m, vector<bool>(n, false));

        queue<pair<int, int>> que;      // 队列，用于 BFS

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == '1' && visited[i][j] == false) {
                    que.push(pair<int, int>({i, j}));
                    visited[i][j] = true;

                    // 进行 BFS
                    while (!que.empty()) {
                        int num = que.size();
                        for (int i = 0; i < num; i++) {
                            pair<int, int> cur = que.front();
                            que.pop();
                            // 放入上下左右的位置
                            for (int k = 0; k < 4; k++) {
                                if (
                                    cur.first + directions[k].first >= 0 &&
                                    cur.first + directions[k].first < m &&
                                    cur.second + directions[k].second >= 0 &&
                                    cur.second + directions[k].second < n
                                ) {
                                    int row = cur.first + directions[k].first;
                                    int col = cur.second + directions[k].second;

                                    if (grid[row][col] == '1' && visited[row][col] == false) {
                                        que.push({row, col});
                                        visited[row][col] = true;
                                    }
                                }
                            }
                        }
                    }
                    result++;
                }
            }
        }
        return result;
    }
};