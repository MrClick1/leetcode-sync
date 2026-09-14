# 42. 接雨水

状态：⚠️ 学习中（左右最大值解法已理解；单调栈解法待巩固）

## 当前学习情况

- 已参考并理解“预处理左右最大值”解法。
- 已阅读单调栈解法，代码能够跟着题解写出，但对弹栈时三个下标的角色以及“按层计算积水”仍不够清晰。
- 本次先记录，不要求立刻掌握；后续复习时重点手动模拟单调栈。

## 解法一：预处理左右最大值

对每个位置 `i`，分别记录：

- `leftMax[i]`：区间 `[0, i]` 中的最大高度。
- `rightMax[i]`：区间 `[i, n - 1]` 中的最大高度。

当前位置可以接到的水量为：

```cpp
min(leftMax[i], rightMax[i]) - height[i]
```

水面高度由左右最高柱子中较矮的一根决定。

```cpp
class Solution {
public:
    int trap(vector<int>& height) {
        int n = static_cast<int>(height.size());
        vector<int> leftMax(n);
        vector<int> rightMax(n);

        leftMax[0] = height[0];
        for (int i = 1; i < n; ++i) {
            leftMax[i] = max(leftMax[i - 1], height[i]);
        }

        rightMax[n - 1] = height[n - 1];
        for (int i = n - 2; i >= 0; --i) {
            rightMax[i] = max(rightMax[i + 1], height[i]);
        }

        int res = 0;
        for (int i = 0; i < n; ++i) {
            res += min(leftMax[i], rightMax[i]) - height[i];
        }
        return res;
    }
};
```

复杂度：时间 `O(n)`，额外空间 `O(n)`。

## 解法二：单调栈（待巩固）

栈中保存的是下标，对应高度从栈底到栈顶单调不增。当前柱子比栈顶更高时，说明找到了可能的右边界，需要不断弹栈。

每次弹栈后的三个角色：

```text
新栈顶 left       弹出的 bottom       当前下标 right
   左边界               槽底                右边界
```

计算公式：

```cpp
int width = right - left - 1;
int waterHeight = min(height[left], height[right]) - height[bottom];
res += width * waterHeight;
```

必须注意：

1. `bottom` 弹出后，新的栈顶才是左边界。
2. 如果弹出后栈为空，说明缺少左边界，本次不能接水。
3. 内层使用 `while`，因为同一根右边界可能让多个较矮柱子依次出栈。
4. 每次弹栈计算的是一层横向积水，不是某一列的全部积水。

```cpp
class Solution {
public:
    int trap(vector<int>& height) {
        int res = 0;
        stack<int> stk;

        for (int right = 0;
             right < static_cast<int>(height.size());
             ++right) {
            while (!stk.empty() &&
                   height[right] > height[stk.top()]) {
                int bottom = stk.top();
                stk.pop();

                if (stk.empty()) {
                    break;
                }

                int left = stk.top();
                int width = right - left - 1;
                int waterHeight =
                    min(height[left], height[right]) - height[bottom];

                res += width * waterHeight;
            }

            stk.push(right);
        }

        return res;
    }
};
```

复杂度：时间 `O(n)`，额外空间 `O(n)`。虽然存在嵌套 `while`，但每个下标最多入栈一次、出栈一次。

## 示例中的关键弹栈

```text
height = [0,1,0,2,1,0,1,3,2,1,2,1]
```

- `right = 3`：弹出下标 `2`，增加 `1` 格水。
- `right = 6`：弹出下标 `5`，增加 `1` 格水。
- `right = 7`：弹出下标 `6` 时增加 `0`；再弹出下标 `4`，增加 `3` 格水。
- `right = 10`：弹出下标 `9`，增加 `1` 格水。
- 总水量为 `1 + 1 + 3 + 1 = 6`。

## 后续复习任务

不要先背完整代码，先完成下面三步：

1. 画出下标 `3、4、5、6、7` 对应的高度 `2、1、0、1、3`。
2. 手动模拟 `right = 7` 时依次弹出 `6、4、3` 的过程。
3. 每次弹栈都说出谁是 `left`、谁是 `bottom`、谁是 `right`，以及为什么有时水层高度为 `0`。

## 当前记忆句

```text
当前柱子是右墙，弹出的柱子是槽底，弹出后的栈顶是左墙；每次弹栈计算一层水。
```

这句话目前仍比较抽象，后续需要配合画图和手动模拟继续理解。
