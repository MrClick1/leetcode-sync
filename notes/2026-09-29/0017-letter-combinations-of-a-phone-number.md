# 17. 电话号码的字母组合

状态：2026-09-29 用户确认双层循环原版 LeetCode 提交通过；代码复核正确。原版与下面推荐的固定数字位置简化版，均通过临时目录中 C++17 对全部 4680 个合法输入的校验。简化版是参考建议，尚未收到用户再次提交它的反馈；不把提交通过等同于思路熟练掌握。用户在家网页刷题，未建立仓库 problems 脚手架。

## 用户通过的实现

```cpp
class Solution {
public:
    string path;
    vector<string> res;

    unordered_map<int, string> umap = {
        {2, "abc"}, {3, "def"}, {4, "ghi"}, {5, "jkl"},
        {6, "mno"}, {7, "pqrs"}, {8, "tuv"}, {9, "wxyz"}
    };

    void backtracking(const string& digits, int startIndex) {
        if (path.size() == digits.size()) {
            res.push_back(path);
        }

        for (int i = startIndex; i < (int)digits.size(); ++i) {
            int digit = digits[i] - '0';
            string alphas = umap[digit];

            for (int j = 0; j < (int)alphas.size(); ++j) {
                int alpha = alphas[j];
                path += alpha;
                backtracking(digits, i + 1);
                path.pop_back();
            }
        }
    }

    vector<string> letterCombinations(string digits) {
        res.clear();
        path.clear();
        backtracking(digits, 0);
        return res;
    }
};
```

## 对照代码随想录：哪里一致，哪里可以简化

