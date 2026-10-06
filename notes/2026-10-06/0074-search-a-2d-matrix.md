# 74. 搜索二维矩阵

状态：2026-10-06 用户两次二分版本已力扣通过。

## 通过代码

```cpp
class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        int m = (int)matrix.size();
        int n = (int)matrix[0].size();

        // 找首元素不大于 target 的最后一行，作为候选行
        int l = 0;
        int r = m - 1;
        int line = -1;

        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (matrix[mid][0] <= target) {
                line = mid;
                l = mid + 1;
            } else {
                r = mid - 1;
            }
        }

        if (line == -1) return false;

        l = 0;
        r = n - 1;
        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (matrix[line][mid] == target) {
                return true;
            } else if (matrix[line][mid] > target) {
                r = mid - 1;
            } else {
                l = mid + 1;
            }
        }
        return false;
    }
};
```

## 为什么只需要检查候选行

第一轮找满足 `matrix[row][0] <= target` 的最大行号。满足时保存 `line=mid`，再向右搜索；`line` 保存目前找到的候选，不会因为排除 mid 而丢失。

- 候选行之前的所有元素 < 候选行首元素 <= target，因此都严格小于 target。
- 候选行之后的首元素都大于 target，每行其余元素也都大于 target。
- 若 `line == -1`，所有行首都大于 target，矩阵中不可能存在目标。

`line` 是目标可能所在的行，不能保证目标存在。示例首列 [1,10,23]、target=13 时，候选行为 1；在 [10,11,16,20] 内二分找不到，返回 false。目标落在两行之间的数值空隙时也同样返回 false。

## 与整体二分的关系

两次二分分别处理 m 行和 n 列，时间 O(log m + log n)，等价于 O(log(m*n))；最小规模只有常数次操作。空间 O(1)。

整体二分也满足相同复杂度：虚拟一维下标 k 对应行 `k/n`、列 `k%n`，无需创建扁平数组。用户当前版本已经满足要求，不必为了复杂度更换写法。

## 复盘重点

第一轮是找最后一个满足条件的位置，匹配后要保存并继续右移；第二轮是查找确切值，匹配后立即返回。两轮重新设置 l、r，并在访问候选行之前检查 line 是否为 -1。
