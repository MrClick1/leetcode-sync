# 76. 最小覆盖子串

状态：✅ 已解决（LeetCode 全部测试通过）

## 学习结果

- 掌握使用 `need` 保存目标需求、使用 `window` 保存当前窗口计数
- 掌握用 `required` 和 `formed` 判断包含重复字符的需求是否全部满足
- 掌握“右端扩张、合法时记录答案、左端收缩到重新不合法”的可变长度滑动窗口
- 修正了哈希表 key 混用、左指针未移动和无解时 `substr(-1, ...)` 等问题

## 当前思路：可变长度滑动窗口

- 用 `need` 统计 `t` 中每种字符需要多少个
- 用 `window` 统计当前窗口中相应字符的数量
- 右指针不断扩张窗口，直到窗口满足全部需求
- 窗口满足后，记录当前答案，并不断右移左指针以寻找更短的合法窗口
- 一旦移除某个必要字符导致数量不足，就停止收缩，继续移动右指针

## 判断窗口是否满足

- `required`：`t` 中不同字符的种类数
- `formed`：当前窗口中，数量已经达到需求的字符种类数
- 当某字符的窗口数量第一次达到 `need` 要求时，`formed++`
- 当收缩窗口导致某字符从“刚好满足”变为“不足”时，`formed--`
- `formed == required` 表示当前窗口包含了 `t` 的全部字符和重复次数

## C++ 语法记录：哈希表 key 的数量

完整说明见：[C++ 语法与标准库速查（个人版）](../00-cpp语法知识文档.md)。

```cpp
unordered_map<char, int> need;
for (char c : t) {
    need[c]++;
}
int required = static_cast<int>(need.size());
```

- `t.size()` 是 `t` 的字符总数，包含重复字符
- `need.size()` 是哈希表中不同 key 的数量，也就是不同字符种类数
- 例如 `t = "AABC"` 时，`t.size() == 4`，`need.size() == 3`
- `need[c]` 在 key 不存在时会自动插入该 key；遍历 `s` 时应先用 `need.count(c)` 或 `need.find(c) != need.end()` 判断它是否属于目标字符，避免把无关字符加入 `need`

## 待继续讨论

- 结合 `ADOBECODEBANC` 手动模拟左右指针
- 明确扩张、记录答案、收缩三个阶段的代码顺序
- 实现后检查重复字符、无解和单字符边界

## 第一版代码 review

1. `right` 在循环中没有递增；只要第一个字符不能让窗口立即满足，程序就会死循环
2. `need[rightC] - 1` 只是计算表达式，没有修改任何计数；当前代码没有记录窗口内字符实际出现了多少次
3. `cCount` 表示未满足的字符种类，但每次遇到目标字符都可能重复减一。例如 `t = "ABC"`、`s = "AAA"` 会把同一个 `A` 重复当成三种需求
4. 对重复需求无法生效。例如 `t = "AA"` 时，`need['A'] == 2`，但窗口计数从未累积到 2
5. `left` 从未移动；找到第一个合法窗口后立即 `break`，因此没有执行“收缩窗口”和“继续寻找更短答案”
6. 没有保存历史最优窗口的起点与长度
7. 没有处理无解情况，最终会直接返回一个不一定满足要求的子串
8. `need.size()` 返回 `size_t`，赋给 `int` 时建议显式使用 `static_cast<int>`

下一版应保留 `need` 作为固定需求，并新增 `window` 统计当前窗口；右端加入字符后更新 `formed`，当 `formed == required` 时用内层循环更新答案并收缩左端。

## 第二版代码 review

这一版已经加入 `window` 并让 `right` 正常移动，但还需要修正以下问题：

