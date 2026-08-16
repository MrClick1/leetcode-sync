# 刷题交接文档（handoff）

> 用途：给"另一个我"（家里的笔记本 / 新的 Codex 会话）快速同步进度和个人易错点。新会话开始时先读本文件 + notes/README.md。

## 当前进度

| 题目 | 状态 | 复盘笔记 |
|---|---|---|
| 1. 两数之和 | ⏳ 未完成（脚手架已建） | — |
| 48. 旋转图像 | ✅ 已解决 | [notes/0048-rotate-image.md](notes/0048-rotate-image.md) |
| 54. 螺旋矩阵 | ✅ 已解决 | [notes/0054-spiral-matrix.md](notes/0054-spiral-matrix.md) |
| 73. 矩阵置零 | ✅ 已解决 | [notes/0073-set-matrix-zeroes.md](notes/0073-set-matrix-zeroes.md) |
| 75. 颜色分类 | ✅ 已解决 | [notes/0075-sort-colors.md](notes/0075-sort-colors.md) |
| 136. 只出现一次的数字 | ✅ 已解决 | [notes/0136-single-number.md](notes/0136-single-number.md) |
| 169. 多数元素 | ✅ 已解决 | [notes/0169-majority-element.md](notes/0169-majority-element.md) |
| 200. 岛屿数量 | ✅ 已解决（延伸练习待做） | [notes/0200-number-of-islands.md](notes/0200-number-of-islands.md) |
| 207. 课程表 | ✅ 已解决 | [notes/0207-course-schedule.md](notes/0207-course-schedule.md) |
| 208. 实现 Trie | ⚠️ 未完全掌握（需巩固） | [notes/0208-implement-trie-prefix-tree.md](notes/0208-implement-trie-prefix-tree.md) |
| 240. 搜索二维矩阵 II | ✅ 已解决 | [notes/0240-search-a-2d-matrix-ii.md](notes/0240-search-a-2d-matrix-ii.md) |
| 994. 腐烂的橘子 | ✅ 已解决 | [notes/0994-rotting-oranges.md](notes/0994-rotting-oranges.md) |

- 语言：C++；本地编译器：g++（WinLibs/MinGW）
- 已完成题目均通过本地测试；部分题目已做随机对拍
- GitHub：远程 `MrClick1/leetcode-sync`，**记得 push 未推送的进度**

## 个人疑难点（重点）

### 1. 模拟类 / 网格遍历类题目苦手 ⚠️
- 典型题：200 岛屿数量（BFS 模拟）、994 腐烂的橘子（多源 BFS 计时）、54 螺旋矩阵（边界收缩）、73 矩阵置零（原地标记）
- 症状：不知道每轮怎么"走"、边界怎么缩、标记怎么不互相污染
- 应对策略：
  - 先画图/手推小例子，把一轮的过程写出来再动代码
  - 记住网格题通用零件：方向数组、visited/沉岛、四边界收缩
  - 模拟题最容易错的是"停止条件"和"防重复"，写码前先想清楚这两点

### 2. 具体踩过的坑（容易复发）
- BFS 忘写 `que.pop()` → 死循环 TLE（200）
- `break` 只跳出内层循环 → 多岛合并 / 漏岛（200）
- 清零时标记区被数据污染（73）
- 循环变量遮蔽外层 `i`，误导自己（200）
- 选择新候选人时忘记给第一票，导致程序退化为返回最后一个元素（169）
- 测试数据特征过于一致会掩盖错误；应加入能区分错误行为的反例（169）
- 测试输出中文在 Windows 终端乱码（GBK 代码页），输出语句用英文

### 3. 学习偏好
- `problem.md` 只放原题面，**不要放提示/解法方向**（会破坏练习）
- 卡住超过 20 分钟才给提示；提示先给思路/结构，不直接给完整代码
- 面试延伸（如 200 的沉岛法/DFS）记入 notes 的"延伸练习"并打勾

## 待做延伸练习

- [ ] 200. 岛屿数量：沉岛法（改 grid 省 visited）
- [ ] 200. 岛屿数量：DFS 版
- [ ] 994. 腐烂的橘子：不修改原网格（dist 数组版）
- [ ] 994. 腐烂的橘子：输出每个格子腐烂时间矩阵
- [ ] 207. 课程表：DFS 三色标记判环
- [ ] 207. 课程表：210. 课程表 II（输出合法学习顺序）
- [ ] 208. Trie：211. 添加与搜索单词（支持 . 通配符）
- [ ] 208. Trie：统计前缀数量 / 删除单词变体
- [ ] 136. 位运算：137（其余出现三次，逐位 mod 3）
- [ ] 136. 位运算：260（两个单数，XOR + 分组）

## 工作流速查

- 每道题目录：`problems/题号-题名/`（problem.md、solution.cpp、test.cpp）
- 完成一道 → Codex review + 随机对拍 → 写复盘笔记 `notes/`
- 一批做完 → `git add -A && git commit && git push`
- 新会话先读：本文件 + [notes/README.md](notes/README.md)
