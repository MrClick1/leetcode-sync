# HANDOFF（本地会话交接）

> 用途：在同一台机器上，让另一个 Codex / 终端会话快速接手当前工作状态。
> 新会话开始工作时，先读本文件，再按需读 notes/README.md 和题目目录中的代码。

## 当前进行中

暂无，等待用户指定下一题。

## 刚完成

**25. K 个一组翻转链表（2026-09-01）✅**

- 使用虚拟头节点和 `groupPrev` 划分每个长度为 k 的分组，迭代翻转区间 `[groupStart, groupNext)`。
- 通过将 `prev` 初始化为 `groupNext`，让翻转后的组尾直接连接下一组；原组头变为组尾后作为下一轮的 `groupPrev`。
- 固定测试和 5000 组随机链表验证全部通过，确认节点身份、节点值、余数分组和无环性质正确。
- 当前保留堆上虚拟头节点写法；固定一个节点的内存泄漏已记录在复盘笔记中，不影响本题判题结果。
- 复盘笔记已写入 notes/2026-09-01/0025-reverse-nodes-in-k-group.md。

**24. 两两交换链表中的节点（2026-08-31）✅**

- 使用栈上虚拟头节点和 pre 指针，迭代交换每一对相邻节点。
- 修复了 first/second 指向同一节点造成的自环，以及交换后未推进 pre 的死循环问题。
- 固定测试和 5000 组随机链表验证全部通过，确认节点身份、节点值和无环性质正确。
- 复盘笔记已写入 notes/2026-08-31/0024-swap-nodes-in-pairs.md。
**76. 最小覆盖子串（2026-08-29）✅**

- 当前在家使用 LeetCode 网页端练习，本题未创建本地代码脚手架。
- 使用 `need` / `window` 双哈希表与 `required` / `formed` 状态实现可变长度滑动窗口。
- LeetCode 全部测试通过，复杂度为 `O(|s| + |t|)`。
- 学习过程中补充了 `unordered_map::size()`、`static_cast`、C++ 命名类型转换和 `substr` 越界等语法笔记。
- 复盘笔记已写入 notes/2026-08-29/0076-minimum-window-substring.md。

**98. 验证二叉搜索树（2026-08-26，完成验证于 2026-08-28）✅**

- 目录：problems/2026-08-26/0098-validate-binary-search-tree/
- 使用 long long 上下界递归验证所有祖先约束，严格拒绝重复值。
- 固定测试全部通过。
- 10000 棵随机二叉树与独立的迭代中序遍历参考实现对比全部通过。
- 复盘笔记已写入 notes/2026-08-26/0098-validate-binary-search-tree.md。

**108. 将有序数组转换为二叉搜索树（2026-08-26）✅**

- 使用中点作为根节点，递归构造左右区间。
- 固定测试和 5000 组随机严格递增数组测试全部通过。
- 复盘笔记已写入 notes/2026-08-26/0108-convert-sorted-array-to-bst.md。

**543. 二叉树的直径（2026-08-26）✅**

- 后序 depth 递归，全局 ans = max(ans, 左高度 + 右高度)。
- 固定用例和 2000 棵随机树对拍已通过。
- 复盘笔记已写入 notes/2026-08-26/0543-diameter-of-binary-tree.md。

## Git 状态

- 当前工作分支为 main，远程分支为 origin/main。
- 25 的代码、测试、复盘笔记和索引已经完成；同步状态以 git status 为准。
- 用户已明确要求：每道题完成并更新笔记后，自动执行 git add、commit、push。
- 新题目统一归档到 problems/YYYY-MM-DD/。

## 环境与命令

### 编译 / 运行

g++ 不在默认 PATH 里，要用完整路径：

C:\Users\BHJ4SZH\AppData\Local\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin\g++.exe

在题目目录中运行：

g++.exe -std=c++17 test.cpp -o test.exe
.\test.exe

### 文件写入

- apply_patch 在此环境可能因 sandbox split-writable-roots 失败。
- 如果 apply_patch 失败，写文件使用 PowerShell 的 UTF-8 无 BOM 方式。
- 测试和程序输出使用英文，避免 Windows 终端编码问题。
- PowerShell 里不要使用 $HOME 作为变量。

## 工作流约定

- problem.md 只放原题面，绝不写提示或解法方向。
- 用户卡住时先给思路和结构，不直接给完整代码。
- 每完成一题：review → 本地测试 → 随机验证 → 更新 notes/YYYY-MM-DD/、notes/README.md 和交接文档。
- 当前在家学习时默认直接使用 LeetCode 网页端，只记录 notes，不创建本地题目脚手架；用户另有要求时除外。
- 当前题目完成后，下一题等待用户指定，不要擅自开始新题。
- 每道题完成且检查无误后，按用户要求自动提交并推送相关变更。

## 下次交接前更新

- [x] 25 已完成并写入复盘笔记。
- [x] 24 已完成并写入复盘笔记。
- [x] 98 已完成并写入复盘笔记。
- [x] 76 已完成并写入复盘笔记，C++ 语法参考文档已更新。
- [x] notes/README.md、handoff.md、HANDOFF.md 已同步。
- [ ] 等待用户指定下一题。