1. `window[left]` 和 `need[left]` 把数组下标 `left` 当成了字符 key。应该先写 `char leftC = s[left]`，再访问 `window[leftC]` 和 `need[leftC]`
2. 当前代码先执行 `left++`，再执行 `window[left]--`，会减掉新位置对应的计数；正确顺序是先减少旧的 `leftC` 的计数，再移动 `left`
3. `need[rightC]` 会在 `rightC` 不属于 `t` 时向 `need` 插入一个值为 0 的新 key。扩张和收缩时都应先使用 `need.find(c) != need.end()` 判断字符是否属于需求
4. 找到合法窗口后没有记录它的起点和长度；遍历结束时的 `left`、`right` 不一定对应全局最短窗口
5. 当前收缩逻辑只尝试删除“删掉以后依旧满足”的字符，因此 `cCount` 一旦变成 0 就不会恢复。这个思路可以实现，但控制逻辑不如标准写法清晰；建议在合法窗口中先记录答案，再实际移除左端字符，并在必要字符变为不足时令 `cCount++`
6. `return s.substr(left, right - left + 1)` 使用的是循环结束后的指针；此时 `right` 通常已经等于 `s.size()`，既有长度偏一问题，也不能处理无解。应通过 `bestStart`、`bestLen` 返回历史最优答案，无解时返回空字符串

标准收缩顺序：窗口合法时先更新最优答案；取得 `leftC = s[left]`；如果它是目标字符且删除前数量刚好等于需求，则恢复一个未满足种类；然后执行 `window[leftC]--` 和 `left++`。

## 第三版代码 review

- 已修正把 `left` 下标误当成字符 key 的问题：现在通过 `leftC = s[left]` 访问计数
- 目前的内层循环只删除无关字符或多余字符，不删除窗口中最后一个必要字符，因此窗口一旦合法就会一直保持合法；在这种实现里 `cCount` 不恢复并不一定是错误，但必须在每次收缩完成后记录当前候选答案
- 仍需增加 `bestStart` 和 `bestLen`。循环结束后的左右指针只表示最后处理的窗口，不能代表遍历期间的全局最短窗口
- `need[rightC]`、`need[leftC]` 仍会为非目标字符插入值为 0 的 key。应使用 `need.find(c)` 或 `need.count(c)` 区分目标字符
- 如果始终没有合法窗口，必须通过 `bestStart == -1` 返回空字符串，不能直接返回循环结束后的子串
- 外层循环结束后 `right == s.size()`，因此 `right - left + 1` 也不再是当前闭区间 `[left, right]` 的长度

若继续采用“只删除多余字符、始终维持合法窗口”的方案，应在 `cCount == 0` 时：先尽量收缩左端，再用 `right - left + 1` 更新历史最优答案。对于左端非目标字符可以直接删除；对于目标字符，只有 `window[leftC] > need[leftC]` 时才能删除。

## 推荐解法：标准可变长度滑动窗口

使用 `need` 保存固定需求，`window` 保存当前窗口中的目标字符数量：

- `required = need.size()`：一共需要满足多少种不同字符
- `formed`：当前已经满足数量要求的字符种类数
- 右端加入字符后，只有当 `window[c]` 第一次等于 `need[c]` 时才执行 `formed++`
- 当 `formed == required` 时窗口合法，先更新最短答案，再不断删除左端字符
- 删除目标字符前，如果 `window[c] == need[c]`，说明删除后会从“刚好满足”变为“不足”，执行 `formed--`

```cpp
class Solution {
public:
    string minWindow(string s, string t) {
        unordered_map<char, int> need;
        unordered_map<char, int> window;

        for (char c : t) {
            need[c]++;
        }

        int required = static_cast<int>(need.size());
        int formed = 0;
        int left = 0;
        int bestStart = -1;
        int bestLen = INT_MAX;

        for (int right = 0; right < static_cast<int>(s.size()); right++) {
            char rightC = s[right];

            if (need.count(rightC)) {
                window[rightC]++;
                if (window[rightC] == need[rightC]) {
                    formed++;
                }
            }

            while (formed == required) {
                int currentLen = right - left + 1;
                if (currentLen < bestLen) {
                    bestLen = currentLen;
                    bestStart = left;
                }

                char leftC = s[left];
                if (need.count(leftC)) {
                    if (window[leftC] == need[leftC]) {
                        formed--;
                    }
                    window[leftC]--;
                }
                left++;
            }
        }

        return bestStart == -1 ? "" : s.substr(bestStart, bestLen);
    }
};
```

