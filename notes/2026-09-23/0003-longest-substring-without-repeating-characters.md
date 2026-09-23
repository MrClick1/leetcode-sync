# 3. 无重复字符的最长子串（二刷）

状态：2026-09-23 用户给出二刷代码，已复核逻辑正确；本轮未单独报告 LeetCode 提交结果。初刷记录见 [2026-09-14 笔记](../2026-09-14/0003-longest-substring-without-repeating-characters.md)。

## 二刷代码

```cpp
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        int n = (int)s.size();
        int res = 0;
        int left = 0;
        int right = 0;
        unordered_set<char> uset;  // 当前窗口中的字符

        while (right < n) {
            while (uset.count(s[right]) && left <= right) {
                uset.erase(s[left]);
                ++left;
            }

            uset.insert(s[right]);
            res = max(res, right - left + 1);
            ++right;
        }

        return res;
    }
};
```

## 窗口不变量与执行顺序

每轮开始时，`uset` 恰好保存闭区间 `[left, right-1]` 的字符，且其中没有重复。`s[right]` 是准备加入的字符。

如果它已在集合里，先从左边逐个删除，直到原来的同字符也被删掉。然后加入 `s[right]`，此时 `[left, right]` 再次无重复，可以安全更新最大长度。最后 `right++`，进入下一轮。

例如 `s = "abba"`：准备加入第二个 `b` 时，当前窗口是 `"ab"`，先删 `a`、再删旧 `b`，留下空窗口；加入新 `b` 后继续处理末尾的 `a`，最终答案为 2。

内层必须用 `while`：重复字符未必位于窗口左端，只删一次可能仍重复。

## 可选简化

`left <= right` 在内层条件中不是必需的。每轮开始有 `left <= right`；只要 `uset.count(s[right])` 仍为真，就说明旧的同字符仍在 `[left, right-1]`，收缩不会越过 `right`。当 `left == right` 时窗口为空，集合也为空，条件自然为假。

```cpp
while (uset.count(s[right])) {
    uset.erase(s[left]);
    ++left;
}
```

保留原条件不会影响本题结果；`s[right]` 的下标由外层 `while (right < n)` 保证有效。

## 边界与复杂度

- 空字符串：外层循环不执行，返回 0。
- 全部相同如 `"bbbbb"`：每次先移除旧字符，答案为 1。
- 全部不同：左边界不动，答案在每次加入右字符后更新，包括到达字符串末尾时。
- `"abcabcbb"` 和 `"pwwkew"`：答案均为 3。

`left`、`right` 都只向右移动，每个字符最多被插入和删除各一次；`unordered_set` 操作平均 O(1)，因此平均时间 O(n)。集合最多保存当前窗口的不同字符，空间 O(min(n, 字符集大小))。本轮依据代码审阅复核，未另建本地测试框架。
