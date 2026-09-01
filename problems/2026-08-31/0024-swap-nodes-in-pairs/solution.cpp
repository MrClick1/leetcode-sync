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
    ListNode* swapPairs(ListNode* head) {
        // 在这里实现你的解法
        ListNode dummy(0, head);
        ListNode* pre = &dummy;

        // ListNode* pre = dummy;

        while (pre->next != nullptr && pre->next->next != nullptr) {
            ListNode* first = pre->next;
            ListNode* second = pre->next->next;

            first->next = second->next;
            second->next = first;
            pre->next = second;

            pre = first;
        }

        return dummy.next;
    }
};
