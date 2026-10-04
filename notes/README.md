# 刷题复盘笔记

每道题一个 md 文件，按练习日期归档，记录遇到的问题、解题要点和复杂度。日期按倒序排列，方便优先复习最近一天的内容。

## 长期参考

- [C++ 语法与标准库速查（个人版）](01-cpp语法知识文档.md)
- [Hot 100 题型复习总览](topics/README.md)

## 按题型复习

日期目录用于回顾“哪天学了什么”，下面的专题目录用于集中复习同类解法。每道题只归入一个主分类，同时会在相关专题中提供交叉引用。

| 专题 | 笔记 |
|---|---|
| 哈希与前缀和 | [topics/01-哈希与前缀和.md](topics/01-哈希与前缀和.md) |
| 双指针 | [topics/02-双指针.md](topics/02-双指针.md) |
| 滑动窗口与子串 | [topics/03-滑动窗口与子串.md](topics/03-滑动窗口与子串.md) |
| 普通数组与技巧 | [topics/04-普通数组与技巧.md](topics/04-普通数组与技巧.md) |
| 矩阵 | [topics/05-矩阵.md](topics/05-矩阵.md) |
| 链表 | [topics/06-链表.md](topics/06-链表.md) |
| 二叉树 | [topics/07-二叉树.md](topics/07-二叉树.md) |
| 图论与 BFS | [topics/08-图论与BFS.md](topics/08-图论与BFS.md) |
| 栈与单调栈 | [topics/09-栈与单调栈.md](topics/09-栈与单调栈.md) |
| 堆与数据流 | [topics/10-堆与数据流.md](topics/10-堆与数据流.md) |
| 前缀树 Trie | [topics/11-前缀树.md](topics/11-前缀树.md) |
| 二分查找 | [topics/12-二分查找.md](topics/12-二分查找.md) |
| 贪心 | [topics/13-贪心.md](topics/13-贪心.md) |
| 回溯 | [topics/14-回溯.md](topics/14-回溯.md) |
| 动态规划 | [topics/15-动态规划.md](topics/15-动态规划.md) |

## 2026-10-04

| 题目 | 状态 | 笔记 |
|---|---|---|
| 5. 最长回文子串 | 二维区间 DP 已力扣通过；起点倒序与官解长度升序对照、短区间及复制开销待巩固 | [2026-10-04/0005-longest-palindromic-substring.md](2026-10-04/0005-longest-palindromic-substring.md) |

## 2026-10-03

| 题目 | 状态 | 笔记 |
|---|---|---|
| 416. 分割等和子集 | 一维 0/1 背包 DP 已力扣通过；倒序保留上一轮状态、每个元素最多一次 | [2026-10-03/0416-partition-equal-subset-sum.md](2026-10-03/0416-partition-equal-subset-sum.md) |
| 32. 最长有效括号三种解法复盘 | DP、下标栈、双向计数均已力扣通过；状态与边界仍待巩固 | [2026-10-03/0032-longest-valid-parentheses.md](2026-10-03/0032-longest-valid-parentheses.md) |
| 64. 最小路径和 | 一维滚动数组已力扣通过；首行累计、上方旧状态与左方新状态 | [2026-10-03/0064-minimum-path-sum.md](2026-10-03/0064-minimum-path-sum.md) |

## 2026-10-02

| 题目 | 状态 | 笔记 |
|---|---|---|
| 322. 零钱兑换 | 用户一维 DP 力扣通过；记忆化搜索暂缓，先完成 Hot 100 DP 首轮 | [2026-10-02/0322-coin-change.md](2026-10-02/0322-coin-change.md) |
| 139. 单词拆分 | 前缀长度 DP 已力扣通过；从思路转为代码仍待巩固 | [2026-10-02/0139-word-break.md](2026-10-02/0139-word-break.md) |
| 300. 最长递增子序列 | O(n²) DP 与贪心加二分均已力扣通过；最小结尾、二分对象与等号边界待巩固 | [2026-10-02/0300-longest-increasing-subsequence.md](2026-10-02/0300-longest-increasing-subsequence.md) |
| 152. 乘积最大子数组 | 双数组 DP 已力扣通过；最大最小状态、三个候选与官解对照待复习 | [2026-10-02/0152-maximum-product-subarray.md](2026-10-02/0152-maximum-product-subarray.md) |

## 2026-09-30

