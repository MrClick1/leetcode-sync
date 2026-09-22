# 189. 轮转数组

状态：用户确认 LeetCode 提交通过（2026-09-22），代码复核正确。已完成三次反转的 O(1) 空间解法。

## 用户通过的代码

```cpp
class Solution {
public:
    void rotate(vector<int>& nums, int k) {
        int n = (int)nums.size();

        reverse(nums.begin(), nums.end());

        if (k >= n) {
            k = k % n;
        }

        reverse(nums.begin(), nums.begin() + k);
        reverse(nums.begin() + k, nums.end());
    }
};
```

题目保证 n >= 1，因此取模合法。先整体反转再取模也正确：整体反转不依赖 k，只要在计算 begin() + k 前归一化即可。

## 为什么三次反转可以右移

令 k 为取模后的步数。把原数组分为 A 和 B，其中 B 是最后 k 个元素，目标是把 A B 变成 B A，同时保持两段内部顺序。

```text
原数组：          A B
整体反转：        reverse(B) reverse(A)
反转前 k 个：     B reverse(A)
反转剩下的：      B A
```

例如 n = 7、k = 3：

```text
原数组：       [1,2,3,4,5,6,7]
整体反转：     [7,6,5,4,3,2,1]
反转前 3 个：  [5,6,7,4,3,2,1]
反转后 4 个：  [5,6,7,1,2,3,4]
```

整体反转完成两段的位置交换，分段反转恢复每一段内部的顺序。

## 取模与半开区间

右移 n 次回到原数组，因此只需右移 k % n 次。可以直接写 `k %= n;`：k < n 时结果本来就是 k，不必额外判断。

`reverse(first, last)` 处理 `[first, last)`，不包含 last：

- `[begin(), begin() + k)`：前 k 个元素，下标 0 到 k-1。
- `[begin() + k, end())`：剩余元素，下标 k 到 n-1。

k 为 0 时第一段为空，reverse 空区间合法；最后一次整体反转会撤销第一次反转，因此用户代码依然正确。

## 可选简化

```cpp
int n = (int)nums.size();
k %= n;
if (k == 0) return;

reverse(nums.begin(), nums.end());
reverse(nums.begin(), nums.begin() + k);
reverse(nums.begin() + k, nums.end());
```

把取模放前面，可以在无需轮转时直接返回。属于可选优化，原代码不需要修正。

## 边界复习与复杂度

| 输入 | 结果 | 关注点 |
|---|---|---|
| `[1,2,3,4,5,6,7]`, k=3 | `[5,6,7,1,2,3,4]` | 两段内部顺序恢复 |
| `[-1,-100,3,99]`, k=2 | `[3,99,-1,-100]` | 数值正负不影响算法 |
| `[1,2,3]`, k=0 | `[1,2,3]` | 空区间反转合法 |
| `[1,2,3]`, k=3 | `[1,2,3]` | 一整圈 |
| `[1,2,3]`, k=4 | `[3,1,2]` | 取模后右移一步 |
| `[7]`, k=100000 | `[7]` | 单元素 |

三次反转处理的元素数量总计 n + k + (n-k) = 2n，因此时间 O(n)，额外空间 O(1)。

三次反转的验证为代码审阅及用户报告的 LeetCode 通过；上述用例用于复习，未另建本地测试框架。额外数组法尚未展开学习；环状替换的学习记录见下文，尚未收到用户独立实现通过的反馈。

记忆：步数先取模，整体翻一次，前 k 和剩余各翻一次。

## 环状替换：放下一个数字，接住一个数字

原来下标 i 的元素应该移动到 `(i + k) % n`。直接覆盖会丢失目标位置原来的元素，因此用独立变量 prev 保存它，再继续运送。

```cpp
int next = (current + k) % n;
swap(nums[next], prev);
current = next;
```

- current：手里这个数字原来所在的下标。
- prev：手里保存、等待放到目标位置的数字。
- next：目标下标。
- start：当前环的起点，返回这里时结束本环。

nums = [1,2,3,4,5,6]、k = 2，从 start = 0、prev = 1 开始：

| current | next | 放入的数 | 接到 prev 的数 | 数组 |
|---|---|---|---|---|
| 0 | 2 | 1 | 3 | [1,2,1,4,5,6] |
| 2 | 4 | 3 | 5 | [1,2,1,4,3,6] |
| 4 | 0 | 5 | 1 | [5,2,1,4,3,6] |

此时回到起点 0，下标 0、2、4 已就位。再从 1 开始处理环 1→3→5→1，得到 [5,6,1,2,3,4]。

`int prev = nums[start]` 是复制，不是引用。swap 明确修改 nums[next] 和 prev，既完成目标位置的更新，也保存被替换的数；不会因为 prev 最初来自 nums[start] 就自动修改起点。详细语法见 [C++ 语法笔记](../01-cpp语法知识文档.md)。

## 为什么环数是 gcd(n, k)

gcd 是最大公约数。设 g = gcd(n,k)，每个环包含 n/g 个位置，一共有 g 个环。

当 k > 0 时，走 b 步第一次回到起点意味着 b*k 是 n 的倍数。写成 n = g*n'、k = g*k'，n' 与 k' 互质，则最小正整数 b 为 n' = n/g。下标每次增加 k，不改变除以 g 的余数，每个余数类恰好有 n/g 个位置，所以从 0 到 g-1 开始可覆盖全部环。

例子：n=6,k=2 有 2 个环；n=6,k=3 有 3 个环；n=7,k=2 有 1 个环。k=0 时 gcd(n,0)=n，每个位置是一个自身环，也可以直接返回。

## 环状替换参考实现

```cpp
// C++17，std::gcd 需要 <numeric>，std::swap 可包含 <utility>
class Solution {
public:
    void rotate(vector<int>& nums, int k) {
        int n = (int)nums.size();
        k %= n;
        if (k == 0) return;
        int cycles = std::gcd(n, k);
        for (int start = 0; start < cycles; ++start) {
            int current = start;
            int prev = nums[start];
            do {
                int next = (current + k) % n;
                std::swap(nums[next], prev);
                current = next;
            } while (current != start);
        }
    }
};
```

初始化时 current == start，所以使用 do...while，先移动一次，再判断是否回到起点。若直接写 while(current != start)，一次都不会执行。

每个位置恰好接收一次最终元素，总时间 O(n)，额外空间 O(1)。也可以累计已经完成的替换次数 moved，从 start=0 起逐环处理，moved==n 时结束，避免显式计算 gcd。
