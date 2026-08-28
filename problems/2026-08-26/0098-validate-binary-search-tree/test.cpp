#include <climits>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <queue>
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

static void destroyTree(TreeNode* node) {
    if (!node) return;
    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}

static void check(const vector<optional<int>>& values, bool expected) {
    TreeNode* root = buildTree(values);
    bool actual = Solution().isValidBST(root);

    if (actual != expected) {
        cerr << "FAIL: expected " << boolalpha << expected
             << ", got " << actual << endl;
        destroyTree(root);
        exit(1);
    }

    destroyTree(root);
}

int main() {
    // Examples
    check({2, 1, 3}, true);
    check({5, 1, 4, nullopt, nullopt, 3, 6}, false);

    // Basic valid trees
    check({1}, true);
    check({-1, -2, 0}, true);
    check({0, INT_MIN, INT_MAX}, true);
    check({INT_MIN, nullopt, 0}, true);
    check({INT_MAX, -1, nullopt}, true);
    check({1, nullopt, 2}, true);

    // Invalid because a descendant violates an ancestor's bound
    check({5, 1, 7, nullopt, nullopt, 4, 8}, false);
    check({5, 3, 7, 2, 6}, false);
    check({10, 5, 15, 2, 12, 11, 20}, false);

    // Duplicates are not allowed
    check({2, 2, 3}, false);
    check({2, 1, 2}, false);

    cout << "Scaffold tests passed!" << endl;
    return 0;
}