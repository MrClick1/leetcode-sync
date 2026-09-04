#include <bits/stdc++.h>

using namespace std;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right)
        : val(x), left(left), right(right) {}
};

class Solution {
public:
    int kthSmallest(TreeNode* root, int k) {
        int result = -1;
        int cnt = 0;
        stack<TreeNode*> stk;

        TreeNode* cur = root;
        // 一直往左遍历，遍历到空节点就 pop() 回溯
        while (cur != nullptr || !stk.empty()) {
            while (cur != nullptr) {
                stk.push(cur);
                cur = cur->left;
            }
            // cur 为空
            // 左中右
            cur = stk.top();
            stk.pop();
            cnt++;
            if (cnt == k) {
                result = cur->val;
            }
            cur = cur->right;
        }
 
        return result;
    }
};
