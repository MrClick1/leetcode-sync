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
    int ans = 0;

    // 返回以 root 为根的子树高度
    int depth(TreeNode* root) {
        if (root == nullptr) {
            return 0;
        }

        int L = depth(root->left);
        int R = depth(root->right);

        ans = max(ans, L + R);
        return 1 + max(L, R);   // 当前节点的高度
    }

    int diameterOfBinaryTree(TreeNode* root) {
        // 在这里实现你的解法、
        ans = 0;
        depth(root);
        return ans;
    }
};