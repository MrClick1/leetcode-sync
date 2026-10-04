# 5. 最长回文子串

2026-10-04 用户经多轮修改后，明确反馈二维区间 DP 解法力扣通过。重点复习状态含义、短区间基础情况、依赖决定的遍历顺序，以及循环中复制子串带来的时间开销；通过不等于已经能独立流畅写出。

## 用户通过代码

保留用户最终实现，包括仍然正确但冗余的首行初始化。

```cpp
class Solution {
public:
    bool isValid(const string& s) {
        int l = 0;
        int r = (int)s.size() - 1;

        while (l < r) {
            if (s[l] != s[r]) return false;
            ++l;
            --r;
        }
        return true;
    }

    string longestPalindrome(string s) {
        int n = (int)s.size();
        int startIdx = 0;
        int endIdx = 0;
        vector<vector<bool>> dp(n, vector<bool>(n, false));

        // 初始化
        for (int j = 0; j < n; ++j) {
            if (isValid(s.substr(0, j + 1))) {
                dp[0][j] = true;
            }
        }

        for (int i = n - 1; i >= 0; --i) {
            for (int j = i; j < n; ++j) {
                if (i == j) {
                    dp[i][j] = true;
                } else if (i + 1 == j) {
                    if (s[i] == s[j]) dp[i][j] = true;
                } else {
                    if (s[i] == s[j]) {
                        dp[i][j] = dp[i+1][j-1];
                    }
                }

                if (dp[i][j] == true && j - i > endIdx - startIdx) {
                    startIdx = i;
                    endIdx = j;
                }
            }
        }
        return s.substr(startIdx, endIdx - startIdx + 1);
    }
};
```

## 状态、基础情况与顺序

dp[i][j] 表示闭区间 s[i..j] 是否为回文串，保存布尔值。它不是这一段里的最长回文长度；最长答案需要另外保存起点与终点。

| 区间长度 | 判断方法 |
|---|---|
| 1，即 i==j | 单个字符一定是回文 |
| 2，即 j==i+1 | 两端字符相等即可 |
| 至少 3 | 两端相等，而且 dp[i+1][j-1] 为 true |

去掉相同的两端后，内部是否回文已经由 DP 保存，不需要再构造字符串或用双指针重新检查。

dp[i][j] 依赖下一行的 dp[i+1][j-1]，因此 i 从 n-1 递减到 0，让下一行先算完。j 从 i 向右遍历。长度至少 3 才读取内部状态，此时 i+1 和 j-1 均在合法下标范围内。第一轮 i=n-1 只处理单字符，不会访问 dp[n]。

以 "abba" 为例，先得到 dp[1][2]=true（"bb"），随后两端 'a' 相同，dp[0][3]=dp[1][2]=true，得到整个长度 4 的回文。

答案只在新回文更长时更新。用户比较 j-i 与 endIdx-startIdx 是正确的，因为两边长度都应加 1，加 1 后大小关系不变。相同长度保留原答案即可，题目允许多个最长答案中的任意一个。

## 可以精简的写法

用户最终循环已经包含第 0 行，并自行处理长度 1、2 的基础情况，所以可删除首行 isValid 初始化，以及不再使用的 isValid 函数。双字符分支直接给布尔表达式赋值，可以同时表达相等与不相等。

以下是静态复核后的学习参考，不是用户另一次力扣提交反馈。

```cpp
class Solution {
public:
    string longestPalindrome(string s) {
        int n = (int)s.size();
        int startIdx = 0;
        int endIdx = 0;
        vector<vector<bool>> dp(n, vector<bool>(n, false));

        for (int i = n - 1; i >= 0; --i) {
            for (int j = i; j < n; ++j) {
                if (i == j) {
                    dp[i][j] = true;
                } else if (i + 1 == j) {
                    dp[i][j] = (s[i] == s[j]);
                } else if (s[i] == s[j]) {
                    dp[i][j] = dp[i+1][j-1];
                }

                if (dp[i][j] && j - i > endIdx - startIdx) {
                    startIdx = i;
                    endIdx = j;
                }
            }
        }
        return s.substr(startIdx, endIdx - startIdx + 1);
    }
};
```

## 修改过程与报错复盘

首个 DP 草稿曾把 dp[j] 当作整数赋值，但二维数组的 dp[j] 是整行。采用区间是否回文的布尔状态后，需要明确两个下标分别表示起点与终点。

| 历史问题 | 原因与修正 |
|---|---|
| 所有长度直接读 dp[i+1][j-1] | 最后 i=j=n-1 时会读 dp[n][n-2]，外层行越界；长度 1、2 要单独处理 |
| i 从小到大计算 | 下一行尚未算好；改成 i 倒序 |
| 只初始化首行，未记录首行的答案 | 识别 dp[0][j] 后也要参与最长比较；最终循环统一覆盖所有起点 |
| 每遇到回文就覆盖 startIdx/endIdx | 后面较短的回文可能覆盖长答案；更新前比较长度 |
| 双字符分支写成 dp[i][j]==true | 这是比较表达式，结果被丢弃；改为赋值 = |
| 双循环内构造未使用的 substring | 每轮复制字符，让最坏总时间达到 O(n³)；删除该变量 |

在 "cbbd" 中，误写 == 导致 dp[1][2] 对应的 "bb" 仍为 false，最后只返回 "c"。最终版本已经修复，不再把这些历史问题当作当前缺陷。

AddressSanitizer 的 heap-buffer-overflow 表示访问堆上分配范围之外的内存，READ 表示越界读取，0 bytes after ... region 表示位置刚好超过分配区间。stl_bvector.h 出现在调用栈中，是非法访问通过 vector<bool> 的内部实现发生，并不表示标准库本身出错。对 n>=2 的首个递推版本，最后访问 dp[n] 即可解释该越界；当时 j>=i>=1，因此主因不是 j-1 变成负数。

## 超时与复杂度

已删除的 string substring=s.substr(i,j-i+1) 虽然没有被后续使用，创建 string 子串仍需复制字符。共有 n(n+1)/2 个区间，累计复制字符数量为：

```text
sum(L * (n-L+1), L=1..n) = n(n+1)(n+2)/6
```

n=1000 时约有 50 万个区间、累计复制 167,167,000 个字符，另外还有构造、析构与部分内存分配开销。这一部分为 O(n³)，是超时的主要原因。它对相同长度的其他字符串也会复制这么多字符；全 '0' 还使首行回文检查无法提前发现不匹配。

最终通过版保留的首行初始化中，所有前缀的复制总计 O(n²)，全相同字符时双指针检查总计也是 O(n²)。因此当前版总体已经是 O(n²) 时间、O(n²) 空间；精简首行只是减少重复计算，没有改变渐进复杂度。最终返回答案的 substr 只调用一次，最多 O(n)，可以保留。

## 复习与验证范围

复写前先说明 dp[i][j] 的闭区间含义，再写长度 1、2 的基础情况；从内部依赖推导 i 倒序，最后只在更长时保存起终点。用 "a"、"bb"、"cbbd"、"abba"、"babad" 和长全相同字符串分别检查边界、双字符、递推、并列答案与开销。

本轮依据用户力扣通过反馈、独立静态审查及手工推演确认逻辑；没有新增本地 C++ 编译或随机测试。学习状态是已通过、独立编码待巩固。继续按用户安排完成 Hot 100 DP 首轮，不主动展开其他回文算法。
