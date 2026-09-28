# 55. 跳跃游戏

状态：2026-09-29 用户确认 LeetCode 提交通过，代码审阅正确。本轮对照用户提供的官方贪心解法，保留通过版本，未另外创建脚手架或编译测试。

## 用户通过版本

```cpp
class Solution {
public:
    bool canJump(vector<int>& nums) {
        int n = (int)nums.size();
        int curMaxLeap = 0;
        int maxLeap = 0;

        for (int i = 0; i < n; ++i) {
            if (i <= maxLeap) {
                curMaxLeap = i + nums[i];
                maxLeap = max(maxLeap, curMaxLeap);
            }
            if (maxLeap >= n - 1) return true;
        }

        return false;
    }
};
```

## 状态含义与正确性

maxLeap 表示利用已经检查的可达位置，当前能够覆盖到的最远下标，对应官方 rightmost。curMaxLeap 表示从当前可达位置 i 出发，一次最多到达的下标 i+nums[i]；它是临时候选，而不是历史最远位置，也不是本次跳跃的长度。

开始就在下标 0，所以 maxLeap=0。题目给的是最大跳跃长度，可以选更短的步数，因此可达位置形成连续覆盖区间。每次只允许从可达下标 i 更新：i<=maxLeap 时，用 i+nums[i] 扩展覆盖范围；未到达的位置不能提供跳跃能力。

不用确定具体跳跃路径，也不是每次必须跳 nums[i] 步。只要逐个检查覆盖范围内的下标，就考虑了所有可能扩展范围的起跳点，不会遗漏答案。覆盖到 n-1 即成功。

## 与官方解法的区别

| 比较点 | 用户版本 | 官方版本 | 影响 |
|---|---|---|---|
| 历史最远下标 | maxLeap | rightmost | 只是命名不同 |
| 当前起跳点候选 | 先存 curMaxLeap=i+nums[i] | 直接使用 i+nums[i] | 临时变量可省，但不影响正确性或复杂度 |
| 成功判断的位置 | 位于可达 if 外 | 位于可达 if 内，更新后检查 | 本题中效果相同 |
| 失败时 | 扫描完整个数组后返回 false | 同样扫描完整个数组后返回 false | 都可在首次不可达时提前返回 false |

用户成功判断放在外面是安全的。如果某一轮 i>maxLeap，则 maxLeap<i<=n-1，因而 maxLeap<n-1，不可能错误返回 true。并且该轮没有更新 maxLeap，判断只是重复检查原状态；可达分支则与官方一样先更新、再判成功。

更贴近含义的命名可以是 curReach 与 farthest；原注释“最远距离”可改为“从当前下标出发能到达的最远下标”，避免把下标与跳跃步数混淆。

## 可选整理：省去临时变量，失败时提前退出

```cpp
bool canJump(vector<int>& nums) {
    int n = (int)nums.size();
    int farthest = 0;

    for (int i = 0; i < n; ++i) {
        if (i > farthest) return false;
        farthest = max(farthest, i + nums[i]);
        if (farthest >= n - 1) return true;
    }

    return false;
}
```

为什么首次 i>farthest 就可失败退出？之前所有可达位置已经检查，覆盖范围无法扩展到 i；所有更后面的下标也都不可达，无法利用它们更新覆盖。该整理不会改变最坏 O(n) 时间，但失败用例可少扫描后面的元素。

## 示例与边界复核

- [2,3,1,1,4]：下标 0 扩展到 2，下标 1 已可达，可进一步扩展到 4，返回 true。不是必须从下标 0 一次跳到 2。
- [3,2,1,0,4]：下标 0、1、2、3 提供的覆盖都不超过 3；下标 4 不可达，不能使用 nums[4]=4，返回 false。
- [0]：起点就是终点，不需要跳跃，返回 true。
- [0,1]：覆盖无法越过 0，返回 false。
- [2,0,0]：可以跨过中间的 0 到达末尾，返回 true。不能仅凭出现 0 判断失败。
- 跳跃能力超过数组末尾也算成功，使用 >=n-1 而不是 ==n-1；不需要访问覆盖范围外的数组下标。

以上为逻辑复核，不是本轮重新执行测试的记录。

## 复杂度与记忆

最多遍历 n 个元素，每轮只做常数次比较与更新，时间 O(n)。只维护几个整数，额外空间 O(1)，多一个临时变量不改变这一结论。

记忆：先确认当前位置可达，再尝试扩大最远覆盖；覆盖到终点就成功，首次断开覆盖即可失败。
