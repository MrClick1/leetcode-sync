# 49. 字母异位词分组

状态：✅ 已解决（2026-09-06，排序作为 key 的解法通过 LeetCode）

## 题目要求

给定字符串数组 `strs`，将互为字母异位词的字符串分到同一组。字母异位词包含相同的字符及相同的出现次数，但字符顺序可以不同。

例如：

```text
eat、tea、ate  →  aet
tan、nat       →  ant
bat             →  abt
```

## 解法：排序后的字符串作为分组 key

对于每个字符串复制出一份 `key`，将 `key` 排序。互为字母异位词的字符串排序后一定相同，因此可以使用：

```cpp
unordered_map<string, vector<string>> groups;
```

- key：排序后的字符串
- value：所有属于该分组的原字符串

```cpp
class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        int n = static_cast<int>(strs.size());
        unordered_map<string, vector<string>> groups;

        for (int i = 0; i < n; i++) {
            string s = strs[i];
            string key = s;
            sort(key.begin(), key.end());
            groups[key].push_back(s);
        }

        vector<vector<string>> res;
        for (const auto& [key, group] : groups) {
            res.push_back(group);
        }
        return res;
    }
};
```

## 正确性要点

1. 两个字符串是字母异位词，当且仅当它们包含的字符及出现次数完全相同。
2. 对字符串排序后，字符及次数相同的字符串会得到同一个结果。
3. 因此相同排序 key 的字符串应当放进同一个 `vector`。
4. 遍历哈希表收集所有 value，即得到全部分组。哈希表顺序不固定，但题目允许任意顺序返回。

## 复杂度

设共有 `n` 个字符串，每个字符串平均长度为 `k`：

- 时间复杂度：`O(n * k log k)`，主要开销是逐个字符串排序。
- 额外空间复杂度：`O(n * k)`，包括分组结果、key 和字符串存储。

进阶方案可以统计每个字符串 26 个小写字母的出现次数，并将计数序列作为 key，从而把时间复杂度降为 `O(n * k)`。

## 本轮遇到的 C++ 问题

### `string::copy` 不是无参复制

错误写法：

```cpp
string key = s.copy();
```

`copy` 用于把字符串内容复制到外部字符数组，需要目标地址、复制长度等参数。复制 `string` 直接使用：

```cpp
string key = s;
```

### `std::sort` 原地修改且没有返回值

错误写法：

```cpp
string key = sort(s);
```

正确写法是先复制，再传入迭代器范围：

```cpp
string key = s;
sort(key.begin(), key.end());
```

### 结构化绑定应优先使用引用

```cpp
for (const auto& [key, group] : groups) {
    res.push_back(group);
}
```

`const auto&` 避免循环时复制哈希表中的 key 和整个 `vector<string>`。`const auto [key, group]` 语法本身可以工作，但会产生不必要的拷贝。

### `.` 与 `->` 的区别

```cpp
for (const auto& entry : groups) {
    res.push_back(entry.second);  // entry 是对象，使用 .
}
```

只有真正的迭代器或指针才使用 `->`：

```cpp
auto it = groups.begin();
res.push_back(it->second);
```

## 最终复盘

用户最终代码使用排序后的字符串分组，移除了未使用的 `find`，并使用 `const auto&` 结构化绑定遍历结果。LeetCode 提交已通过；重复字符串、空字符串和单字符字符串均由哈希表自然处理。
