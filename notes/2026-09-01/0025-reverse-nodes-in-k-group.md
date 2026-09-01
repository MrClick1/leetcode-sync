# 25. K 个一组翻转链表 —— 复盘笔记

状态：✅ 已解决

## 解题思路

这道题可以看作“不断翻转长度为 `k` 的闭区间”。使用虚拟头节点后，让 `groupPrev` 始终指向当前待翻转分组前面的节点。

每一轮分为四步：

1. 从 `groupPrev` 开始向后走 `k` 步，找到当前组最后一个节点 `kth`。
2. 如果 `kth == nullptr`，说明剩余节点不足 `k` 个，直接结束，剩余部分保持原顺序。
3. 保存当前组的边界：`groupStart = groupPrev->next`，`groupNext = kth->next`。
4. 翻转区间 `[groupStart, groupNext)`，再把前一组、当前组和后一组重新连接起来。

翻转完成后，原来的 `groupStart` 会变成当前组的最后一个节点，所以令 `groupPrev = groupStart`，继续处理下一组。

## 翻转过程

假设当前结构为：

```text
groupPrev -> 1 -> 2 -> 3 -> groupNext
```

初始化：

```cpp
ListNode* prev = groupNext;
ListNode* curr = groupStart;
```

`prev` 初始化为 `groupNext` 很关键。这样第一次执行 `curr->next = prev` 时，原组头 `1` 就直接连接到了下一组，翻转结束后不需要再单独连接组尾。

循环中每次只翻转一条边：

```cpp
ListNode* nextN = curr->next;
curr->next = prev;
prev = curr;
curr = nextN;
```

以三个节点为例：

```text
初始：prev = groupNext，curr = 1
第 1 次：1 -> groupNext，prev = 1，curr = 2
第 2 次：2 -> 1，        prev = 2，curr = 3
第 3 次：3 -> 2，        prev = 3，curr = groupNext
```

此时 `prev` 指向翻转后的组头 `3`，执行 `groupPrev->next = prev` 后得到：

```text
groupPrev -> 3 -> 2 -> 1 -> groupNext
```

## 边界情况

- `k == 1` 时，每组只有一个节点，链表结构保持不变。
- `k` 等于链表长度时，整条链表翻转一次。
- 最后一组不足 `k` 个节点时，找不到 `kth`，因此不会进入翻转过程。
- 算法只修改 `next` 指针，没有修改节点内部的 `val`。

## 哑节点的内存管理

当前实现使用 `new` 创建虚拟头节点，但函数返回前没有释放它，因此每次调用会留下一个辅助节点的内存泄漏。这不影响本题判题结果，也不改变额外空间复杂度为 O(1) 的数量级；工程代码中更推荐使用栈上的虚拟头节点。

## 复杂度

- 每个节点被查找和翻转常数次，时间复杂度 O(n)。
- 只使用固定数量的指针，额外空间复杂度 O(1)。

## 验证

- 固定测试全部通过，覆盖 `k = 1`、`k = n`、完整分组、存在余数、重复值、边界值和 5000 个节点。
- 测试检查输出节点的指针身份，确保实际翻转节点，而不是修改节点值。
- 5000 组随机链表验证全部通过，同时检查节点顺序、节点值不变以及链表无环。
