# 17. 电话号码的字母组合

状态：2026-09-29 用户先确认双层循环原版 LeetCode 提交通过，随后确认本笔记末尾的固定数字位置新版通过；两份用户代码复核正确。最新一轮 C++17 校验覆盖全部 4680 个合法输入，用户原版、用户新版与助手此前推荐版均与独立枚举参考一致。用户新版保留 int alpha、path+=alpha、字母组复制及非空入口，与助手参考版并非逐字相同；不把参考或提交通过等同于熟练掌握。用户在家网页刷题，未建立仓库 problems 脚手架。

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

## 后续疑问：为什么只把终止条件改成 startIndex==n 就不正确

用户保留原版两层循环，仅将保存条件改为 `startIndex == digits.size()`，收集后 return。此实验版本错误，不影响此前 path 长度检查版本已经通过的记录。

问题不是 return，而是参数含义与终止条件不匹配。外层 for 仍然可以跳过数字位置，递归参数是 i+1，所以 startIndex 只表示下一候选数字下标，不保证等于已经选出的字母数。

对 digits=23，根调用 startIndex=0：

| 分支 | 选择过程 | 下一次 startIndex | path | 是否完整 |
|---|---|---:|---|---|
| 正常分支 | 先选下标 0 的 a，再选下标 1 的 d | 2 | ad | 是，两个数字各贡献一个字母 |
| 跳过数字的分支 | 根调用外层循环直接令 i=1，选下标 1 的 d | 2 | d | 否，没有为数字 2 选字母 |

第二条分支调用 backtracking(digits, i+1)，即 backtracking(digits, 2)。新条件 2==digits.size() 成立，就把 d 错误保存。同理还会保存 e、f。C++17 实际运行返回 `[ad,ae,af,bd,be,bf,cd,ce,cf,d,e,f]` 共 12 个，而正确答案仅前 9 个。

三字符输入 234 的本轮复现返回 48 个结果，只有 27 个完整长度结果，另有 21 个短结果；单字符输入 2 则仍正确。测试仅在临时目录编译运行，无仓库脚手架；本次诊断未重新执行此前全部 4680 个输入的验证。

修正理解：

- 保留原版外层位置循环，就仍需用 path.size()==digits.size() 判断是否得到完整答案；之前通过不是因为条件可以任意互换。
- 推荐删除外层数字位置循环，只处理 digits[index]，每添加一个字母就递归 index+1。此时始终有 index==path.size()，才可以仅用 index==digits.size() 代表完整。
- 参数不是因为改名叫 index 就拥有这个含义，而是每一步“固定当前数字、只前进一位、添加一个字母”的操作维持了它。
- 保留外层循环也可在 startIndex 到末尾后再检查 path 长度，但这只过滤短路径，没有消除无效搜索；学习时优先使用固定数字位置的推荐结构。

当时为终止条件诊断补记，等待用户修正反馈；错误实验版不标记为通过，也不覆盖此前通过版本的历史状态。用户随后提交下节固定数字位置新版并确认通过。

## 最终确认通过：固定数字位置新版与最初通过版的区别

