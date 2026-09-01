#include <bits/stdc++.h>

using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
public:
    ListNode* reverseKGroup(ListNode* head, int k) {
        // 在这里实现你的解法
        ListNode* dummy = new ListNode(0, head);

        ListNode* groupPrev = dummy;

        while (true) {
            ListNode* kth = groupPrev;
            // 往后面找 k 个节点
            for (int i = 0; i < k && kth != nullptr; i++) {
                kth = kth->next;
            }

            if (kth == nullptr) {
                // 直接结束，不做翻转
                break;
            }

            // 翻转当前区间
            // 保存边界
            ListNode* groupStart = groupPrev->next;
            ListNode* groupNext = kth->next;

            // 初始化
            ListNode* prev = groupNext;
            ListNode* curr = groupStart;

            // 开始翻转
            while (curr != groupNext) {
                ListNode* nextN = curr->next;   // 2

                // 当前节点反向指向 prev
                // groupPrev -> 1 -> 2 -> 3 -> groupNext
                // groupPrev -> 3 -> 2 -> 1 -> groupNext

                // 1 -> groupNext
                curr->next = prev;
                prev = curr;        // 1
                curr = nextN;       // 2
            }
            groupPrev->next = prev;

            // 进入下一个区间，更新 groupPrev
            groupPrev = groupStart;
        }

        return dummy->next;
    }
};
