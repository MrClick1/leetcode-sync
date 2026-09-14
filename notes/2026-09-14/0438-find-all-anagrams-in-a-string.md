# 438. 找到字符串中所有字母异位词

## 状态

✅ 已解决

## 核心思路

固定长度滑动窗口，窗口长度始终等于 `p.size()`。

使用长度为 26 的数组 `diff`：

```text
diff[c] = 当前窗口中字符 c 的出现次数 - p 中字符 c 的出现次数
```

当所有 `diff[c] == 0` 时，当前窗口与 `p` 的字符频次完全一致，因此当前窗口是 `p` 的异位词。

为了避免每次窗口滑动都重新扫描 26 个字符，再维护：

```text
differ = 当前 diff 中非 0 项的个数
```

因此：

```text
differ == 0
```

就表示当前窗口是异位词。

## 初始化

如果 `s.size() < p.size()`，直接返回空结果，否则初始化第一个长度为 `p.size()` 的窗口：

```cpp
for (int i = 0; i < (int)p.size(); i++) {
    diff[p[i] - 'a']--;
}

for (int i = 0; i < (int)p.size(); i++) {
    diff[s[i] - 'a']++;
}
```

然后扫描 26 个位置统计 `differ`。

## 窗口滑动

每次窗口向右移动一格时只发生两件事：

```text
移出 s[left]
加入 s[right]
```

### 移出左端字符

`diff[idx]--` 前后，只关心它是否跨过 0：

```cpp
if (diff[idx] == 0) {
    differ++;
}

diff[idx]--;

if (diff[idx] == 0) {
    differ--;
}
```

含义：

- `0 -> -1`：原本相等，现在出现差异，`differ++`。
- `1 -> 0`：原本有差异，现在消除差异，`differ--`。
- 其他非零到非零的变化不影响 `differ`。

随后必须：

```cpp
++left;
```

否则会重复移除同一个字符，窗口不再保持固定长度。本题调试时曾漏掉这一步，导致示例 `s = "cbaebabacd", p = "abc"` 只能找到下标 `0`，后面的 `6` 无法识别。

### 加入右端字符

同理，执行 `diff[idx]++` 前后只关心是否跨过 0：

```cpp
if (diff[idx] == 0) {
    differ++;
}

diff[idx]++;

if (diff[idx] == 0) {
    differ--;
}
```

## 最终代码

```cpp
class Solution {
public:
    vector<int> findAnagrams(string s, string p) {
        vector<int> diff(26, 0);
        int differ = 0;
        vector<int> res;

        if (s.size() < p.size()) return res;

        for (int i = 0; i < (int)p.size(); i++) {
            diff[p[i] - 'a']--;
        }

        for (int i = 0; i < (int)p.size(); i++) {
            diff[s[i] - 'a']++;
        }

        for (int i = 0; i < 26; ++i) {
            if (diff[i] != 0) differ++;
        }

        if (differ == 0) res.push_back(0);

        int left = 0;
        int right = p.size();

        for (; right < (int)s.size(); ++right) {
            int idx = s[left] - 'a';

            if (diff[idx] == 0) differ++;
            diff[idx]--;
            if (diff[idx] == 0) differ--;

            ++left;

            idx = s[right] - 'a';

            if (diff[idx] == 0) differ++;
            diff[idx]++;
            if (diff[idx] == 0) differ--;

            if (differ == 0) {
                res.push_back(left);
            }
        }

        return res;
    }
};
```

## 易错点

1. `s.size() < p.size()` 时初始化首窗口会越界，必须提前返回。
2. `right` 从 `p.size()` 开始，因为 `[0, p.size()-1]` 已经作为第一个窗口处理。
3. 移出 `s[left]` 后必须执行 `left++`。
4. `differ` 只关心 `diff[idx]` 是否为 0，不关心非零值具体是 `1`、`-1` 还是更大的数。
5. `diff` 的含义必须始终保持为“窗口频次 - p 频次”。

## 复杂度

- 时间复杂度：`O(|s| + |p| + 26)`，可视为 `O(|s| + |p|)`。
- 空间复杂度：`O(1)`，因为仅使用固定长度 26 的数组。

## 复习关键词

固定长度滑动窗口、字符频次差、`diff`、`differ`、`0 -> 非0`、`非0 -> 0`、左右边界同步移动。
