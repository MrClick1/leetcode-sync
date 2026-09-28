# 912. 排序数组：随机基准快速排序

状态：2026-09-28 用户确认最终版本 LeetCode 提交通过，代码审阅正确。本轮未重新运行本地测试；大量相等元素仍可使此二路划分退化。本题作为第 347 题快速选择的前置练习。

## 最终代码

保留用户的单向扫描、while 循环和随机基准写法，仅删除 quicksort 内未使用的 n，并明确下标注释。

```cpp
class Solution {
public:
    int partition(vector<int>& nums, int left, int right) {
        int pivotIdx = rand() % (right - left + 1) + left;
        int pivot = nums[pivotIdx];
        swap(nums[right], nums[pivotIdx]);

        int i = left - 1; // 已收集的“小于 pivot 区域”的末尾下标
        int j = left;    // 当前子区间中下一个待检查的下标
        while (j < right) {
            if (nums[j] < pivot) {
                ++i;
                swap(nums[i], nums[j]);
            }
            ++j;
        }

        ++i;
        swap(nums[i], nums[right]);
        return i;
    }

    void quicksort(vector<int>& nums, int left, int right) {
        if (left >= right) return;
        int pos = partition(nums, left, right);
        quicksort(nums, left, pos - 1);
        quicksort(nums, pos + 1, right);
    }

    vector<int> sortArray(vector<int>& nums) {
        int n = (int)nums.size();
        quicksort(nums, 0, n - 1);
        return nums;
    }
};
```

## partition 的职责与不变量

只处理闭区间 [left,right]；把基准放到最终位置 pos，左侧元素都小于基准，右侧元素都大于等于基准。返回的 pos 是基准位置，不是另一种分区算法中的分割线。

基准先存入局部变量，并将对应数组元素放到 right。扫描过程中：

| 范围 | 含义 |
|---|---|
| [left,i] | 已收集的小于 pivot 的元素 |
| [i+1,j-1] | 已检查的大于等于 pivot 的元素 |
| [j,right-1] | 尚未检查的元素 |
| right | 暂存的基准元素 |

开始时小元素区域为空，所以 i=left-1；第一个待检查位置为 j=left。left-1 只是边界标记，先 ++i 再访问 nums[i]，不会访问负下标。

若 nums[j] < pivot，先扩大小元素区域，再交换到区域末尾。扫描结束后，i+1 就是基准应放的位置。基准归位后，左右区间还不一定内部有序，递归负责继续处理。

## [4,2,5,1,3] 的首轮模拟

假设选中的基准为末尾 3：

| 步骤 | 数组 | i |
|---|---|---|
| 初始 | [4,2,5,1,3] | -1 |
| 检查 4，不小于 3 | [4,2,5,1,3] | -1 |
| 检查 2，扩大区域并交换 | [2,4,5,1,3] | 0 |
| 检查 5，不小于 3 | [2,4,5,1,3] | 0 |
| 检查 1，扩大区域并交换 | [2,1,5,4,3] | 1 |
| 基准与下标 i+1=2 交换 | [2,1,3,4,5] | 返回 2 |

下一步仅递归 [0,1] 与 [3,4]，不再包含已经就位的下标 2。

## 本轮错误与原因

1. `rand() % (right-left)` 在左右相等时取模 0；闭区间长度应为 right-left+1，并在调用 partition 前判断 left>=right。
2. 递归范围重新使用 0、n-1：破坏“只负责当前子区间”的约定，可能扩大区间、循环递归并触发 stack-overflow。错误栈显示 rand，并不意味着 rand 是根因，需查看连续重复的 quicksort 调用。
3. 左递归包含 pos：当 pos==right 时子区间不缩小。当前返回基准位置的算法必须使用 [left,pos-1]、[pos+1,right]。
4. 换成单向扫描后仍固定 i=-1、j=0：只有 left==0 时才符合含义。递归到右侧区间后会重新扫描前缀，甚至返回小于 left 的 pos。必须改为 i=left-1、j=left。
5. 例如 [1,1,1]，错误初始化会让 partition(1,2) 返回 0，右递归再次调用 quicksort(1,2)，无限重复。
6. 早期双向交换写法中的 swap(nums[left],pivot) 是与局部变量交换，且当时基准已在相遇处，是多余操作；当前单向扫描末尾的 swap(nums[i],nums[right]) 则负责让末尾基准归位，不能删除。

## 和官解的关系

官解把“随机选择基准并放到末尾”和“单向扫描划分”拆成两个函数。用户版本将它们合在 partition 中，算法相同。

官解使用 for(j=left; j<right; ++j)，用户使用 while，效果相同。官解最后交换 i+1 并返回 i+1，用户先 ++i 再交换并返回 i，也等价。

官解的 srand(time(NULL)) 用于设置伪随机种子，不影响排序逻辑正确性，也不能解决相等元素的退化。rand 取模的随机性与范围受实现影响，不应把它视为严格均匀的通用随机采样器。

## 复杂度与局限

一次长度为 s 的 partition 是 O(s)，额外工作空间 O(1)。分区较均衡时，共 O(log n) 层，每层总工作量 O(n)，所以总时间 O(n log n)，递归栈空间 O(log n)。通常的随机快排期望分析以适当的随机选择与键分布/重复值处理为前提；本版本不能无条件保证对任意重复值输入都期望 O(n log n)。

分区极不均衡时，处理规模依次为 n、n-1、n-2……，总时间 O(n²)，递归栈深度 O(n)。全相等数组会使严格 < 条件始终为假，每次 pos=left；无论随机选哪个基准都是同一个值，仍然退化。

因此提交通过与最坏复杂度保证是两件事。本题提出的 O(n log n) 目标，并不能由这份随机二路快排提供最坏情况保证。后续可学三路划分改善重复值情况；要求最坏 O(n log n) 时可用归并、堆排序或内省排序。

## 复写顺序

1. 明确当前负责 [left,right]，先写 left>=right 的终止条件。
2. 随机基准放末尾，初始化 i=left-1、j=left。
3. 扫描其余元素，小于基准的移入前面的区域。
4. 基准归位，返回位置 pos。
5. 递归两侧且排除 pos。

第 347 题的快速选择只递归需要继续寻找的一侧；快排要递归两侧。用户决定先练熟快排，再继续快速选择，不自动推进下一种算法。
