#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <unordered_set>
#include <vector>

#include "solution.cpp"

using namespace std;

struct BuiltList {
    ListNode* head;
    vector<ListNode*> nodes;
    vector<int> originalValues;
};

static BuiltList buildList(const vector<int>& values) {
    BuiltList result{nullptr, {}, values};
    ListNode dummy;
    ListNode* tail = &dummy;

    for (int value : values) {
        ListNode* node = new ListNode(value);
        result.nodes.push_back(node);
        tail->next = node;
        tail = node;
    }

    result.head = dummy.next;
    return result;
}

static void fail(const char* message, size_t listSize, int k) {
    cerr << "FAIL: " << message
         << " (list size " << listSize << ", k " << k << ")" << endl;
    exit(1);
}

static void check(const vector<int>& values, int k) {
    BuiltList list = buildList(values);

    vector<ListNode*> expectedOrder = list.nodes;
    for (size_t start = 0; start + k <= expectedOrder.size(); start += k) {
        reverse(expectedOrder.begin() + start,
                expectedOrder.begin() + start + k);
    }

    ListNode* result = Solution().reverseKGroup(list.head, k);

    vector<ListNode*> actualOrder;
    unordered_set<ListNode*> visited;
    for (ListNode* node = result; node != nullptr; node = node->next) {
        if (!visited.insert(node).second) {
            fail("result contains a cycle", values.size(), k);
        }
        actualOrder.push_back(node);
        if (actualOrder.size() > list.nodes.size()) {
            fail("result contains unexpected nodes", values.size(), k);
        }
    }

    if (actualOrder != expectedOrder) {
        fail("node pointer order is incorrect", values.size(), k);
    }

    for (size_t i = 0; i < list.nodes.size(); ++i) {
        if (list.nodes[i]->val != list.originalValues[i]) {
            fail("a node value was modified", values.size(), k);
        }
    }

    for (ListNode* node : list.nodes) {
        delete node;
    }
}

int main() {
    // Examples
    check({1, 2, 3, 4, 5}, 2);
    check({1, 2, 3, 4, 5}, 3);

    // k = 1 and one-node list
    check({1}, 1);
    check({1, 2, 3, 4}, 1);

    // Complete groups
    check({1, 2}, 2);
    check({1, 2, 3, 4}, 4);
    check({1, 2, 3, 4, 5, 6}, 3);

    // Incomplete final group must remain unchanged
    check({1, 2, 3}, 2);
    check({1, 2, 3, 4, 5, 6, 7}, 3);

    // Repeated and boundary values
    check({0, 1000, 0, 1000, 0}, 2);
    check({7, 7, 7, 7, 7, 7}, 4);

    // Maximum list length with a remainder
    vector<int> maximumLength;
    for (int value = 0; value < 5000; ++value) {
        maximumLength.push_back(value % 1001);
    }
    check(maximumLength, 37);

    cout << "Scaffold tests passed!" << endl;
    return 0;
}
