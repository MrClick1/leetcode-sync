#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    int orangesRotting(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        queue<pair<int, int>> q;
        int fresh = 0;

        vector<pair<int, int>> directions = {
            {-1, 0},    // 上
            {1, 0},     // 下
            {0, -1},    // 左
            {0, 1},    // 右
        };

        // 第一步：统计新鲜橘子，并把所有腐烂橘子入队（多源！）
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 1) fresh++;
                else if (grid[i][j] == 2) q.push({i, j});
            }
        }

        int minutes = 0;
        // 第二步：按"层"BFS，一层 = 1 分钟
        while (!q.empty()) {
            int size = q.size();          // 当前这一分钟能扩散的橘子数
            bool rotten = false;

            for (int k = 0; k < size; k++) {
                auto cur = q.front();
                q.pop();
                // TODO: 四个方向，若邻居是 1（新鲜）：
                //       改成 2（腐烂）、fresh--、入队
                for (int i = 0; i < 4; i++) {
                    int ni = cur.first + directions[i].first;
                    int nj = cur.second + directions[i].second;
                    if (ni >= 0 && ni < m && nj >= 0 && nj < n && grid[ni][nj] == 1) {
                        grid[ni][nj] = 2;
                        fresh--;
                        q.push({ni, nj});
                        rotten = true;
                    }
                }
            }
            if (rotten) minutes++;  // 过了一分钟
        }

        // 第三步：还有新鲜橘子说明永远烂不完
        return fresh == 0 ? minutes : -1;
    }
};