| 题目 | 状态 | 笔记 |
|---|---|---|
| 70. 爬楼梯 | 数组 DP 已力扣通过；2026-10-02 标记矩阵快速幂暂时跳过、待复习 | [2026-09-30/0070-climbing-stairs.md](2026-09-30/0070-climbing-stairs.md) |
| 51. N 皇后 | 用户最终棋盘检查版力扣通过，C++17 对 n=1..9 与独立参考一致；行号、斜线与撤销逻辑待巩固 | [2026-09-30/0051-n-queens.md](2026-09-30/0051-n-queens.md) |
| 131. 分割回文串 | ✅ 已力扣通过；C++17 724 个输入复核；substr 下标/长度、回文双指针与分割起终点 | [2026-09-30/0131-palindrome-partitioning.md](2026-09-30/0131-palindrome-partitioning.md) |
| 79. 单词搜索 | ✅ path 版力扣通过；新 k 版本地 3153 个输入通过、待提交反馈；调用者标记/撤销分工待巩固 | [2026-09-30/0079-word-search.md](2026-09-30/0079-word-search.md) |
| 22. 括号生成 | ✅ 成员版已力扣通过；局部引用参数版与官解均复核 n=1..8；状态分类、选择前检查与计数传值对照 | [2026-09-30/0022-generate-parentheses.md](2026-09-30/0022-generate-parentheses.md) |

## 2026-09-29

| 题目 | 状态 | 笔记 |
|---|---|---|
| 39. 组合总和 | ✅ 已通过并复核；startIndex、重复选取及 sum 撤销区别待巩固 | [2026-09-29/0039-combination-sum.md](2026-09-29/0039-combination-sum.md) |
| 17. 电话号码的字母组合 | ✅ 用户原版与固定数字位置新版均已通过并复核；循环范围与终止条件对照待巩固 | [2026-09-29/0017-letter-combinations-of-a-phone-number.md](2026-09-29/0017-letter-combinations-of-a-phone-number.md) |
| 78. 子集 | ✅ 已通过并复核（每个节点收集，i+1 避免重复） | [2026-09-29/0078-subsets.md](2026-09-29/0078-subsets.md) |
| 46. 全排列 | 代码已复核，本地校验通过；待确认 LeetCode 提交 | [2026-09-29/0046-permutations.md](2026-09-29/0046-permutations.md) |
| 763. 划分字母区间 | ✅ 已通过，动态边界与贪心切分待巩固 | [2026-09-29/0763-partition-labels.md](2026-09-29/0763-partition-labels.md) |
| 45. 跳跃游戏 II | ✅ 已通过，分层覆盖与计数待巩固 | [2026-09-29/0045-jump-game-ii.md](2026-09-29/0045-jump-game-ii.md) |
| 55. 跳跃游戏 | ✅ 已解决（维护最远可达下标） | [2026-09-29/0055-jump-game.md](2026-09-29/0055-jump-game.md) |
| 121. 买卖股票的最佳时机 | ✅ 已解决（历史最低价 + 最大利润） | [2026-09-29/0121-best-time-to-buy-and-sell-stock.md](2026-09-29/0121-best-time-to-buy-and-sell-stock.md) |

## 2026-09-28

| 题目 | 状态 | 笔记 |
|---|---|---|
| 215. 数组中的第 K 个最大元素 | ✅ 迭代大顶堆版本通过，快速选择待巩固 | [2026-09-28/0215-kth-largest-element-in-an-array.md](2026-09-28/0215-kth-largest-element-in-an-array.md) |
| 347. 前 K 个高频元素 | ✅ 全量大顶堆版本通过，进阶待学习 | [2026-09-28/0347-top-k-frequent-elements.md](2026-09-28/0347-top-k-frequent-elements.md) |
| 912. 排序数组（快排、归并、堆排序） | 待复习；快排、递归/迭代堆排序通过，归并待确认 | [2026-09-28/0912-sort-an-array.md](2026-09-28/0912-sort-an-array.md) |

## 2026-09-27

| 题目 | 状态 | 笔记 |
|---|---|---|
| 739. 每日温度 | ✅ 已解决 | [2026-09-27/0739-daily-temperatures.md](2026-09-27/0739-daily-temperatures.md) |
| 84. 柱状图中最大的矩形（二刷） | ✅ 已解决，待巩固边界与距离 | [2026-09-27/0084-largest-rectangle-in-histogram.md](2026-09-27/0084-largest-rectangle-in-histogram.md) |

## 2026-09-23

| 题目 | 状态 | 笔记 |
|---|---|---|
| 3. 无重复字符的最长子串（二刷） | 代码已复核 | [2026-09-23/0003-longest-substring-without-repeating-characters.md](2026-09-23/0003-longest-substring-without-repeating-characters.md) |
| 146. LRU 缓存（三刷） | ✅ 已解决，待巩固流畅度 | [2026-09-23/0146-lru-cache.md](2026-09-23/0146-lru-cache.md) |

## 2026-09-22