来源：[代码随想录：17. 电话号码的字母组合](https://programmercarl.com/algo/backtracking/0017-letter-combinations-of-a-phone-number.html#%E6%80%9D%E8%B7%AF)。本轮直接网页读取超时后，按用户此前授权在侧边栏打开对应页面，已读取思路、C++ 示例和总结；以下是个人解释与用户代码的整理，不复制整篇文章。

| 项目 | 用户原版 | 推荐结构及与文章的对照 |
|---|---|---|
| 数字到字母映射 | unordered_map<int, string> | 文章使用固定字符串表；两者都能完成映射 |
| 当前方案 | 成员 string path | 同样维护正在构造的字符串 |
| 数字位置 | startIndex 起点，再循环选择 i | index 直接表示这层必须处理的数字位置 |
| 当前层候选 | 同时枚举后续数字位置和对应字母 | 只枚举 digits[index] 对应的字母 |
| 收集 | path 长度等于数字长度 | index 等于数字长度；维护 path.size()==index 时等价 |
| 终止 | 收集后无显式 return | 收集后 return，不再访问下一个数字 |
| 撤销 | path.pop_back() | 相同；只撤销本层追加的字母 |

原版没有算法错误。它允许跳过数字位置，但一旦跳过，最终 path 长度不足，就不会收集。例如输入 23，根调用除从数字 2 开始得到九个正确结果，还会直接从数字 3 开始探索 d、e、f；这三个短字符串不保存，却花费额外调用。

为什么不会保存错位的完整结果？原版选择的数字下标严格递增且不重复。若 n 个数字中选出了 n 个位置，只能恰好选择 0..n-1，因此完整路径包含所有数字位置，顺序也正确。

为什么原版不写 return 也通过？path 长度已经为 n 时，选到的最后数字下标必为 n-1，下一次调用 startIndex 已是 n，循环没有候选，自然返回。简化版没有这个外层空循环的保护，终止分支收集后必须返回，再访问 digits[index] 就不符合本层任务。

## 碰到本题时如何想到回溯

1. 一个答案有多少个字符？有几个数字，就必须填几个字符。
2. 答案第 index 个字符从哪里来？只能从 digits[index] 对应的字母组中选一个。
3. 两个数字可以写两层循环，三个数字要三层；用递归把“循环层数随输入变化”写成统一结构。
4. 每次调用只负责一个位置：本层循环字母，下一层处理下一数字。
5. 所有位置填完才收集；递归返回后恢复当前 path，继续尝试本位置的下一字母。

定义：进入 backtracking(digits, index) 时，path 恰好对应前 index 个数字，且 path.size()==index。不要机械照搬子集的 startIndex 循环。

## 23 的调用与撤销模拟

```text
第 0 层：处理数字 2，候选 a / b / c
    选 a -> path="a"
    第 1 层：处理数字 3，候选 d / e / f
        选 d -> path="ad" -> 第 2 层保存 -> 返回后删 d，恢复 "a"
        选 e -> path="ae" -> 第 2 层保存 -> 返回后删 e，恢复 "a"
        选 f -> path="af" -> 第 2 层保存 -> 返回后删 f，恢复 "a"
    第 1 层循环结束返回；第 0 层删 a，恢复 ""
    改选 b -> 产生 bd / be / bf -> 删 b，恢复 ""
    改选 c -> 产生 cd / ce / cf -> 删 c，恢复 ""
```

递归返回只会回到调用处，不会自动修改共享 path。下层返回前已撤销下层的选择，本层再撤销自己添加的那个字母。res.push_back(path) 会复制字符串，之后删除 path 字符不会删除已经保存的结果。

## 基于用户代码的推荐简化版

保留熟悉的 unordered_map 和成员变量，只修改每层的职责。

```cpp
class Solution {
public:
    string path;
    vector<string> res;
    unordered_map<int, string> umap = {
        {2, "abc"}, {3, "def"}, {4, "ghi"}, {5, "jkl"},
        {6, "mno"}, {7, "pqrs"}, {8, "tuv"}, {9, "wxyz"}
    };

    void backtracking(const string& digits, int index) {
        if (index == static_cast<int>(digits.size())) {
            res.push_back(path);
            return;
        }

        int digit = digits[index] - '0';
        const string& alphas = umap.at(digit);
        for (char alpha : alphas) {
            path.push_back(alpha);
            backtracking(digits, index + 1);
            path.pop_back();
        }
    }

    vector<string> letterCombinations(string digits) {
        res.clear();
        path.clear();
        if (digits.empty()) return res; // 可选防御：当前题面保证非空
        backtracking(digits, 0);
        return res;
    }
};
```

- `char alpha` 比原版 int 更贴合字符含义；push_back/pop_back 明确表达成对添加与撤销。
- `const string&` 不复制字母组，也不修改它；at 不会为缺失 key 静默插入条目。本题保证数字 2..9，映射完整，原版 [] 也正确。
- 固定 10 个字符串的表也很适合本题，但这是映射容器选择，不是正确性修复；可先保留 map 熟悉递归流程。
- 当前题面保证 digits 非空；空字符串提前返回是对更一般调用场景的防御，并非原版在当前范围内的错误。

## 与 46 全排列、78 子集的区别

| 题目 | 每层选择范围 | 什么时候保存 | 需要 used 吗 |
|---|---|---|---|
| 46 全排列（path/used 版） | 所有未使用的数组位置 | 选够全部元素 | 需要，记录当前排列用了哪些位置 |
| 78 子集 | startIndex 之后的位置，允许跳过元素 | 每个调用入口 | 不需要，i+1 保证下标严格递增 |
| 17 字母组合 | 当前数字位置对应的字母组 | 每个数字都选完 | 不需要，每层已固定数字位置 |

17 不允许跳过数字，也不禁止不同位置出现相同字母，例如 digits=22 的 aa 是合法答案。不能把“全排列不重复使用数组位置”理解成“所有回溯题不许重复字母”。

## 字符串语法问题

```cpp
path.push_back('a');  // 添加一个字符
path += 'a';         // 也能添加字符
path += "abc";       // 添加一个字符串
path.pop_back();     // 删除末尾一个字符，要求非空，返回 void
path.back();         // 读取末尾字符，不删除，要求非空
path.clear();        // 清空全部，不适合作为撤销一个字符
```

原版 int alpha=alphas[j] 再 path+=alpha，在本题字母范围内能工作：char 转为 int 后又转回 char，并不是把数字的十进制文本加进去。若要整数文本，用 to_string。完整参考说明已补充到 [C++ 语法笔记的 string 章节](../01-cpp语法知识文档.md)。

## 复杂度与本轮验证

设 n 为数字个数，第 i 个数字有 b_i 个候选字母（3 或 4），答案总数 P=各 b_i 的乘积。

- 推荐简化版时间 O(n×P)，因为每个答案要复制一个长 n 的字符串；最坏 O(n×4^n)。
- 不计输出，path 与递归栈需要 O(n) 辅助空间；保存结果需要 O(n×P)。
- 网页用三个字母/四个字母数字的数量描述指数部分；这里额外计入结果字符串的复制长度，并明确区分结果空间和辅助空间。
- 原版会探索跳过数字的短路径。实际计数：23 原版 16 次调用、简化版 13 次；9999 原版 625 次、简化版 341 次，结果相同。

临时目录中使用 g++ -std=c++17 -Wall -Wextra -pedantic 编译，无警告；枚举数字 2..9 的所有长度 1..4 字符串，共 8+64+512+4096=4680 个输入。两版分别与独立的逐组笛卡尔积枚举比对，结果正确且无重复；复用同一对象，检查 path 恢复为空、调用者输入不变和已返回结果不受后续调用影响。另检查简化版可选空输入处理与 string 操作示例。未在仓库添加测试脚手架。

## 下次复盘

先口述“每个数字都参与、每层固定一个位置”，再写终止分支、取当前字母组、循环字母、添加/递归/撤销。手动追踪 23 的前缀 a 如何依次产生 ad/ae/af，并对比 22 可以产生 aa。最后脱离提示重写，确认不再照搬子集的外层数字位置循环。
