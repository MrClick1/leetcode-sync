# 200. 岛屿数量（二刷）

状态：✅ 已解决（2026-09-13，原地沉岛 BFS 通过 LeetCode）

## 题目要求

给定由 `'1'`（陆地）和 `'0'`（水）组成的二维网格，统计由上下左右相邻陆地组成的岛屿数量。

## 解法：扫描网格 + BFS 沉岛

依次扫描每个格子：

- 遇到 `'0'`：跳过。
- 遇到 `'1'`：发现一座尚未访问的岛屿，答案加一，并从这里执行 BFS。
- BFS 将与起点连通的所有 `'1'` 改成 `'0'`，后续扫描就不会重复计算这座岛。

```cpp
class Solution {
public:
    void bfs(vector<vector<char>>& grid, int r, int c) {
        int rows = static_cast<int>(grid.size());
        int cols = static_cast<int>(grid[0].size());

        queue<pair<int, int>> que;
        que.push({r, c});
        grid[r][c] = '0';

        while (!que.empty()) {
            auto position = que.front();
            que.pop();

            int currentRow = position.first;
            int currentCol = position.second;

            if (currentRow - 1 >= 0 &&
                grid[currentRow - 1][currentCol] == '1') {
                grid[currentRow - 1][currentCol] = '0';
                que.push({currentRow - 1, currentCol});
            }

            if (currentRow + 1 < rows &&
                grid[currentRow + 1][currentCol] == '1') {
                grid[currentRow + 1][currentCol] = '0';
                que.push({currentRow + 1, currentCol});
            }

            if (currentCol - 1 >= 0 &&
                grid[currentRow][currentCol - 1] == '1') {
                grid[currentRow][currentCol - 1] = '0';
                que.push({currentRow, currentCol - 1});
            }

            if (currentCol + 1 < cols &&
                grid[currentRow][currentCol + 1] == '1') {
                grid[currentRow][currentCol + 1] = '0';
                que.push({currentRow, currentCol + 1});
            }
        }
    }

    int numIslands(vector<vector<char>>& grid) {
        int rows = static_cast<int>(grid.size());
        int cols = static_cast<int>(grid[0].size());
        int result = 0;

        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                if (grid[row][col] == '1') {
                    ++result;
                    bfs(grid, row, col);
                }
            }
        }

        return result;
    }
};
```

## 为什么发现一个 `'1'` 就能让答案加一

此前已经发现的岛屿都会被 BFS 完整地改成 `'0'`。因此外层扫描再次遇到 `'1'` 时，它不可能属于之前处理过的岛屿，只能是一座新岛的起点。

BFS 会继续处理该起点上下左右所有连通的陆地，所以一座岛只会让答案增加一次。

## 本轮问题一：行列边界写混导致越界

最初检查右侧格子时写成了：

```cpp
if (currentRow + 1 < cols && grid[currentRow][currentCol + 1] == '1')
```

实际移动的是列坐标 `currentCol`，必须检查：

```cpp
currentCol + 1 < cols
```

四个方向的边界规则是：

- 上下移动改变行坐标，与 `rows` 比较。
- 左右移动改变列坐标，与 `cols` 比较。

使用错误的坐标做判断，可能允许 `currentCol + 1 == cols` 时继续访问，造成 `heap-buffer-overflow`。

## 本轮问题二：出队时标记导致重复入队

最初在节点出队后才执行：

```cpp
grid[currentRow][currentCol] = '0';
```

一个尚未出队的陆地可能被多个相邻节点重复发现，从而多次进入队列。陆地非常密集时，重复节点会大量累积并导致超时。

正确做法是在入队时立即标记：

```cpp
grid[nextRow][nextCol] = '0';
que.push({nextRow, nextCol});
```

起点也同样先标记再开始循环：

```cpp
que.push({r, c});
grid[r][c] = '0';
```

这样每个陆地最多进入队列一次。

## 队列操作复盘

```cpp
queue<pair<int, int>> que;
que.push({row, col});

auto position = que.front();
que.pop();

int row = position.first;
int col = position.second;
```

- `push()` 在队尾加入坐标。
- `front()` 读取队首，但不删除。
- `pop()` 删除队首，返回 `void`。
- 调用 `front()` 或 `pop()` 前必须确认 `!que.empty()`。
- 网格 BFS 应在节点入队时标记访问，避免重复入队。

也可以使用结构化绑定：

```cpp
auto [row, col] = que.front();
que.pop();
```

## 复杂度

- 时间复杂度：`O(rows * cols)`，每个格子最多扫描一次，每个陆地最多入队一次。
- 队列空间复杂度：最坏 `O(rows * cols)`。
- 没有额外的 `visited` 数组，但算法会直接修改输入网格。

## 与第一次练习的联系

本题第一次记录在 `notes/2026-08-12/0200-number-of-islands.md`。本次二刷继续使用 BFS，但进一步巩固了行列边界、`queue<pair<int, int>>` 操作，以及“入队即标记”这一保证线性复杂度的关键规则。

## 最终复盘

用户最终版本采用外层扫描配合原地沉岛 BFS，修正了右边界行列混淆和出队时才标记造成的重复入队问题，LeetCode 提交通过。
