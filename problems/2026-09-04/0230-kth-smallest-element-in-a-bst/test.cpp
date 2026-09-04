#include <cstdlib>
#include <iostream>
#include <optional>
#include <queue>
#include <stack>
#include <string>
#include <vector>

#include "solution.cpp"

using namespace std;

static TreeNode* buildTree(const vector<optional<int>>& values) {
    if (values.empty() || !values[0].has_value()) {
        return nullptr;
    }

    TreeNode* root = new TreeNode(*values[0]);
    queue<TreeNode*> nodes;
    nodes.push(root);

    size_t index = 1;
    while (!nodes.empty() && index < values.size()) {
        TreeNode* current = nodes.front();
        nodes.pop();

        if (index < values.size() && values[index].has_value()) {
            current->left = new TreeNode(*values[index]);
            nodes.push(current->left);
        }
        ++index;

        if (index < values.size() && values[index].has_value()) {
            current->right = new TreeNode(*values[index]);
            nodes.push(current->right);
        }
        ++index;
    }

    return root;
}

static void destroyTree(TreeNode* root) {
    if (root == nullptr) {
        return;
    }

    stack<TreeNode*> nodes;
    nodes.push(root);
    while (!nodes.empty()) {
        TreeNode* current = nodes.top();
        nodes.pop();

        if (current->left != nullptr) {
            nodes.push(current->left);
        }
        if (current->right != nullptr) {
            nodes.push(current->right);
        }
        delete current;
    }
}

static void check(TreeNode* root, int k, int expected,
                  const string& context) {
    const int actual = Solution().kthSmallest(root, k);
    if (actual != expected) {
        cerr << "FAIL: " << context
             << " (k " << k
             << ", expected " << expected
             << ", got " << actual << ")" << endl;
        destroyTree(root);
        exit(1);
    }

    destroyTree(root);
}

static void check(const vector<optional<int>>& values, int k, int expected,
                  const string& context) {
    check(buildTree(values), k, expected, context);
}

static void testExamples() {
    check({3, 1, 4, nullopt, 2}, 1, 1, "example 1");
    check({5, 3, 6, 2, 4, nullopt, nullopt, 1}, 3, 3, "example 2");
}

static void testBasicCases() {
    check({1}, 1, 1, "single node");

    check({2, 1}, 1, 1, "two nodes: first element");
    check({2, 1}, 2, 2, "two nodes: last element");
    check({1, nullopt, 2}, 2, 2, "right child");

    check({4, 2, 6, 1, 3, 5, 7}, 1, 1,
          "balanced tree: first element");
    check({4, 2, 6, 1, 3, 5, 7}, 4, 4,
          "balanced tree: root rank");
    check({4, 2, 6, 1, 3, 5, 7}, 7, 7,
          "balanced tree: last element");

    check({8, 3, 10, 1, 6, nullopt, 14,
           nullopt, nullopt, 4, 7, 13},
          5, 7, "irregular tree");
    check({5000, 0, 10000}, 3, 10000, "value limits");
}

static TreeNode* buildRightSkewedTree(int nodeCount) {
    TreeNode* root = new TreeNode(0);
    TreeNode* current = root;

    for (int value = 1; value < nodeCount; ++value) {
        current->right = new TreeNode(value);
        current = current->right;
    }

    return root;
}

static TreeNode* buildLeftSkewedTree(int nodeCount) {
    TreeNode* root = new TreeNode(nodeCount - 1);
    TreeNode* current = root;

    for (int value = nodeCount - 2; value >= 0; --value) {
        current->left = new TreeNode(value);
        current = current->left;
    }

    return root;
}

static void testMaximumSizeCases() {
    constexpr int nodeCount = 10000;
    check(buildRightSkewedTree(nodeCount), nodeCount, nodeCount - 1,
          "maximum size: right-skewed tree, last element");
    check(buildLeftSkewedTree(nodeCount), 1, 0,
          "maximum size: left-skewed tree, first element");
}

int main() {
    testExamples();
    testBasicCases();
    testMaximumSizeCases();

    cout << "Scaffold tests passed!" << endl;
    return 0;
}
