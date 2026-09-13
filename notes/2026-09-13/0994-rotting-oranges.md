# 994. 腐烂的橘子（二刷）

状态：✅ 已解决（2026-09-13，多源 BFS 通过 LeetCode）

## 一句话认识这道题

这不是“从某个腐烂橘子走到所有位置”，而是“所有初始腐烂橘子同时向外扩散”。因此要把全部初始腐烂橘子放进同一个队列，执行一次多源 BFS。

## 用户通过版本的核心

用户的最终版本完成了以下步骤：

1. 扫描网格，统计新鲜橘子数量 `cnt`。
2. 没有新鲜橘子时直接返回 `0`。
3. 再次扫描，把所有初始腐烂橘子放入同一个队列。
4. 每轮先保存 `que.size()`，处理这一层的全部橘子。
5. 新鲜橘子变腐烂时立即改成 `2`、入队并执行 `cnt--`。
6. BFS 结束后仍有新鲜橘子则返回 `-1`。

这已经是正确的多源 BFS。LeetCode 提交通过。

## 用户版本与官方版本的共同点

- 都把所有初始腐烂橘子同时放入队列。
- 都只进行一次 BFS，而不是为每个腐烂橘子分别搜索。
- 都在新鲜橘子第一次被发现时标记，防止重复入队。
- 都使用新鲜橘子计数判断是否存在永远无法腐烂的橘子。
- 时间复杂度都是 `O(rows * cols)`。

所以两份代码的算法本质相同，区别主要是“如何标记访问”和“如何计算时间”。

## 两份实现的区别

| 对比项 | 用户版本 | 官方版本 |
|---|---|---|
| 队列位置 | 类成员变量 `que` | 函数内局部变量 `Q` |
| 初始扫描 | 分两次统计新鲜、加入腐烂源 | 一次扫描同时完成 |
| 访问标记 | 直接把 `grid` 中的 `1` 改成 `2` | 使用 `dis` 数组，`-1` 表示未访问 |
| 时间计算 | 按 BFS 层计数 `dur` | 给每个格子记录距离 `dis` |
| 四个方向 | 四段独立的 `if` | 方向数组配合循环 |
| 最终时间 | 队列完全清空后返回 `dur - 1` | 最后腐烂橘子的距离作为 `ans` |
| 是否修改输入 | 会修改 `grid` | 不修改 `grid` |
| 额外空间 | BFS 队列 | BFS 队列 + 距离数组 |

### 队列更适合放在函数内部

用户版本把队列声明成类成员：

```cpp
queue<pair<int, int>> que;
```

本题只调用一次且 BFS 最终会清空队列，所以能够通过。但局部变量更容易保证每次调用都是干净状态，也能减少函数之间的隐藏依赖。

### 原地改值与 `dis` 数组

用户版本：

```cpp
grid[nextRow][nextCol] = 2;
que.push({nextRow, nextCol});
```

这里的 `2` 同时表示“已经腐烂”和“已经访问”。代码简单，并省去距离数组，但会修改输入网格。

官方版本不修改网格，而是使用：

```cpp
dis[nextRow][nextCol] = dis[row][col] + 1;
```

`dis == -1` 表示尚未访问，非负数表示这个位置腐烂所需的分钟数。

### 按层计时与距离计时

用户版本每轮保存当前队列长度：

```cpp
int levelSize = static_cast<int>(que.size());
```

这一批节点属于同一分钟。处理完这一层后执行 `dur++`。

官方版本不用保存层大小，而是让新节点的时间等于父节点时间加一：

```cpp
dis[nextRow][nextCol] = dis[row][col] + 1;
```

BFS 按距离从小到大出队，因此最后被腐烂的新鲜橘子距离就是最终答案。

两种计时方法都是正确的：

- 只需要最终分钟数时，按层计时更直观。
- 需要每个位置的具体腐烂时间时，距离数组更合适。

## 为什么用户版本需要 `dur - 1`

用户版本使用：

```cpp
while (!que.empty()) {
    // 处理完整一层
    dur++;
}
```

最后一批刚腐烂的橘子也会进入队列。下一轮把它们弹出时，已经没有新的橘子可以感染，但 `dur` 仍然增加了一次，所以最后用 `dur - 1` 抵消这次空扩散。

这种写法可以通过，但要配合“没有新鲜橘子直接返回 0”，否则 `[[0]]` 会得到 `-1`。

更容易理解的做法是只在仍有新鲜橘子时继续扩散：

