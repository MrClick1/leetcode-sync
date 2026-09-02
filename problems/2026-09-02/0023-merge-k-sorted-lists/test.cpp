#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

struct ListNode {
    int val;
    ListNode* next;

    ListNode() : val(0), next(nullptr) {}
    explicit ListNode(int value) : val(value), next(nullptr) {}
    ListNode(int value, ListNode* nextNode) : val(value), next(nextNode) {}
};

#include "solution.cpp"

struct BuiltInput {
    vector<ListNode*> heads;
    vector<ListNode*> originalNodes;
};

static BuiltInput buildInput(const vector<vector<int>>& values) {
    BuiltInput input;
    input.heads.reserve(values.size());

    for (const vector<int>& listValues : values) {
        ListNode dummy;
        ListNode* tail = &dummy;

        for (int value : listValues) {
            ListNode* node = new ListNode(value);
            input.originalNodes.push_back(node);
            tail->next = node;
            tail = node;
        }

        input.heads.push_back(dummy.next);
    }

    return input;
}

static void fail(const string& message, size_t listCount, size_t nodeCount) {
    cerr << "FAIL: " << message
         << " (lists " << listCount
         << ", nodes " << nodeCount << ")" << endl;
    exit(1);
}

static void check(const vector<vector<int>>& values) {
    vector<int> expected;
    for (const vector<int>& listValues : values) {
        expected.insert(expected.end(), listValues.begin(), listValues.end());
    }
    sort(expected.begin(), expected.end());

    BuiltInput input = buildInput(values);
    ListNode* result = Solution().mergeKLists(input.heads);

    vector<int> actual;
    vector<ListNode*> outputNodes;
    unordered_set<ListNode*> visited;

    for (ListNode* node = result; node != nullptr; node = node->next) {
        if (!visited.insert(node).second) {
            fail("result contains a cycle", values.size(), expected.size());
        }

        actual.push_back(node->val);
        outputNodes.push_back(node);
        if (actual.size() > expected.size()) {
            fail("result contains too many nodes", values.size(), expected.size());
        }
    }

    if (actual != expected) {
        fail("merged values are incomplete or not sorted", values.size(), expected.size());
    }

    unordered_set<ListNode*> nodesToDelete(
        input.originalNodes.begin(), input.originalNodes.end());
    nodesToDelete.insert(outputNodes.begin(), outputNodes.end());
    for (ListNode* node : nodesToDelete) {
        delete node;
    }
}

int main() {
    // Official examples
    check({{1, 4, 5}, {1, 3, 4}, {2, 6}});
    check({});
    check({{}});

    // Empty lists mixed with non-empty lists
    check({{}, {-3, 0, 8}, {}, {1, 1}, {}});
    check(vector<vector<int>>(10000));

    // One list and one node
    check({{-5, -1, 0, 4, 9}});
    check({{7}});

    // Repeated values, negatives, and value boundaries
    check({{-10000, -2, -2, 10000}, {-10000, -2, 3, 10000}, {-2, -2}});
    check({{1, 1, 1}, {1, 1}, {1}, {1, 1, 1, 1}});

    // Maximum number of lists with one non-empty list
    vector<vector<int>> maximumListCount(10000);
    maximumListCount.back() = {-10000, 0, 10000};
    check(maximumListCount);

    // Maximum total node count: 20 lists, 500 nodes each
    vector<vector<int>> maximumNodes(20);
    for (int listIndex = 0; listIndex < 20; ++listIndex) {
        for (int position = 0; position < 500; ++position) {
            maximumNodes[listIndex].push_back(position * 20 + listIndex - 10000);
        }
    }
    check(maximumNodes);

    cout << "Scaffold tests passed!" << endl;
    return 0;
}
