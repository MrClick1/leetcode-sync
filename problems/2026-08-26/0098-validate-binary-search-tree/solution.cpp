#include <bits/stdc++.h>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right)
        : val(x), left(left), right(right) {}
};

class Solution {
public:
    bool check(TreeNode* node, long long lower, long long upper) {
        if (node == nullptr) {
            return true;
        }

        if (node->val <= lower || node->val >= upper) {
            return false;
        }

        bool leftOK = check(node->left, lower, node->val);
        bool rightOK = check(node->right, node->val, upper);

        return leftOK && rightOK;
    }

    bool isValidBST(TreeNode* root) {
        // 在这里实现你的解法
        return check(root, LLONG_MIN, LLONG_MAX);
    }
};