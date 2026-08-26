#include <cstdlib>
#include <iostream>
#include <vector>

#include "solution.cpp"

// 检查是否高度平衡，同时通过引用返回子树高度
static bool isBalanced(TreeNode* node, int& height) {
    if (!node) {
        height = 0;
        return true;
    }
    int lh = 0, rh = 0;
    if (!isBalanced(node->left, lh)) return false;
    if (!isBalanced(node->right, rh)) return false;
    height = 1 + std::max(lh, rh);
    return std::abs(lh - rh) <= 1;
}

static void inorder(TreeNode* node, std::vector<int>& out) {
    if (!node) return;
    inorder(node->left, out);
    out.push_back(node->val);
    inorder(node->right, out);
}

// 正确答案不唯一：只验证"是 BST 且中序等于 nums" + "高度平衡"
static void check(std::vector<int> nums) {
    TreeNode* root = Solution().sortedArrayToBST(nums);

    std::vector<int> vals;
    inorder(root, vals);
    if (vals != nums) {
        std::cerr << "FAIL: in-order traversal does not match nums\n";
        std::exit(1);
    }

    int h = 0;
    if (!isBalanced(root, h)) {
        std::cerr << "FAIL: tree is not height-balanced\n";
        std::exit(1);
    }
}

int main() {
    // 示例 1、2
    check({-10, -3, 0, 5, 9});
    check({1, 3});

    // 边界：单元素
    check({1});

    // 满二叉树规模（7 个元素）
    check({1, 2, 3, 4, 5, 6, 7});

    // 偶数个元素
    check({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});

    // 负数与边界值
    check({-10000, -5000, 0, 5000, 10000});

    // 两个元素、三个元素
    check({-3, -1});
    check({2, 4, 6});

    std::cout << "All tests passed!" << std::endl;
    return 0;
}