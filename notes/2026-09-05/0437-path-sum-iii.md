# 437. 路径总和 III

状态：✅ 已解决（2026-09-05，前缀和优化版通过 LeetCode）

## 题目要求

统计二叉树中节点值之和等于 `targetSum` 的非空路径数量。

- 路径可以从任意节点开始，在任意节点结束。
- 路径必须沿父节点到子节点的方向向下。
- 返回符合条件的路径数量。
- 节点数 `[0, 1000]`，节点值 `[-10^9, 10^9]`，目标值 `[-1000, 1000]`。

## 示例

- `root = [10,5,-3,3,2,null,11,3,-2,null,1]`，`targetSum = 8`，答案 `3`。
- `root = [5,4,8,11,null,13,4,7,2,null,null,5,1]`，`targetSum = 22`，答案 `3`。

## 当前进度

- 用户已参考前缀和解法写出优化版本，并确认 LeetCode 提交通过。
- 在家使用 LeetCode 网页端练习，通过对话记录思路和报错。
- 已完成两层递归基础解法与前缀和优化解法的对比、易错点和复杂度记录。

## 第一次讲解：枚举起点，向下累加

把问题拆成两个函数，避免把“选择起点”和“延长同一条路径”混在一起：

- `countFrom(node, remaining)`：只统计必须从 `node` 开始、向下的路径，路径所需剩余和为 `remaining`。
- `pathSum(root, targetSum)`：统计整棵树所有合法路径；分别计算从根开始、起点在左子树、起点在右子树的路径数量。

```cpp
int countFrom(TreeNode* node, long long remaining) {
    if (node == nullptr) return 0;

    int count = (node->val == remaining) ? 1 : 0;
    long long nextRemaining = remaining - node->val;
    count += countFrom(node->left, nextRemaining);
    count += countFrom(node->right, nextRemaining);
    return count;
}
```

外层递推关系：

```cpp
// root 为空时直接返回 0
countFrom(root, targetSum)
    + pathSum(root->left, targetSum)
    + pathSum(root->right, targetSum)
```

每条非空路径有唯一的起点和终点，所以枚举起点不会重复计数。固定起点后，每次向下递归到一个节点，就检查以该节点作为终点是否符合要求。

官方第一例的三条路径为 `5 -> 3`、`5 -> 2 -> 1`、`-3 -> 11`，路径可以共享节点，也可以不从根开始。

## 用户参考的双重递归代码

`rootSum` 就是前文的 `countFrom`，名字不同，统计范围相同。

```cpp
class Solution {
public:
    int rootSum(TreeNode* root, long long targetSum) {
        if (root == nullptr) return 0;

        int ret = 0;
        if (root->val == targetSum) ret++;

        ret += rootSum(root->left, targetSum - root->val);
        ret += rootSum(root->right, targetSum - root->val);
        return ret;
    }

    int pathSum(TreeNode* root, int targetSum) {
        if (!root) return 0;

        int ret = rootSum(root, targetSum);
        ret += pathSum(root->left, targetSum);
        ret += pathSum(root->right, targetSum);
        return ret;
    }
};
```

### 两层递归为何都访问左右孩子

- `rootSum(root, targetSum)`：本次函数调用只统计必须从当前 `root` 开始的非空向下路径。在整次搜索中，往孩子递归是在延长原起点的路径，因此减去已经选择的当前节点值。
- `pathSum(root, targetSum)`：统计这棵子树内所有起点的路径。递归到孩子时是在枚举新的起点，因此目标保持不变。

例如 `5 -> 2 -> 1`，目标 3：`rootSum(5, 3)` 为 0；但 `pathSum(5, 3)` 为 1，因为它还会枚举起点 2，找到路径 `2 -> 1`。这说明两个函数不能互相替换。

空节点返回 0 表示没有非空路径，并非检查空路径之和。`root->val == targetSum` 只表示当前节点可以作为该搜索路径的终点，因此计数加一后仍要继续搜索其他终点。

复习自测：解释为什么 `rootSum` 要减去当前值，而 `pathSum` 不减；解释匹配后为何不能立即返回；解释路径共享节点为何不等于重复计数。

## 关键边界

- 达到目标后仍要继续向下搜索，后续节点可能包含 0，或者一段正负数之和为 0。例如 `1 -> 0`、目标 1，有两条以 1 为起点的合法路径。
- 节点可能为负数，不能因当前和超过目标就剪枝。例如 `10 -> -2`、目标 8，整段仍是有效路径。
- 累计和或剩余和使用 `long long`：最多 1000 个节点，每个节点绝对值可达 `10^9`，中间数值可能超过 `int`。
- 外层选择新起点时传入原始目标；内层延长路径时传入减去当前节点值后的剩余目标。

