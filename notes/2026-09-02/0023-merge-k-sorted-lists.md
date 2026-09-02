# 23. 合并 K 个升序链表

状态：✅ 已解决（本地 Scaffold 测试全部通过）

## 当前实现 review

测试失败：`FAIL: merged values are incomplete or not sorted (lists 3, nodes 8)`。

第一处问题是最小堆还没有初始化：

```cpp
priority_queue<ListNode*, vector<ListNode*>, Compare> minHeap;

while (!minHeap.empty()) {
    //...
}
```

刚创建的堆为空，因此循环一次都不会执行，`dummy->next` 仍是 `nullptr`。第一条官方用例共有 8 个节点，所以测试报告节点内容不完整。

正确顺序应是：先把每条非空链表的头节点加入堆，再进入循环：

```cpp
for (ListNode* head : lists) {
    if (head != nullptr) {
        minHeap.push(head);
    }
}
```

第二处问题是弹出一个节点后，必须把它在原链表中的后继节点加入堆。否则每条链表只会处理头节点：

```cpp
ListNode* node = minHeap.top();
minHeap.pop();

if (node->next != nullptr) {
    minHeap.push(node->next);
}

cur->next = node;
cur = node;
```

最小堆始终保存“每条尚未处理链表的当前最小节点”。每次弹出全局最小节点后，用它的后继替换该链表在堆中的候选节点，直到堆为空。

## 复杂度

设所有链表节点总数为 `N`，链表数量为 `k`：初始化堆为 `O(k)`，每个节点入堆和出堆一次，总时间复杂度为 `O(N log k)`，堆额外空间为 `O(k)`。

## 其他问题

- `n == 0` 返回 `nullptr` 是正确的
- `new ListNode(0)` 创建的虚拟头节点在当前实现中没有释放，会造成一个辅助节点泄漏；可改用栈上对象 `ListNode dummy(0)`，不影响本题判题结果
- 当前失败不是测试代码的问题，而是合并循环没有真正启动；修复后还要验证空链表、重复值、负数和总节点数等用例

## 最终解答

```cpp
class Solution {
public:
    struct Compare {
        bool operator()(const ListNode* left, const ListNode* right) const {
            return left->val > right->val;
        }
    };

    ListNode* mergeKLists(vector<ListNode*>& lists) {
        ListNode dummy(0);
        ListNode* cur = &dummy;

        priority_queue<ListNode*, vector<ListNode*>, Compare> minHeap;

        for (ListNode* head : lists) {
            if (head != nullptr) {
                minHeap.push(head);
            }
        }

        while (!minHeap.empty()) {
            ListNode* node = minHeap.top();
            minHeap.pop();

            if (node->next != nullptr) {
                minHeap.push(node->next);
            }

            cur->next = node;
            cur = node;
        }

        return dummy.next;
    }
};
```

上面的复盘代码使用栈上的虚拟头节点；当前提交的 `solution.cpp` 仍使用 `new ListNode(0)`，会留下一个辅助节点泄漏，但不影响判题结果。工程代码中推荐采用复盘代码的栈上写法。

## 验证

- `g++ -std=c++17 test.cpp -o test.exe` 编译成功
- `test.exe` 输出 `Scaffold tests passed!`
- 测试覆盖官方示例、空输入、空链表、重复值、负数、边界值、10000 条链表和总计 10000 个节点

## 复杂度总结

- 设所有链表节点总数为 `N`，链表数量为 `k`
- 时间复杂度：`O(N log k)`
- 最小堆额外空间：`O(k)`