| 题目 | 状态 | 笔记 |
|---|---|---|
| 53. 最大子数组和 | 已修正并复核 | [2026-09-22/0053-maximum-subarray.md](2026-09-22/0053-maximum-subarray.md) |
| 56. 合并区间 | ✅ 已解决 | [2026-09-22/0056-merge-intervals.md](2026-09-22/0056-merge-intervals.md) |
| 189. 轮转数组 | ✅ 已解决（三次反转） | [2026-09-22/0189-rotate-array.md](2026-09-22/0189-rotate-array.md) |
| 41. 缺失的第一个正数（二刷） | ✅ 已解决 | [2026-09-22/0041-first-missing-positive.md](2026-09-22/0041-first-missing-positive.md) |
| 35. 搜索插入位置 | ✅ 已解决 | [2026-09-22/0035-search-insert-position.md](2026-09-22/0035-search-insert-position.md) |
| 34. 在排序数组中查找元素的第一个和最后一个位置 | ✅ 已解决 | [2026-09-22/0034-find-first-and-last-position.md](2026-09-22/0034-find-first-and-last-position.md) |

## 2026-09-14

| 题目 | 状态 | 笔记 |
|---|---|---|
| 146. LRU 缓存（二刷） | ✅ 已解决 | [2026-09-14/0146-lru-cache.md](2026-09-14/0146-lru-cache.md) |
| 438. 找到字符串中所有字母异位词 | ✅ 已解决 | [2026-09-14/0438-find-all-anagrams-in-a-string.md](2026-09-14/0438-find-all-anagrams-in-a-string.md) |
| 3. 无重复字符的最长子串 | ✅ 已解决 | [2026-09-14/0003-longest-substring-without-repeating-characters.md](2026-09-14/0003-longest-substring-without-repeating-characters.md) |

## 2026-09-13

| 题目 | 状态 | 笔记 |
|---|---|---|
| 42. 接雨水 | ⚠️ 学习中（单调栈待巩固） | [2026-09-13/0042-trapping-rain-water.md](2026-09-13/0042-trapping-rain-water.md) |
| 994. 腐烂的橘子（二刷） | ✅ 已解决 | [2026-09-13/0994-rotting-oranges.md](2026-09-13/0994-rotting-oranges.md) |
| 200. 岛屿数量（二刷） | ✅ 已解决 | [2026-09-13/0200-number-of-islands.md](2026-09-13/0200-number-of-islands.md) |

## 2026-09-07

| 题目 | 状态 | 笔记 |
|---|---|---|
| 560. 和为 K 的子数组 | ✅ 已解决 | [2026-09-07/0560-subarray-sum-equals-k.md](2026-09-07/0560-subarray-sum-equals-k.md) |

## 2026-09-06

| 题目 | 状态 | 笔记 |
|---|---|---|
| 15. 三数之和 | ✅ 已解决 | [2026-09-06/0015-3sum.md](2026-09-06/0015-3sum.md) |
| 128. 最长连续序列 | ✅ 已解决 | [2026-09-06/0128-longest-consecutive-sequence.md](2026-09-06/0128-longest-consecutive-sequence.md) |
| 49. 字母异位词分组 | ✅ 已解决 | [2026-09-06/0049-group-anagrams.md](2026-09-06/0049-group-anagrams.md) |
| 1. 两数之和 | ✅ 已解决 | [2026-09-06/0001-two-sum.md](2026-09-06/0001-two-sum.md) |

## 2026-09-05

| 题目 | 状态 | 笔记 |
|---|---|---|
| 114. 二叉树展开为链表 | ✅ 已解决（含 O(1) 空间进阶） | [2026-09-05/0114-flatten-binary-tree-to-linked-list.md](2026-09-05/0114-flatten-binary-tree-to-linked-list.md) |
| 437. 路径总和 III | ✅ 已解决 | [2026-09-05/0437-path-sum-iii.md](2026-09-05/0437-path-sum-iii.md) |

## 2026-09-04

| 题目 | 状态 | 笔记 |
|---|---|---|
| 32. 最长有效括号 | ✅ 已解决 | [2026-09-04/0032-longest-valid-parentheses.md](2026-09-04/0032-longest-valid-parentheses.md) |

## 2026-09-03

| 题目 | 状态 | 笔记 |
|---|---|---|
| 84. 柱状图中最大的矩形 | ✅ 已解决 | [2026-09-03/0084-largest-rectangle-in-histogram.md](2026-09-03/0084-largest-rectangle-in-histogram.md) |
| 295. 数据流的中位数 | ✅ 已解决 | [2026-09-03/0295-find-median-from-data-stream.md](2026-09-03/0295-find-median-from-data-stream.md) |

## 2026-09-02

| 题目 | 状态 | 笔记 |
|---|---|---|
| 23. 合并 K 个升序链表 | ✅ 已解决 | [2026-09-02/0023-merge-k-sorted-lists.md](2026-09-02/0023-merge-k-sorted-lists.md) |

## 2026-09-01

