# 34. 在排序数组中查找元素的第一个和最后一个位置

状态：2026-09-22 用户确认 LeetCode 提交通过，代码复核正确。

## 核心方法

两次闭区间二分，找到相等元素后先保存候选答案，再向目标边界继续查找。

- 找最左位置：first=mid，然后 right=mid-1。
- 找最右位置：last=mid，然后 left=mid+1。
- 比较结果不相等时，与普通二分查找的方向一致。

候选位置已经保存，因此将 mid 从搜索范围中排除不会丢失答案。如果更靠边的位置仍有目标值，就用新位置更新候选；否则保留当前候选。

## 用户通过的代码

```cpp
class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        int n = (int)nums.size();
        int first = -1;
        int last = -1;
        int left = 0;
        int right = n - 1;

        // 找最左边的位置
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] == target) {
                first = mid;
                right = mid - 1;
            } else if (nums[mid] > target) {
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }

        left = 0;
        right = n - 1;
        // 找最右边的位置
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] == target) {
                last = mid;
                left = mid + 1;
            } else if (nums[mid] > target) {
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
        return {first, last};
    }
};
```

## 与 35、704 的区别

| 目的 | 相等时 | 没找到时 |
|---|---|---|
| 704 普通查找 | 返回 mid | 返回 -1 |
| 35 搜索插入位置（无重复） | 返回 mid | 返回 left |
| 34 最左位置 | 记录 mid，继续向左 | 候选保持 -1 |
| 34 最右位置 | 记录 mid，继续向右 | 候选保持 -1 |

## 复习检查点

- 第二次二分前必须重新初始化 left、right，用户代码已正确处理。
- 闭区间只有一个候选位置时也要检查，因此 while 条件用 <=。
- n 先转为 int，空数组时 right=n-1 得到 -1，两次循环都跳过，返回 {-1,-1}。
- 不存在目标值时，两次循环都不更新候选，自然返回 {-1,-1}。
- 可选优化：第一次循环后若 first==-1，可直接返回，省略第二次查找；当前代码不加也正确。
- 不要在命中后线性向两边扫描，否则全数组相同的情况下会变成 O(n)。

## 边界复习

| 输入 | 输出 |
|---|---|
| [5,7,7,8,8,10], target=8 | [3,4] |
| [5,7,7,8,8,10], target=6 | [-1,-1] |
| [], target=0 | [-1,-1] |
| [8], target=8 | [0,0] |
| [8], target=7 | [-1,-1] |
| [8,8,8,8], target=8 | [0,3] |

两次二分总时间仍为 O(log n)，额外空间 O(1)。本轮验证为代码审阅及用户报告的 LeetCode 通过；表格用于复习，未另建本地测试框架。

记忆：相等先记录；找左缩右界，找右缩左界。
