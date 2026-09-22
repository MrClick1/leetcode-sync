# 35. 搜索插入位置

状态：2026-09-22 用户确认 LeetCode 全部通过，代码复核正确。

## 用户解法：闭区间二分

```cpp
class Solution {
public:
    int searchInsert(vector<int>& nums, int target) {
        int n = (int)nums.size();
        int left = 0;
        int right = n - 1;

        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] == target) {
                return mid;
            } else if (nums[mid] > target) {
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
        return left;
    }
};
```

## 边界为什么这样写

- 搜索区间为 [left,right]，初始 [0,n-1]。
- left == right 时还有一个候选元素，因此循环条件是 <=。
- 比较过 mid 且不相等后，mid 不可能是目标，更新为 mid-1 或 mid+1。
- mid = left + (right-left)/2 避免直接相加左右边界可能产生的溢出。

## 为什么没找到时返回 left

未提前返回时，始终保持：下标小于 left 的元素都小于 target，下标大于 right 的元素都大于 target。

如果 nums[mid] < target，则从 left 到 mid 都小于 target，可以令 left=mid+1；反之，从 mid 到 right 都大于 target，可以令 right=mid-1。

循环结束时 left=right+1，待搜索区间为空，数组被分成：

```text
[0, left)    所有元素 < target
[left, n)    所有元素 > target
```

把 target 插入 left，恰好维持升序。因此 left 可以是 0，也可以是 n；返回 n 表示插入末尾，不是访问 nums[n]。

例如 nums=[1,3,5,6]、target=2：

| left | right | mid | 比较与更新 |
|---|---|---|---|
| 0 | 3 | 1 | 3 > 2，right=0 |
| 0 | 0 | 0 | 1 < 2，left=1 |

退出时 left=1、right=0，返回 1。

## 边界与复杂度

| 输入 | 返回 | 含义 |
|---|---|---|
| [1,3,5,6], target=5 | 2 | 直接命中 |
| [1,3,5,6], target=2 | 1 | 中间插入 |
| [1,3,5,6], target=0 | 0 | 最前插入 |
| [1,3,5,6], target=7 | 4 | 末尾插入 |
| [1], target=1 | 0 | 单元素命中 |
| [1], target=2 | 1 | 单元素末尾插入 |

每轮搜索范围约减半，时间 O(log n)，额外空间 O(1)，不修改数组。以上用例用于复习，本轮验证为代码审阅与用户报告的 LeetCode 通过，未另建本地测试框架。

题目保证无重复元素，因此相等时直接返回 mid 即可；若改成允许重复且要求第一个大于等于 target 的位置，相等时不能直接返回任意 mid，需要继续寻找左边界。

## 与 704 的联系

704 未找到时返回 -1；本题未找到时返回 left。闭区间二分的边界更新规则一致，区别在于未找到目标后如何解释最后的边界。
