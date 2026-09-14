# 3. 无重复字符的最长子串

状态：✅ 已解决（2026-09-14，LeetCode 提交通过）

## 一句话认识这道题

维护一个不含重复字符的连续窗口：右指针负责加入字符；新字符重复时，左指针不断删除字符，直到窗口重新合法。

## 窗口定义

当前窗口使用闭区间：

```text
s[left...right]
```

完成每轮处理后，窗口始终满足：

```text
s[left...right] 中没有重复字符
```

`unordered_set<char> window` 记录当前窗口中已经出现的字符。因为窗口始终无重复，只需要判断字符是否存在，不需要记录频次。

## 最终通过代码

```cpp
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        int n = static_cast<int>(s.size());
        unordered_set<char> window;
        int res = 0;

        int left = 0;
        int right = 0;

        for (; right < n; ++right) {
            // s[right] 已经存在，收缩左边界
            while (window.count(s[right])) {
                window.erase(s[left]);
                ++left;
            }

            // 收缩后，s[right] 可以进入窗口
            window.insert(s[right]);

            // 当前闭区间 [left, right] 没有重复字符
            res = max(res, right - left + 1);
        }

        return res;
    }
};
```

也可以直接在 `for` 初始化部分声明右指针：

```cpp
for (int right = 0; right < n; ++right) {
    // ...
}
```

## 为什么收缩必须使用 `while`

重复字符不一定正好位于窗口最左端。例如 `s = "abba"`，处理第二个 `'b'` 时，先删除 `'a'` 后窗口中仍有旧的 `'b'`，还要继续删除一次。

```cpp
while (window.count(s[right])) {
    window.erase(s[left]);
    ++left;
}
```

如果写成 `if`，只删除一个字符后窗口可能仍然重复。

## 本轮出现的问题

最初代码混用了两种滑动窗口模板：

```cpp
int right = 0;
window.count(s[right + 1]);
```

这里存在三个问题：

1. `right = 0` 却访问 `s[right + 1]`，会跳过 `s[0]`。
2. 当 `right == n - 1` 时访问 `s[n]`，发生越界和未定义行为。
3. 只在遇到重复时更新 `res`，完全不重复或最长窗口到达末尾时无法记录答案。

最终版本统一采用“`right` 指向当前准备加入的字符”这一语义，因此始终访问 `s[right]`，并在窗口恢复合法、加入字符后更新答案。

## 与官方解法的区别

官方代码使用：

```cpp
int rk = -1;
```

然后外层枚举左边界 `i`，内层检查 `s[rk + 1]` 并尽量扩展右边界。`occ` 是 `occur/occurrence` 的缩写，表示当前窗口中已经出现的字符集合。

两种写法本质相同：

- 官方版本：外层移动左边界，右边界尽量扩展。
- 用户版本：外层移动右边界，出现重复时收缩左边界。

用户版本的窗口移动顺序更符合下面的通用滑动窗口模板：

```text
右端加入候选 → 不合法时收缩左端 → 恢复合法 → 更新答案
```

## 为什么时间复杂度是 O(n)

虽然存在 `for` 和内层 `while`，但：

- 每个字符最多被右指针加入窗口一次。
- 每个字符最多被左指针删除一次。
- `left` 和 `right` 都只向右移动，不会回退。

所以全部移动次数是线性的，平均时间复杂度为 `O(n)`。

## 边界用例

```text
""        -> 0
"a"       -> 1
"bbbbb"   -> 1
"abcabcbb"-> 3
"pwwkew"  -> 3
"abba"    -> 2
```

`"pwke"` 不是答案，因为它不是原字符串中的连续子串。

## 复杂度

- 平均时间复杂度：`O(n)`。
- 额外空间复杂度：`O(k)`，`k` 是窗口中不同字符的数量；在有限字符集下也可以看作常数上界。

## 记忆口诀

```text
右边不断进；
重复左边退；
退到不重复；
再更新长度。
```