## 复杂度与后续方向

基础版本最坏时间 `O(n^2)`：退化成长链时，从各起点分别搜索约 `n, n-1, ..., 1` 个节点。递归栈空间 `O(h)`，最坏为 `O(n)`。

后续可以用“当前祖先路径上的前缀和 + 哈希表计数”优化到平均 `O(n)` 时间；先掌握两层递归的职责，再学习该优化。

## 优化：前缀和 + 哈希表 + 回溯

基础方法枚举起点，重复访问后续节点。优化方法改为只遍历一次树：到达节点时，以它为终点，直接查询有多少个合法起点。

令 `sum` 是根到当前节点的节点值之和，`prefix` 是某个祖先位置的前缀和，那么该祖先之后到当前节点的路径和为 `sum - prefix`。要等于 `targetSum`，就查询 `prefix == sum - targetSum` 的数量。

`freq` 保存当前祖先路径上每种前缀和出现的次数，不是整棵树的全局历史。初始化 `freq[0] = 1` 代表根之前的空前缀，使从根开始的路径也能被统计。查询必须早于当前前缀的插入，防止目标为 0 时误计空路径。

```cpp
class Solution {
public:
    int dfs(TreeNode* node, long long sum, int targetSum,
            unordered_map<long long, int>& freq) {
        if (node == nullptr) return 0;

        sum += node->val;
        int count = 0;
        auto it = freq.find(sum - targetSum);
        if (it != freq.end()) {
            count = it->second;
        }

        freq[sum]++;
        count += dfs(node->left, sum, targetSum, freq);
        count += dfs(node->right, sum, targetSum, freq);
        if (--freq[sum] == 0) {
            freq.erase(sum);
        }
        return count;
    }

    int pathSum(TreeNode* root, int targetSum) {
        unordered_map<long long, int> freq;
        freq[0] = 1;
        return dfs(root, 0LL, targetSum, freq);
    }
};
```

执行顺序：累加当前值 → 查询 `sum - targetSum` → 加入当前前缀 → 递归左右子树 → 撤销当前前缀。

例如根到当前路径为 `10 -> 5 -> 2 -> 1`，当前和为 18、目标为 8。查询前缀和 10，去掉该前缀后得到路径 `5 -> 2 -> 1`。

前缀和可能重复，因此必须保存次数。例如 `0 -> 0`、目标为 0：处理第一个节点时贡献 1，第二个节点时贡献 2，总计 3 条非空路径。

回溯必要性：对于根 1、左孩子 2、右孩子 3，目标为 1，答案只有根节点本身。若左子树的前缀和 3 未撤销，处理右孩子时当前和 4，会错误查询到 `4 - 1 == 3`，把跨分支、实际不存在的向下路径计入答案。

空间与时间：每个节点只遍历一次，哈希表操作平均常数时间，总时间平均 `O(n)`。递归栈和有效前缀表均为 `O(h)`（上面删除计数归零的 key），最坏 `O(n)`。累计和与哈希表 key 必须为 `long long`；数量使用 `int` 足够本题约束。

用户已将优化版本提交到 LeetCode 并通过全部测试。

## 用户前缀和版本 review

用户实现与标准前缀和解法一致：

- `freq[0] = 1` 正确统计从根节点开始的路径
- 先查询 `sum - targetSum`，再执行 `freq[sum]++`，不会把空路径计入结果
- 左右子树共享当前祖先前缀记录；返回当前节点时用 `--freq[sum]` 回溯，避免跨分支污染
- `sum` 和 `freq` 的 key 使用 `long long`，能覆盖节点值累加的范围；`0LL` 明确传入长整型初始和
- `int cnt` 足以保存本题最多 1000 个节点产生的路径数量

该版本已通过 LeetCode，相关验证结果已记录在“最终复盘”中。

## 最终复盘

- 基础双重递归通过枚举每个节点作为起点实现，最坏时间 `O(n²)`，递归栈 `O(h)`。
- 最终采用前缀和哈希表：对当前前缀和 `sum` 查询 `sum - targetSum` 的出现次数，一次遍历完成统计。
- `freq[0] = 1` 统计从根开始的路径；查询在插入当前前缀之前完成，避免目标为 0 时把空路径计入。
- 递归返回时撤销当前前缀和，防止不同分支之间互相污染。
- 最终优化版本时间平均 `O(n)`，递归栈和当前路径哈希表空间 `O(h)`，最坏 `O(n)`。
- 用户确认最终优化版本在 LeetCode 通过。
