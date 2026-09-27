# 84. 柱状图中最大的矩形（二刷）

状态：2026-09-27 用户确认距离数组版本提交通过，代码审阅正确。本轮在 LeetCode 网页端练习，未重新运行本地测试。初刷见 [2026-09-03 笔记](../2026-09-03/0084-largest-rectangle-in-histogram.md)。

## 本次采用的思路

枚举每根柱子作为矩形高度，找左右第一个严格更矮的柱子作为阻挡边界。左右都可以同时扩展，得到以这根柱子为高度的最大矩形。

本轮保留与每日温度相似的写法：当前柱子比栈顶矮时，为被弹出的旧柱子确定一个方向的答案。

- 从左向右：当前 i 是被弹出的 idx 的右边界，记录 rightDist[idx] = i - idx。
- 从右向左：当前 i 是被弹出的 idx 的左边界，记录 leftDist[idx] = idx - i。
- 每一趟栈内对应高度从栈底到栈顶单调不减，允许相等。它不是“最大栈”。
- 相等高度不能成为严格更矮的边界，所以只在 heights[i] < heights[stk.top()] 时弹栈。
- 每个下标在各自一趟中恰好入栈一次，最多弹出一次；未弹出的保留默认边界距离。

## 距离定义与面积

leftDist[i] 是 i 到左侧更矮边界 L 的距离 i-L；rightDist[i] 是 i 到右侧更矮边界 R 的距离 R-i。它们不是边界下标。

无更矮柱子时，虚拟左边界为 -1，右边界为 n，所以默认初始化为：

```cpp
leftDist[i] = i + 1;
rightDist[i] = n - i;
```

虚拟边界只参与距离计算，不访问数组。实际矩形范围为 [L+1, R-1]：

```cpp
int width = leftDist[i] + rightDist[i] - 1;
int area = heights[i] * width;
```

用户最终写法也完全正确：

```cpp
int leftArea = heights[i] * left[i];
int rightArea = heights[i] * right[i];
int maxArea = leftArea + rightArea - heights[i];
```

左右部分均包含当前柱子的一份宽度，相加后扣除重叠的 heights[i]。因式分解后就是 height * (leftDist + rightDist - 1)，先算宽度更容易看出矩形含义。

## 保留原算法的简化版本

只整理命名、循环起点和面积公式；使用两个独立空栈，避免预先入栈及额外的端点处理。

```cpp
class Solution {
public:
    int largestRectangleArea(vector<int>& heights) {
        int n = (int)heights.size();
        vector<int> leftDist(n), rightDist(n);

        for (int i = 0; i < n; ++i) {
            leftDist[i] = i + 1;
            rightDist[i] = n - i;
        }

        // 为被弹出的柱子确定右侧更矮边界的距离
        stack<int> rightStk;
        for (int i = 0; i < n; ++i) {
            while (!rightStk.empty() && heights[i] < heights[rightStk.top()]) {
                int idx = rightStk.top();
                rightStk.pop();
                rightDist[idx] = i - idx;
            }
            rightStk.push(i);
        }

        // 为被弹出的柱子确定左侧更矮边界的距离
        stack<int> leftStk;
        for (int i = n - 1; i >= 0; --i) {
            while (!leftStk.empty() && heights[i] < heights[leftStk.top()]) {
                int idx = leftStk.top();
                leftStk.pop();
                leftDist[idx] = idx - i;
            }
            leftStk.push(i);
        }

        int res = 0;
        for (int i = 0; i < n; ++i) {
            int width = leftDist[i] + rightDist[i] - 1;
            res = max(res, heights[i] * width);
        }
        return res;
    }
};
```

用户原版预先入栈 0 / n-1，然后从 1 / n-2 开始，也是正确的（题目保证 n >= 1）。整理版让所有柱子都在 for 内统一入栈，减少漏入栈或重复入栈的机会。

## 本轮错误复盘

1. 左右方向与变量混淆：从左往右为被弹出的旧柱子找到的是右边界；从右往左找到的是左边界。
2. 注释说存下标，实际存距离：应统一变量定义，不能把边界公式与距离公式混用。
3. 反向距离曾写为 i-topIdx，得到负数；应为 topIdx-i。
4. 预先 push(n-1) 后又从 n-1 开始循环，重复入栈。原结构应从 n-2 开始；统一空栈写法可以避免这个问题。
5. 默认全 0：单柱 [1] 两趟循环不执行，面积错误为 0。没有更矮柱子表示可扩展到数组边缘，默认距离应为 i+1、n-i。
6. 分别计算左右面积后取 max：漏掉跨越两侧的矩形。[2,1,2] 的中间柱子能覆盖三个位置，面积是 3，不是 2。

## 与初刷“为当前柱子查边界”写法的区别

| 写法 | 从左向右时计算谁的什么 | 弹栈条件 | 记录时机 |
|---|---|---|---|
| 本轮：为被弹出的柱子确定答案 | 旧柱子的右边界/距离 | 当前高度 < 栈顶高度 | 每次弹栈时 |
| 初刷：为当前柱子寻找答案 | 当前柱子的左边界 | 栈顶高度 >= 当前高度 | 弹栈结束后读新栈顶 |

两种都能找到严格更矮的边界，但相等高度的处理不同。不要单独照搬比较符号；先说明“谁获得答案，答案是哪一侧”，再决定条件。

## 边界复核与复杂度

- [1]：两侧默认距离都是 1，宽度为 1，面积为 1。
- [2,4]：两根柱子的候选面积均为 4。
- [2,1,2]：leftDist=[1,2,1]，rightDist=[1,2,1]，中间面积为 3。
- [2,2,2]：相等不弹栈，默认距离使每根都能覆盖全数组，面积为 6。
- 高度为 0 的柱子贡献面积 0，同时能够截断更高柱子的扩展。

时间 O(n)，辅助空间 O(n)。两个栈与两个数组不会改变复杂度等级。最大矩形面积不超过 100000 * 10000 = 10^9，32 位 int 足够；原版左右面积相加也不超过 10000 * 100001，在范围内。

当前优先掌握距离定义和两趟弹栈写法，无需急着切换一次扫描的面积结算版本。
