# 1. 两数之和

状态：✅ 已解决（2026-09-06，哈希表解法 review 通过）

## 题目要求

给定整数数组 `nums` 和目标值 `target`，返回数组中和为 `target` 的两个元素下标。每个元素只能使用一次，题目保证恰好存在一个答案。

## 解法：一次遍历 + 哈希表

从左到右遍历数组。处理当前值 `num = nums[i]` 时，所需的另一个值是：

```text
complement = target - num
```

哈希表保存已经遍历过的“值 → 下标”。如果补数已经在表中，说明当前值与之前的元素组成答案；否则保存当前值及其下标，供后面的元素查找。

```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int, int> index;

        for (int i = 0; i < static_cast<int>(nums.size()); i++) {
            int num = nums[i];
            auto it = index.find(target - num);

            if (it != index.end()) {
                return {i, it->second};
            }

            index.insert({num, i});
        }

        return {};
    }
};
```

## 为什么要先查找再插入

当前元素还没有被加入哈希表，因此查找补数时不会把当前元素和自己配对。例如 `nums = [3]`、`target = 6`，不会错误地使用同一个 `3` 两次。

对于 `nums = [3, 3]`、`target = 6`：

1. 第一个 `3` 没有找到补数，保存 `3 → 0`。
2. 第二个 `3` 找到 `3 → 0`，返回 `{1, 0}`。

## 正确性要点

- 哈希表中的元素都位于当前下标 `i` 之前，因此下标一定不同。
- 若答案中有当前元素，则它的另一个值已经被遍历并保存，当前轮能够找到。
- 若当前元素不是答案的一部分，保存它不会影响后续查找。
- 题目保证存在唯一答案，因此找到后可以立即返回。

## 复杂度

- 平均时间复杂度：`O(n)`，每个元素进行一次哈希查找和一次插入。
- 额外空间复杂度：`O(n)`。
- `unordered_map` 理论最坏查找可能退化为 `O(n)`，但通常按平均 `O(1)` 分析。

## C++ 易错点

- `find` 找不到元素时返回 `end()`，应先判断 `it != index.end()`，再访问 `it->second`。
- `index.insert({num, i})` 遇到重复 key 时不会覆盖旧下标；保留第一次出现的位置足以解决本题。
- 使用 `index[num]` 读取不存在的 key 会自动插入默认值，查找时优先使用 `find`。
- 返回下标的顺序不影响答案，`{i, it->second}` 与 `{it->second, i}` 都可以。
- `static_cast<int>(nums.size())` 将无符号的 `size_t` 明确转换为 `int`，用于和循环下标比较。

## 本轮复盘

用户实现与标准一次遍历哈希表解法一致：先用 `find(target - num)` 查补数，未找到时用 `insert` 保存当前值和下标；重复值、禁止重复使用同一元素和唯一答案等边界均处理正确。
