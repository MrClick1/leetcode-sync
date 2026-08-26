#include <iostream>
#include <optional>
#include <queue>
#include <vector>

#include "solution.cpp"

// 用层序序列建树：nullopt 表示空节点
static TreeNode* buildTree(const std::vector<std::optional<int>>& vals) {
    if (vals.empty() || !vals[0].has_value()) return nullptr;
    TreeNode* root = new TreeNode(*vals[0]);
    std::queue<TreeNode*> q;
    q.push(root);
    size_t i = 1;
    while (!q.empty() && i < vals.size()) {
        TreeNode* cur = q.front();
        q.pop();
        if (i < vals.size() && vals[i].has_value()) {
            cur->left = new TreeNode(*vals[i]);
            q.push(cur->left);
        }
        i++;
        if (i < vals.size() && vals[i].has_value()) {
            cur->right = new TreeNode(*vals[i]);
            q.push(cur->right);
        }
        i++;
    }
    return root;
}

static void check(const std::vector<std::optional<int>>& vals, int expected) {
    TreeNode* root = buildTree(vals);
    int got = Solution().diameterOfBinaryTree(root);
    if (got != expected) {
        std::cerr << "FAIL: expected " << expected << ", got " << got << "\n";
        std::exit(1);
    }
}

int main() {
    // 示例 1、2
    check({1, 2, 3, 4, 5}, 3);
    check({1, 2}, 1);

    // 边界：单节点
    check({1}, 0);

    // 左链：1-2-3-4
    check({1, 2, std::nullopt, 3, std::nullopt, 4}, 3);

    // 右链：1-2-3
    check({1, std::nullopt, 2, std::nullopt, 3}, 2);

    // 完全二叉树（三层）：直径 = 左深 + 右深 = 4
    check({1, 2, 3, 4, 5, 6, 7}, 4);

    // 直径不经过根节点：5-3-2-4
    check({1, 2, std::nullopt, 3, 4, 5, 6}, 3);

    // 负数
    check({-1, -2, -3}, 2);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}