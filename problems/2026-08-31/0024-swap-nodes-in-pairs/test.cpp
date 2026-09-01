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

static void fail(const char* message, size_t listSize) {
    cerr << "FAIL: " << message << " (list size " << listSize << ")" << endl;
    exit(1);
}

static void check(const vector<int>& values) {
    BuiltList list = buildList(values);

    vector<ListNode*> expectedOrder = list.nodes;
    for (size_t i = 0; i + 1 < expectedOrder.size(); i += 2) {
        swap(expectedOrder[i], expectedOrder[i + 1]);
    }

    ListNode* result = Solution().swapPairs(list.head);

    vector<ListNode*> actualOrder;
    unordered_set<ListNode*> visited;
    for (ListNode* node = result; node != nullptr; node = node->next) {
        if (!visited.insert(node).second) {
            fail("result contains a cycle", values.size());
        }
        actualOrder.push_back(node);
        if (actualOrder.size() > list.nodes.size()) {
            fail("result contains unexpected nodes", values.size());
        }
    }

    if (actualOrder != expectedOrder) {
        fail("nodes were not rearranged into pairwise order", values.size());
    }

    for (size_t i = 0; i < list.nodes.size(); ++i) {
        if (list.nodes[i]->val != list.originalValues[i]) {
            fail("a node value was modified", values.size());
        }
    }

    for (ListNode* node : list.nodes) {
        delete node;
    }
}

int main() {
    // Examples
    check({1, 2, 3, 4});
    check({});
    check({1});

    // Even and odd lengths
    check({1, 2});
    check({1, 2, 3});
    check({1, 2, 3, 4, 5});

    // Repeated and boundary values
    check({0, 100, 0, 100});
    check({7, 7, 7, 7, 7, 7});

    // Maximum list length
    vector<int> maximumLength;
    for (int value = 0; value < 100; ++value) {
        maximumLength.push_back(value);
    }
    check(maximumLength);

    cout << "Scaffold tests passed!" << endl;
    return 0;
}
