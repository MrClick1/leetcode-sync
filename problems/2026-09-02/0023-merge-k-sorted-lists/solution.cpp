#include <bits/stdc++.h>

using namespace std;

// ListNode 由 LeetCode 和本地 test.cpp 提供，请勿在这里重复定义。
class Solution {
public:
    struct Compare{
        bool operator()(const ListNode* left, const ListNode* right) const {
            return left->val > right->val;  // > 返回 true  表示 left 的优先级比 right 低，是最小堆
        }
    };

    ListNode* mergeKLists(vector<ListNode*>& lists) {
        // 在这里实现你的解法
        int n = static_cast<int>(lists.size());

        if (n == 0) {
            return nullptr;
        }
        ListNode* dummy = new ListNode(0);
        ListNode* cur = dummy;

        // 最小堆
        priority_queue<ListNode*, vector<ListNode*>, Compare> minHeap;

        for (int i = 0; i < n; i++) {
            if (lists[i] != nullptr) {
                minHeap.push(lists[i]);
            }
        }

        while (!minHeap.empty()) {
            ListNode* node = minHeap.top();
            minHeap.pop();
            // 把当前节点所在链表的下一个节点加入堆
            if (node->next != nullptr) {
                minHeap.push(node->next);
            }

            cur->next = node;
            cur = cur->next;
        }

        return dummy->next;
    }
};
