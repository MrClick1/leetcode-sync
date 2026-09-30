# 131. 分割回文串

- 开始日期：2026-09-30。
- 状态：用户补上回文检查的 ++l/--r 后于 2026-09-30 明确反馈力扣通过；C++17 对 724 个输入与独立分割参考复核一致。以下保留错误诊断历史，再记录通过版，不将通过等同于熟练掌握。
- 学习方式：在 LeetCode 网页端编写，本地只维护笔记，不建立题目脚手架。

## 题目要求

将字符串 s 完整分割为若干连续子串，每一段都是回文串，返回所有分割方案。1<=s.length<=16，仅含小写字母。

- s="aab" -> [["a","a","b"],["aa","b"]]。
- s="a" -> [["a"]]。

## 用户第二版：已修正 substr 的参数类型

```cpp
class Solution {
public:
    bool isValid(const string& s) {
        int l = 0;
        int r = (int)s.size() - 1;
        while (l < r) {
            if (s[l] != s[r]) return false;
        }
        return true;
    }

    void backtacking(const string& s, vector<vector<string>>& res,
                     vector<string>& path, int startIndex) {
        if (startIndex == s.size()) {
            res.push_back(path);
        }
        for (int i = startIndex; i < s.size(); ++i) {
            string subs = s.substr(startIndex, i - startIndex + 1);
            if (isValid(subs)) {
                path.push_back(subs);
                backtacking(s, res, path, i + 1);
                path.pop_back();
            }
        }
    }

    vector<vector<string>> partition(string s) {
        vector<vector<string>> res;
        vector<string> path;
        backtacking(s, res, path, 0);
        return res;
    }
};
```

## 一、首版把迭代器传给 substr

首版写 s.substr(s.begin()+startIndex, i-startIndex+1)。substr 的起点参数要求整数下标，不接收 string 迭代器；这是参数类型不匹配，不能正常编译。

第二版的 s.substr(startIndex, i-startIndex+1) 正确，取闭区间 [startIndex,i]，其长度为 i-startIndex+1。不要因为 sort/reverse 使用迭代器区间，就认为 substr 也采用同样接口。语法对照见 [C++ 语法文档](../01-cpp语法知识文档.md)。

## 二、主要问题：回文判断没有向中间推进

检查 "aa" 时，l=0、r=1，s[l]==s[r]，因此不返回 false；但是循环没有修改 l/r，下一轮仍为同样位置，l<r 永远成立。

不只是回文串会卡住："abca" 首尾也是相同的 a，会一直比较首尾，根本没有机会检查里面的 b/c。单字符会直接返回 true；"ab" 第一轮发现不相等，会正常返回 false。这解释了为什么部分小例子看起来正常。

局部修正建议：每比较成功一对字符，就让左右指针各向中间走一步。

```cpp
while (l < r) {
    if (s[l] != s[r]) return false;
    ++l;
    --r;
}
return true;
```

诊断阶段仅指出上述辅助函数片段，没有代替用户重写整套算法；用户随后补上指针推进并确认通过，最终代码见下文。

## 三、以 aab 分析程序停在哪里

先逐字符选择 "a"、"a"、"b"，三个单字符的回文检查均直接通过，收集 ["a","a","b"]。回退后试 "ab"，第一对字符不相等，正常拒绝。再回到起点试 "aa"，回文检查不移动指针而卡住；已保存部分结果，但整个 partition 不能正常返回，不能把它说成只返回了第一组答案。

用带 4 次比较保护的 JavaScript 行为模型按第二版模拟，记录上述顺序及已经收集的第一组答案；这是行为模型证据，不是原始 C++ 的完整运行，也不是修正版通过证明。

## 四、回溯框架正确，收集后 return 是清晰性改进

- startIndex 表示尚未分割部分的起点。
- 本层 i 枚举这一段的结束下标，当前候选为 s[startIndex..i]。
- 只选择回文段，加入 path，然后从 i+1 继续分割；返回后 pop_back 撤销本层片段。
- startIndex==s.size() 表示字符串已完整分割，保存整个 path。

