# 139 单词拆分

2026-10-02 用户在讨论后写出前缀长度 DP，并反馈力扣通过；自评需要对话帮助才能形成代码，独立编码仍待巩固。当前按约定继续 Hot 100 DP 首轮，不展开其他解法。

## 用户通过代码

```cpp
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        int n = (int)s.size();
        int m = (int)wordDict.size();

        vector<bool> dp(n+1, false);
        dp[0] = true;

        for (int i = 1; i <= n; ++i) {
            string str = s.substr(0, i);
            for (const auto& word : wordDict) {
                int len = (int)word.size();
                if (i >= len) {
                    // 当前字符串可以进行检查
                    string substring = s.substr(i - len, len);
                    if (substring == word && dp[i-len]) {
                        dp[i] = true;
                    }
                }
            }
        }
        return dp[n];
    }
};
```

## 从想法写成代码

dp[i] 表示 s 的前 i 个字符能否由字典单词拼成；i 是长度，不是最后一个字符的下标。该前缀对应下标 [0,i)，最后一个字符下标为 i-1。

假设最后一个单词为 word，长度为 len，则前面的长度是 i-len，末尾单词起始下标也为 i-len。先检查 i>=len，再检查前面 dp[i-len] 为真且后面 s.substr(i-len,len)==word。这里 substr 第二个参数是字符数量，不是结束下标。

| 自然语言 | 代码 |
|---|---|
| 考虑长度为 i 的前缀 | 外层 i=1..n |
| 尝试最后一个单词 | 内层遍历 wordDict |
| 前缀能放下该单词 | i>=len |
| 去掉最后单词，前面能拼成 | dp[i-len] |
| 最后一段确实是该单词 | s.substr(i-len,len)==word |
| 找到一种合法拼接 | dp[i]=true |

两项都满足时即可确认为真；某个单词不匹配不能把此前已经为真的状态覆盖成 false。若一个前缀可拆分，其最后一个单词必然在字典中，会被枚举到，因此此判断不漏方案。

len>=1，故 i-len<i；从短前缀到长前缀遍历，依赖的状态已经算完。dp[0]=true 表示空前缀可以作为拼接起点；第一个单词恰好覆盖整个前缀时需要它。每个前缀都会重新枚举完整字典，所以同一单词可以重复使用，不需要 used 标记。

## applepenapple 的关键状态

字典 [apple,pen]：

| i | 尝试的最后单词 | 前面状态 | 当前末尾 | 结果 |
|---|---|---|---|---|
| 5 | apple | dp[0]=true | apple | dp[5]=true |
| 8 | pen | dp[5]=true | pen | dp[8]=true |
| 13 | apple | dp[8]=true | apple | dp[13]=true |

不能只判断最后一段是否是单词。例如 s=xapple、字典 [apple]，末尾 apple 匹配，但前面的 x 不可拆分，dp[1]=false，所以 dp[6] 仍为 false。

## 可以简化的地方

原代码的 m 没有使用，可以删掉。str=s.substr(0,i) 也没有使用；前缀已由长度 i 表示，无需复制。找到可行方案后可以 break，省去其余尝试；原来不 break 仍然正确。

```cpp
if (i >= len && dp[i - len] && s.substr(i - len, len) == word) {
    dp[i] = true;
    break;
}
```

把 dp[i-len] 放在 substr 比较前，可以借助 && 短路在前面不可达时省去子串构造；用户原来的判断顺序仍然正确。

令 n=s.size()、m=wordDict.size()、L 为最长单词长度。原代码额外复制全部前缀，时间上界 O(n²+nmL)；删除无用 str 后为 O(nmL)，其中 substr 复制及比较的成本不能忽略。辅助空间原版 O(n+L)，简化后同样 O(n+L)，本题 L<=20。本轮仅静态检查，提交通过来自用户反馈，未重新编译执行。

## 独立编码复习

先口述“当前前缀=可拆分的前缀+一个字典单词”，再写 dp 含义。然后写出 len、i-len 和 substr 的两个参数，最后填上循环、dp[0] 与 return dp[n]。可以用 leetcode/[leet,code] 手写 dp[4] 和 dp[8]，再用 xapple/[apple] 检查是否遗漏前面必须可达的条件。