### 为什么相等时才更新 `formed`

假设 `need['A'] == 2`：窗口中第一个 `A` 还不够；第二个 `A` 使该字符刚好满足，因此 `formed++`；第三个 `A` 只是多余字符，不能再次增加 `formed`。

收缩时同理：若删除前 `window['A'] == need['A']`，删除后必然不足，所以先执行 `formed--`；若删除前数量大于需求，删掉一个后仍满足，不改变 `formed`。

### 复杂度

左右指针都只会从左向右经过 `s` 一次，因此时间复杂度是 `O(|s| + |t|)`；哈希表空间与字符种类数有关。题目限定为英文字母时，字符种类有固定上限，也可以视为常数空间。

## 易错点：能否把右端更新拆成两个 `if`

可以拆开，但第二个 `if` 仍然必须判断 `rightC` 是目标字符：

```cpp
if (need.count(rightC)) {
    window[rightC]++;
}

if (need.count(rightC) && window[rightC] == need[rightC]) {
    formed++;
}
```

如果第二个判断直接写成 `window[rightC] == need[rightC]`，当 `rightC` 不属于目标时，两次 `operator[]` 都可能插入默认值 0，条件会错误地得到 `0 == 0`。

另外，`window[rightC] == need[rightC]` 只说明当前这一种字符刚好满足，不能说明整个窗口满足。例如 `t = "ABC"`、当前只读到一个 `A` 时，`A` 已满足但 `B`、`C` 仍缺失。因此这里应该执行 `formed++`；只有 `formed == required` 时才进入收缩循环并更新最短答案。

更新最优答案也不能写成 `min(right - left + 1)`，因为 `std::min` 需要两个参数，并且只有新窗口更短时才能同时更新长度和起点：

```cpp
int currentLen = right - left + 1;
if (currentLen < bestLen) {
    bestLen = currentLen;
    bestStart = left;
}
```

## 运行时错误：`substr` 的位置变成超大整数

异常中的 `18446744073709551615` 是 64 位无符号整数的最大值。它通常来自把有符号整数 `-1` 转换为 `size_t`：当没有找到合法窗口时，`bestStart` 仍为 `-1`，而 `string::substr` 的位置参数是无符号的 `size_t`，所以 `s.substr(bestStart, bestLen)` 会把 `-1` 转成这个超大数并抛出 `std::out_of_range`。

返回前必须处理无解情况：

```cpp
if (bestStart == -1) {
    return "";
}
return s.substr(bestStart, bestLen);
```

这版收缩代码还有两个错误：

1. `window[left]--` 把整数下标当成了 `char` 类型的 key，应改为 `window[leftC]--`
2. 每轮收缩结束都必须执行 `left++`。如果左端是无关字符，原代码既不改变 `formed` 也不移动 `left`，内层循环会一直处理同一个位置

正确的收缩尾部是：

```cpp
char leftC = s[left];
if (need.count(leftC)) {
    if (window[leftC] == need[leftC]) {
        formed--;
    }
    window[leftC]--;
}
left++;
```

## 最终解答检查

- 最终代码已在 LeetCode 通过全部测试
- `need` 始终保存固定需求，只对目标字符维护 `window`
- 右端字符数量第一次达到需求时执行 `formed++`，不会把多余字符重复计入
- 窗口合法时先记录答案，再删除左端字符；删除必要字符前正确执行 `formed--`
- 使用 `bestStart == -1 ? "" : s.substr(bestStart, bestLen)` 正确处理无解情况
- 外层条件中的 `left <= right` 可以省略为 `right < s.size()`，但在本题非空输入和当前更新顺序下不影响正确性

时间复杂度：`O(|s| + |t|)`；空间复杂度：`O(字符种类数)`，英文字母字符集固定时可视为 `O(1)`。