建议收集后加 return，明确表达本层结束：

```cpp
if (startIndex == static_cast<int>(s.size())) {
    res.push_back(path);
    return;
}
```

但用户原版没有这个 return 并不是当前错误的原因：此时 for 的 i 初始就等于 s.size()，i<s.size() 为 false，自然结束，不会继续截取。backtacking 的拼写虽然不是常见的 backtracking，但定义和调用一致，也不影响功能。

## 用户通过的最终实现

只修正双指针推进，保留用户原有接口、命名以及收集后自然结束的结构；没有额外引入预处理或改写算法。

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

    void backtacking(const string& s, vector<vector<string>>& res,
                     vector<string>& path, int startIndex) {
        if (startIndex == s.size()) {
            res.push_back(path);
        }
        for (int i = startIndex; i < s.size(); ++i) {
            string subs = s.substr(startIndex, i - startIndex + 1);
            if (isValid(subs)) {
                path.push_back(subs);
                backtacking(s, res, path, i + 1);
                path.pop_back();
            }
        }
    }

    vector<vector<string>> partition(string s) {
        vector<vector<string>> res;
        vector<string> path;
        backtacking(s, res, path, 0);
        return res;
    }
};
```

## 一次调用的任务与记忆框架

一次 backtacking(...,startIndex) 负责在已有 path 后，继续分割尚未处理的后缀 s[startIndex..]。已选择片段恰好覆盖 s[0..startIndex-1]，每段都是非空回文串；本层不需要 used，因为递归起点只会向右推进。

本层固定 startIndex，循环只移动终点 i，依次尝试长度 1、2、3 等当前片段。不是循环选择任意字符，也不是把 startIndex 和 i 同时向右移动。合法候选递归 i+1，意味着当前闭区间已经分割完，从它后一个字符继续；返回后只撤销自己加入的那一段。

口诀：起点固定，终点枚举；回文才选，后缀继续；切到末尾，保存整组；递归回来，撤销本段。

## 本地复核证据

在系统临时目录使用 g++ -std=c++17 -O2 -D_GLIBCXX_ASSERTIONS 编译用户最终版，未创建仓库题目脚手架。

- 14 个固定输入：aab/a、单段成功或失败、奇偶回文、abca、重复模式、长度 16 的全相同字符与全不同字符等。
- 510 个穷举输入：长度 1..8 的全部 a/b 字符串。
- 200 个确定种子随机输入：长度 1..12，字符来自 a/b/c。
- 共 724 个输入与独立参考一致。参考枚举 n-1 个字符间隙的切割位掩码，逐字符构造片段，再用反转比较检查回文，不使用递归分割。
- 每段非空且回文、所有片段连接为原串、无重复方案、所有非空子串的 isValid 结果均复核通过。
- 固定输入直接调用辅助函数继续分割已选首字符后的后缀，返回后原 path 不变；同一 Solution 对象反复调用、输入不变和已返回结果保持独立通过。
- 长度 16 的全相同字符串有 2^15=32768 种分割，输出数量正确；全不相同字符的最大长度输入也已验证，不是对长度 16 的所有字符串做穷举。

历史死循环使用的是带次数保护的 JavaScript 模型；本节是用户修正版的实际 C++17 运行。两者证据类型不同。

## 复杂度

令 n=s.length。最坏时间 O(n×2^n)：最多 2^(n-1) 种切割方式，保存一组方案需要复制总长 n 的字符及相应字符串对象；substr、回文判断和搜索开销也包含在此最坏上界内。

不计结果的辅助空间 O(n)：最多 n 层递归、n 个路径片段；当前路径以及活跃调用持有的已选 subs 总字符数均为 O(n)。计入结果时，最坏输出空间 O(n×2^n)。不需要因为结果数量大就改用哈希去重：终点序列确定一种切法，原有循环结构不会生成重复方案。

当前先保留这套已经正确的基础写法，等待用户指定下一题；不主动推进预处理等优化。