| 题目 | 状态 | 笔记 |
|---|---|---|
| 25. K 个一组翻转链表 | ✅ 已解决 | [2026-09-01/0025-reverse-nodes-in-k-group.md](2026-09-01/0025-reverse-nodes-in-k-group.md) |
| 146. LRU 缓存 | ✅ 已解决 | [2026-09-01/0146-lru-cache.md](2026-09-01/0146-lru-cache.md) |

## 2026-08-31

| 题目 | 状态 | 笔记 |
|---|---|---|
| 24. 两两交换链表中的节点 | ✅ 已解决 | [2026-08-31/0024-swap-nodes-in-pairs.md](2026-08-31/0024-swap-nodes-in-pairs.md) |

## 2026-08-30

| 题目 | 状态 | 笔记 |
|---|---|---|
| 25. K 个一组翻转链表 | 📝 初始思路记录 | [2026-08-30/0025-reverse-nodes-in-k-group.md](2026-08-30/0025-reverse-nodes-in-k-group.md) |

## 2026-08-29

| 题目 | 状态 | 笔记 |
|---|---|---|
| 41. 缺失的第一个正数 | ✅ 已解决 | [2026-08-29/0041-first-missing-positive.md](2026-08-29/0041-first-missing-positive.md) |
| 76. 最小覆盖子串 | ✅ 已解决 | [2026-08-29/0076-minimum-window-substring.md](2026-08-29/0076-minimum-window-substring.md) |

## 2026-08-26

| 题目 | 状态 | 笔记 |
|---|---|---|
| 98. 验证二叉搜索树 | ✅ 已解决 | [2026-08-26/0098-validate-binary-search-tree.md](2026-08-26/0098-validate-binary-search-tree.md) |
| 108. 将有序数组转换为二叉搜索树 | ✅ 已解决 | [2026-08-26/0108-convert-sorted-array-to-bst.md](2026-08-26/0108-convert-sorted-array-to-bst.md) |
| 543. 二叉树的直径 | ✅ 已解决 | [2026-08-26/0543-diameter-of-binary-tree.md](2026-08-26/0543-diameter-of-binary-tree.md) |

## 2026-08-16

| 题目 | 状态 | 笔记 |
|---|---|---|
| 31. 下一个排列 | ✅ 已解决 | [2026-08-16/0031-next-permutation.md](2026-08-16/0031-next-permutation.md) |
| 75. 颜色分类 | ✅ 已解决 | [2026-08-16/0075-sort-colors.md](2026-08-16/0075-sort-colors.md) |
| 169. 多数元素 | ✅ 已解决 | [2026-08-16/0169-majority-element.md](2026-08-16/0169-majority-element.md) |
| 287. 寻找重复数 | ✅ 已解决（证明需复习） | [2026-08-16/0287-find-the-duplicate-number.md](2026-08-16/0287-find-the-duplicate-number.md) |

## 2026-08-14

| 题目 | 状态 | 笔记 |
|---|---|---|
| 136. 只出现一次的数字 | ✅ 已解决 | [2026-08-14/0136-single-number.md](2026-08-14/0136-single-number.md) |
| 208. 实现 Trie | ⚠️ 未完全掌握（需巩固） | [2026-08-14/0208-implement-trie-prefix-tree.md](2026-08-14/0208-implement-trie-prefix-tree.md) |

## 2026-08-13

| 题目 | 状态 | 笔记 |
|---|---|---|
| 207. 课程表 | ✅ 已解决 | [2026-08-13/0207-course-schedule.md](2026-08-13/0207-course-schedule.md) |
| 994. 腐烂的橘子 | ✅ 已解决 | [2026-08-13/0994-rotting-oranges.md](2026-08-13/0994-rotting-oranges.md) |

## 2026-08-12

| 题目 | 状态 | 笔记 |
|---|---|---|
| 200. 岛屿数量 | ✅ 已解决 | [2026-08-12/0200-number-of-islands.md](2026-08-12/0200-number-of-islands.md) |

## 2026-08-11

| 题目 | 状态 | 笔记 |
|---|---|---|
| 48. 旋转图像 | ✅ 已解决 | [2026-08-11/0048-rotate-image.md](2026-08-11/0048-rotate-image.md) |
| 54. 螺旋矩阵 | ✅ 已解决 | [2026-08-11/0054-spiral-matrix.md](2026-08-11/0054-spiral-matrix.md) |
| 73. 矩阵置零 | ✅ 已解决 | [2026-08-11/0073-set-matrix-zeroes.md](2026-08-11/0073-set-matrix-zeroes.md) |
| 240. 搜索二维矩阵 II | ✅ 已解决 | [2026-08-11/0240-search-a-2d-matrix-ii.md](2026-08-11/0240-search-a-2d-matrix-ii.md) |
