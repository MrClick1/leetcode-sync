#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    vector<int> rightSideView(TreeNode* root) {
        vector<int> res;
        queue<TreeNode*> que;

        // 层序遍历，只存放一层中最后一个元素
        if (root == nullptr) return res;
        que.push(root);

        while (!que.empty()) {
            int n = static_cast<int>(que.size());

            for (int i = 0; i < n; i++) {
                TreeNode* node = que.front();
                que.pop();
                if (node->left != nullptr) que.push(node->left);
                if (node->right != nullptr) que.push(node->right);
                if (i == n-1) {
                    res.push_back(node->val);
                }
            }
        }
        return res;
    }
};