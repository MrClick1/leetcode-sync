# 56. 合并区间

状态：用户确认 LeetCode 提交通过（2026-09-22），代码复核正确。

## 思路与不变量

先按照起点升序排序，起点相同时按终点升序。依次处理时，result 始终是已经处理过的区间合并后的结果，内部有序且互不重叠。

新来的区间只需要和 result 的最后一个区间比较：更早的结果区间已经与最后一个区间分开，而新起点不会更小，因此不可能再与那些更早的区间重叠。

- 当前起点 <= 最后一个区间的终点：重叠，将终点更新为两者的最大值。
- 当前起点 > 最后一个区间的终点：存在间隔，加入一个新区间。

端点相等也要合并，例如 `[1,4]` 与 `[4,5]` 得到 `[1,5]`。

## 用户通过的代码

```cpp
class Solution {
public:
    vector<vector<int>> merge(vector<vector<int>>& intervals) {
        int n = (int)intervals.size();
        vector<vector<int>> result;

        sort(intervals.begin(), intervals.end(),
             [](const vector<int>& a, const vector<int>& b) {
                 if (a[0] != b[0]) {
                     return a[0] < b[0];
                 }
                 return a[1] < b[1];
             });

        result.push_back(intervals[0]);

        for (int i = 1; i < n; ++i) {
            const vector<int>& interval = intervals[i];
            vector<int>& endRes = result[(int)result.size() - 1];
            if (interval[0] <= endRes[1]) {
                endRes[1] = max(endRes[1], interval[1]);
            } else {
                result.push_back(interval);
            }
        }
        return result;
    }
};
```

题目保证至少一个区间，因此访问 intervals[0] 合法。

## 本轮错误复盘

1. 比较器误写 `a[0] != a[1]`，应比较两个区间的起点 `a[0] != b[0]`。错误比较器可能破坏排序所要求的严格弱序。
2. 重叠条件误写 `interval[0] >= endRes[1]`，应是 `<=`。例如 `[1,3]` 和 `[2,6]` 重叠，而 `[1,3]` 和 `[8,10]` 不重叠。
3. 合并终点不能直接赋成 interval[1]：`[1,10]` 包含 `[2,3]`，直接赋值会丢失 `[3,10]`。必须使用 max。

## 示例与边界复习

| 输入 | 结果 | 关注点 |
|---|---|---|
| `[[1,3],[2,6],[8,10],[15,18]]` | `[[1,6],[8,10],[15,18]]` | 重叠与分离 |
| `[[1,4],[4,5]]` | `[[1,5]]` | 端点相等 |
| `[[4,7],[1,4]]` | `[[1,7]]` | 先排序 |
| `[[1,10],[2,3]]` | `[[1,10]]` | 包含时不能缩短终点 |
| `[[1,2],[2,3],[3,4]]` | `[[1,4]]` | 连续合并 |
| `[[1,1],[1,1]]` | `[[1,1]]` | 单点与重复区间 |
| `[[0,0]]` | `[[0,0]]` | 单个区间 |

以上用于手动复习；本轮验证为代码审阅及用户报告的 LeetCode 通过，未另建本地测试框架。

## 语法与可选简化

当前比较器与 vector 的默认字典序相同，可以简写为 `sort(intervals.begin(), intervals.end());`。

`vector<int>& endRes = result.back();` 可以替代通过 size 取最后一个元素。这里必须保留 `&`，修改 endRes 才会修改 result 中的区间。

`push_back` 可能导致 result 扩容，使此前取得的 endRes 引用失效。当前代码在 push_back 之后不再使用这个引用，下一轮重新取得引用，因此没有问题。

Lambda 和默认字典序的详细语法见 [C++ 语法笔记](../01-cpp语法知识文档.md)。

## 复杂度与记忆

排序 O(n log n)，扫描 O(n)，总时间 O(n log n)。结果空间 O(n)；常见 std::sort 实现还使用 O(log n) 栈空间，扫描本身辅助空间 O(1)。排序会改变输入区间的排列顺序。

记忆：先按起点排序，只看最后一段；接得上就延长终点，接不上就另起一段。