以下保留用户本轮确认通过的实现，没有擅自改成 char、引用或其他映射容器。

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
        if (index == (int)digits.size()) {
            res.push_back(path);
            return;
        }

        int digit = digits[index] - '0';
        string alphas = umap[digit];
        for (int i = 0; i < (int)alphas.size(); ++i) {
            int alpha = alphas[i];
            path += alpha;
            backtracking(digits, index + 1);
            path.pop_back();
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

| 对照点 | 最初通过的双层循环版 | 本轮通过的固定数字位置版 |
|---|---|---|
| 一次调用的任务 | 从 startIndex 起的候选位置中挑一个数字，再挑它的字母 | 必须为 digits[index] 挑一个字母 |
| 本层循环 | 数字位置循环 + 该数字的字母循环 | 只有当前数字的字母循环 |
| 参数含义 | 下一次可选的数字起点，不代表已选几个字母 | 已经处理的数字数量，也是当前要填的位置和路径长度 |
| 递归前进 | i+1，可能跳过数字 | index+1，只前进一位，不跳过数字 |
| 收集条件 | path.size()==digits.size()，过滤短路径 | index==digits.size()，此时每个数字都已参与 |
| 终止 | 收集时位置循环恰好已空，自然返回 | 显式 return，避免继续读取 digits[index] |
| 输出与撤销 | 完整结果正确；递归返回后删除本层字符 | 完整结果相同；撤销动作相同 |

旧版可以理解成“生成很多递增数字位置选择，再留下长度完整的方案”；新版直接构造“每个数字各选一个字母”的方案。两版都枚举所有完整答案，不是一个回溯、另一个非回溯。删去外层循环没有遗漏答案，因为本题本来就不允许跳过数字。

对 23，两版都生成九个完整字符串；旧版还从根直接选择数字 3，探索 d/e/f，然后因长度不足丢弃。新版根只处理数字 2，下一层才处理数字 3，不会单独生成这些短分支。两版递归中途都会出现短前缀如 a，不能把新版“无无效分支”理解成“从不出现短字符串”；这些前缀后面会继续补全。

新版在每个调用入口都保持 index==path.size()：初始都是 0；每次添加一个字符后递归 index+1；下层返回后删除本层字符，恢复原状态。原版只有未跳过数字的分支恰好满足该关系，因此原版不能直接用下标到末尾代替完整长度判断。这个差别来自操作规则，不是参数重命名。

递归的 index 决定沿数字位置向下推进；当前 for 的 i 只是在当前字母组中切换候选。例如 index=0 时 i 可以依次对应 a/b/c，但所有这些分支的下一层都是 index=1，而不是递归 i+1。两种下标角色要分开。

### 搜索工作量的对照

设第 i 个数字的候选字母数为 b_i（3 或 4），P 为所有 b_i 的乘积。

- 新版的调用数为 1 + b_0 + b_0*b_1 + ... + P：只构造各层完整位置前缀。保存 P 个长度 n 的字符串需要 O(n*P) 时间；辅助空间不计结果为 O(n)。
- 旧版的调用数为所有 (1+b_i) 的乘积：每个数字位置既可以不选，也可以选其某个字母，每种按下标递增的部分方案都会被访问。计入结果复制，总时间为 O(所有 (1+b_i) 的乘积 + n*P)，辅助空间不计输出同样 O(n)。
- 全部数字都是四字母组时，调用数旧版是 5^n，新版是 1+4+...+4^n。题目数字长度最多 4，旧版因规模小仍能通过，不代表这些额外分支是必须的。
- 本轮实际计数：23 为旧版 16 次、新版 13 次；9999 为旧版 625 次、新版 341 次，输出分别仍为相同的 9 个、256 个字符串。

### 本轮复核与复习结论

在临时目录中扩展此前测试，按用户本轮代码的递归逻辑保留 string 字母组复制、int alpha、+= 和 index+1，仅增加调用计数及路径长度断言用于检查。g++ C++17 -Wall -Wextra -pedantic 编译无警告；全部 4680 个合法输入上，三份实现均与独立逐组笛卡尔积枚举一致。同对象反复调用、输入不变、path 最终为空和已保存结果独立也通过；未在仓库添加测试脚手架。本轮对用户新版的验证限定题面给出的非空数字串，不把助手参考版可选空串防御当作用户已有代码。

本题无需继续为了正确性修改算法。int alpha 在本题字母范围内正确，后续想提高表达清晰度，可选择 char alpha 和 push_back；不必把这些语法整理与本次搜索结构优化混为一谈。

优先记忆新版：位置 index 决定层数，字母循环决定同层分支；每个数字选一个，选完才保存。旧版可作为对照，帮助理解“循环范围、递归参数、收集条件”必须一起设计，而不是套用一个终止条件。