```cpp
while (!que.empty() && fresh > 0) {
    // 处理一层
    ++minutes;
}
```

这样每轮确实让至少一批新鲜橘子进入下一分钟，最后直接返回 `minutes`，不需要减一。

## 官方代码中的几个语法点

### `Q.emplace(i, j)`

队列元素类型是 `pair<int, int>`：

```cpp
queue<pair<int, int>> Q;
```

下面两种写法都可以：

```cpp
Q.push({i, j});
Q.emplace(i, j);
```

`push` 先构造一个 `pair` 再加入队列；`emplace` 使用参数直接构造队列元素。

### `~dis[tx][ty]`

官方写法：

```cpp
if (... || ~dis[tx][ty] || ...) continue;
```

`~` 是按位取反。`dis` 初始化为 `-1`：

- `dis == -1` 时，`~(-1) == 0`，条件为假，说明尚未访问。
- `dis >= 0` 时，按位取反通常为非零，条件为真，说明已经访问，需要跳过。

这是一种竞赛式简写，但可读性较弱。复习时建议写成：

```cpp
if (dis[tx][ty] != -1) continue;
```

### `!grid[tx][ty]`

本题中网格值只有 `0、1、2`，所以：

```cpp
!grid[tx][ty]
```

等价于：

```cpp
grid[tx][ty] == 0
```

也就是跳过空格子。明确比较通常更容易阅读。

### `if (!cnt) break`

官方代码中的 `break` 只会结束当前的四方向 `for` 循环，不会直接结束外层 `while`。此时所有新鲜橘子已经腐烂，后续处理不会再改变 `ans`，所以结果仍然正确；若想真正提前结束，也可以在 `cnt == 0` 时直接返回 `ans`。

官方的 `dis[10][10]` 使用固定大小，是因为题目保证行列都不超过 10。更通用的写法可以根据 `rows` 和 `cols` 创建二维 `vector`。

## 最推荐记忆的模板

```cpp
class Solution {
public:
    int orangesRotting(vector<vector<int>>& grid) {
        int rows = static_cast<int>(grid.size());
        int cols = static_cast<int>(grid[0].size());

        queue<pair<int, int>> que;
        int fresh = 0;

        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                if (grid[row][col] == 2) {
                    que.push({row, col});
                } else if (grid[row][col] == 1) {
                    ++fresh;
                }
            }
        }

        int directions[4][2] = {
            {-1, 0}, {1, 0}, {0, -1}, {0, 1}
        };

        int minutes = 0;

        while (!que.empty() && fresh > 0) {
            int levelSize = static_cast<int>(que.size());

            for (int i = 0; i < levelSize; ++i) {
                auto [row, col] = que.front();
                que.pop();

                for (const auto& direction : directions) {
                    int nextRow = row + direction[0];
                    int nextCol = col + direction[1];

                    if (nextRow < 0 || nextRow >= rows ||
                        nextCol < 0 || nextCol >= cols ||
                        grid[nextRow][nextCol] != 1) {
                        continue;
                    }

                    grid[nextRow][nextCol] = 2;
                    --fresh;
                    que.push({nextRow, nextCol});
                }
            }

            ++minutes;
        }

        return fresh == 0 ? minutes : -1;
    }
};
```

## 记忆口诀

```text
烂橘全部先入队，
新鲜橘子单独数；
队列一层一分钟，
入队立刻标腐烂；
最后还有新鲜橘，
说明隔绝返回负一。
```

也可以压缩成五个动作：

```text
收集源点 → 统计目标 → 分层扩散 → 入队标记 → 检查剩余
```

## 本轮错误复盘

1. 最初忘记 `que.pop()`，队首无法移除，循环超时。
2. 最初从新鲜橘子分别 BFS，忽略了所有腐烂源同时扩散。
3. 最初按出队节点数量增加时间，而不是按 BFS 层增加时间。
4. `[[0]]` 中没有新鲜橘子也没有腐烂橘子，`res` 保持 `-1`；应直接返回 `0`。
5. 修改为多源 BFS 后忘记接收 `bfs` 返回值，导致 `res` 仍为 `-1`。
6. 最终版本完成多源入队、分层计时、入队标记和剩余新鲜橘子检查，LeetCode 提交通过。

## 复杂度

- 时间复杂度：`O(rows * cols)`，每个格子最多扫描和入队一次。
- 队列空间复杂度：最坏 `O(rows * cols)`。
- 用户的原地标记版本不需要额外距离数组；官方版本另有 `O(rows * cols)` 的 `dis` 数组。
