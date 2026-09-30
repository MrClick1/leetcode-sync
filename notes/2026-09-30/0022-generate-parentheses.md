# 22. 括号生成

- 题目开始于 2026-09-29，首次代码诊断与笔记日期为 2026-09-30。
- 状态：用户成员变量修正版已 LeetCode 提交通过；后续局部变量+引用参数版与用户提供的官解也对全部 n=1..8 经 C++17 穷举复核通过，未另行收到局部变量版的力扣提交反馈。
- 用户在 LeetCode 网页写题，问题记录到 notes。

## 首版代码与错误记录

```cpp
class Solution {
public:
    string path;
    vector<string> res;

    void backtracking(const int& n, int left, int right) {
        if (right > left) return;

        if (right == n) {
            res.push_back(path);
            return;
        }

        if (right == left) {
            path.push_back('(');
            ++left;
            backtracking(n, left, right);
            --left;
            path.pop_back();
        } else if (left < right) {
            path.push_back('(');
            ++left;
            backtracking(n, left, right);
            --left;
            path.pop_back();

            path.push_back(')');
            ++right;
            backtracking(n, left, right);
            --right;
            path.pop_back();
        }
    }

    vector<string> generateParenthesis(int n) {
        res.clear();
        path.clear();
        backtracking(n, 0, 0);
        return res;
    }
};
```

left 表示已放入的左括号数量，right 表示已放入的右括号数量。path 保存当前括号前缀；res 保存完整结果。

## 空结果的直接原因：合法状态没有对应分支

先用 n=1 逐行推演：

1. 首次调用 left=0、right=0，满足 right==left，加入左括号，path="("。
2. 递归到 left=1、right=0。right>left 不成立，right==n 不成立。
3. right==left 不成立；else if(left<right) 即 1<0 也不成立。
4. 函数走到末尾自然返回，没有追加右括号，也没有保存结果。
5. 初始调用恢复 left/path 后结束，res 仍为空。

对合法 n>=1，初始步骤相同，所以该版本都在第一个左括号之后结束。

此外，顶端 `if (right > left) return;` 与后面的 `else if (left < right)` 判断的是同一关系。若该关系成立，函数早已返回，所以 else if 分支无法到达。

right>left 时，前缀已有无法与前面左括号配对的右括号，例如 ")"，未来追加括号无法修复前缀；这个剪枝本身正确。正常可以继续的非平衡状态是 left>right，即存在尚未闭合的左括号。

## 修正方向：分别判断能否追加两类括号

不要仅将 left<right 反过来：原分支会无条件继续加入左括号，缺少左括号数量上限，可能形成不断增加 left 的递归。

每次调用都可以分别判断两个动作：

- left<n：左括号还没用完，可以追加一个 '('。
- right<left：已有未闭合的左括号，可以追加一个 ')'。

两个动作可能同时合法。例如 n=2、path="("、left=1、right=0 时，既可继续追加 '(' 得到 "(("，也可追加 ')' 得到 "()"。两条分支分别继续探索，才能生成 "(())" 与 "()()"。

一种整理方式是把原来的分支替换为两个独立 if，分别判断两种动作是否可选。这不是说本题不能使用 if/else if；用户最终版采用互斥状态分类，也正确，见下文。沿用用户主动修改、再撤销计数的写法：

```cpp
if (left < n) {
    path.push_back('(');
    ++left;
    backtracking(n, left, right);
    --left;
    path.pop_back();
}

if (right < left) {
    path.push_back(')');
    ++right;
    backtracking(n, left, right);
    --right;
    path.pop_back();
}
```

两段先后执行：第一条分支递归返回后，计数和 path 恢复到本层入口状态，再判断并探索第二条分支。将这两个条件连成 else if，会在两者同时成立时漏掉右括号分支。

也可像上一题的 sum+num 一样用 left+1/right+1 传值，避免主动修改本层计数；但不是此次问题的必要修正。用户原有的 ++/-- 配对本身成立。

## 计数约束与收集条件

两条选择规则维持 `0 <= right <= left <= n`：

- 追加左括号前检查 left<n，保证左括号不超过 n。
- 追加右括号前检查 right<left，保证任意前缀中右括号数量不超过左括号数量。

在此约束下，用户的 right==n 收集条件可以保留：right==n 与 right<=left<=n 一起推出 left==n，因此两类括号各有 n 个。写成 left==n && right==n 会更直接，也同样正确。

上述先判断动作是否合法的版本，每次递归添加一个字符，最多添加 2n 个，因此搜索有明确长度上限。

与 39 组合总和对照：本题每层候选是 '(' 和 ')' 两种动作，计数决定动作是否可执行，不需要候选数组循环或 startIndex。使用两个独立 if 也可以枚举一层的多个候选。

## 用户最终通过的代码

```cpp
class Solution {
public:
    string path;
    vector<string> res;

    void backtracking(const int& n, int left, int right) {
        if (left > n || right > left) return;

        if (right == n) {
            res.push_back(path);
            return;
        }

        if (right == left) {
            path.push_back('(');
            ++left;
            backtracking(n, left, right);
            --left;
            path.pop_back();
        } else if (left > right) {
            path.push_back('(');
            ++left;
            backtracking(n, left, right);
            --left;
            path.pop_back();

            path.push_back(')');
            ++right;
            backtracking(n, left, right);
            --right;
            path.pop_back();
        }
    }

    vector<string> generateParenthesis(int n) {
        res.clear();
        path.clear();
        backtracking(n, 0, 0);
        return res;
    }
};
```

### 为什么这版的 if/else if 正确

最上面的剪枝先排除 left>n 或 right>left。通过检查后，计数满足 0<=right<=left<=n，剩下两类互斥状态：

| 当前状态 | 含义 | 本层探索 |
|---|---|---|
| left==right | 已有左括号都闭合了，没有可供新右括号匹配的左括号 | 只能继续放 '('；两者已等于 n 时，上面的收集条件已返回 |
| left>right | 还有未闭合的左括号 | 分别尝试 '(' 与 ')'；左括号若超额，在下一次调用入口剪枝 |

用户在 left>right 的同一个分支内部探索了两个动作，所以不会因为使用 else if 而漏掉右括号选择。与两条独立 if 的区别在于：此处分类条件是“当前处于什么状态”，而 left<n/right<left 是“当前哪个动作可以执行”。状态互斥可以用 else if；动作可同时合法就要分别探索。

例如 n=2、path="("、left=1、right=0 时，进入 left>right 分支：先探索加 '(' 的路径，最终得到 "(())"；恢复为 "(" 后，再探索加 ')' 的路径，最终得到 "()()"。不是先做完左括号选择后，在改变了的状态上接着选右括号；两条分支各自从本层入口状态出发。

这次的两处必要修正：把原来不可达的 left<right 改为 left>right，覆盖有待闭合括号的合法状态；增加 left>n 的剪枝，限制左括号使用数量。right==n 的收集条件可以保留，因为通过剪枝后 right<=left<=n，right==n 自动推出 left==n。

### 小优化：尝试后剪枝与选择前检查

当前版在 left==n、right<n 时仍会尝试放一个 '('，递归入口发现 left==n+1 便立即返回。这条无效尝试不会收集结果，不影响正确性。若想少一次调用，可以在追加 '(' 前检查 left<n；整理成上面的两个独立 if 更容易统一这条规则。

left/right 是按值传参。用户先 ++、递归、再 -- 的配对写法正确：-- 是恢复本层变量，path.pop_back() 则恢复共享的成员字符串。也可以传 left+1/right+1，让本层计数保持不变，仍需撤销 path。n 使用 const int& 合法，但小整数直接传 int 也足够。

## 验证与复杂度

2026-09-30 在系统临时目录编译用户最终代码：g++ -std=c++17 -O2 -Wall -Wextra -pedantic，无警告。独立参考用位掩码枚举所有长度 2n 的括号串，再检查所有前缀余额非负、最终余额为零；不复用用户的回溯逻辑。

| n | 合法组合数 | 用户结果与穷举参考 |
|---:|---:|---|
| 1 | 1 | 一致 |
| 2 | 2 | 一致 |
| 3 | 5 | 一致 |
| 4 | 14 | 一致 |
| 5 | 42 | 一致 |
| 6 | 132 | 一致 |
| 7 | 429 | 一致 |
| 8 | 1430 | 一致 |

共校验 2055 个合法组合，无重复、无遗漏；复用同一 Solution 对象、返回后 path 为空、此前返回结果副本保持不变均通过。临时测试没有加入仓库，也没有创建题目脚手架。

设 C_n 为 n 对括号的合法组合数（卡特兰数）。时间 O(n×C_n)，包括复制每个长度 2n 的结果；不计输出，递归栈和 path 占 O(n) 辅助空间。用户最终版偶尔多试一个左括号再剪枝，不改变这些复杂度。

## 复习重点

先定义 left/right 为已使用数量，再从“此时能否放一个字符”写条件。左括号看剩余数量，右括号看能否匹配已有左括号。选择、递归、撤销三步仍与之前回溯题一致。

首版诊断只做代码推演；用户随后自行修正并确认 LeetCode 通过，本轮已编译复核最终版。记忆规则：左括号看是否还有名额，右括号看是否有尚未闭合的左括号；能选的分支都探索，递归返回恢复现场。

相关语法：[独立 if 与 else if](../01-cpp语法知识文档.md)。

## 后续调整：局部变量与引用参数

2026-09-30，用户把 res/path 从成员变量改成入口函数中的局部变量，再以非 const 引用传给递归。这一改动正确，选择逻辑与上一版相同：

```cpp
class Solution {
public:
    void backtracking(vector<string>& res, string& path,
                      const int& n, int left, int right) {
        if (left > n || right > left) return;

        if (right == n) {
            res.push_back(path);
            return;
        }

        if (right == left) {
            path.push_back('(');
            ++left;
            backtracking(res, path, n, left, right);
            --left;
            path.pop_back();
        } else if (left > right) {
            path.push_back('(');
            ++left;
            backtracking(res, path, n, left, right);
            --left;
            path.pop_back();

            path.push_back(')');
            ++right;
            backtracking(res, path, n, left, right);
            --right;
            path.pop_back();
        }
    }

    vector<string> generateParenthesis(int n) {
        vector<string> res;
        string path;
        backtracking(res, path, n, 0, 0);
        return res;
    }
};
```

res/path 的局部变量每次调用入口都会重新构造为空，因此不再需要 clear。& 让所有递归调用共同使用入口中的同一份结果容器与路径，避免每层复制；共享的 path 仍要在递归返回后 pop_back。res.push_back(path) 保存的是字符串副本，后续修改 path 不会改变已收集的答案。

## 与用户提供的官方回溯代码对照

代码来源为用户提供的 [力扣官方题解](https://leetcode.cn/problems/generate-parentheses/solutions/192912/gua-hao-sheng-cheng-by-leetcode-solution/)，本轮基于贴出的代码进行分析，未另行访问网页。算法都是回溯枚举合法括号前缀。

| 用户版 | 官解 | 含义 |
|---|---|---|
| res | ans | 所有完整合法方案 |
| path | cur | 当前正在构造的括号串 |
| left | open | 当前已放的左括号数量，不是下标 |
| right | close | 当前已放的右括号数量，不是下标 |

### 1. 状态分类与动作检查

用户版判断 left==right / left>right，按状态决定本层尝试什么：平衡时只加 '('，有待闭合左括号时分别尝试两种字符。官解直接逐个检查动作：

```cpp
if (open < n) {
    // 左括号还有名额，探索添加 '(' 的分支
}
if (close < open) {
    // 存在待闭合左括号，探索添加 ')' 的分支
}
```

n=2、cur="("、open=1、close=0 时，两条动作条件同时成立，必须用两个独立 if；第一条分支探索并撤销后，第二条仍应探索，分别得到 "(())" 和 "()()"。用户版把两条动作放在 left>right 状态分支内部，也覆盖了这两种选择，没有算法错误。

### 2. 先尝试后剪枝与先检查再选择

用户版 left==n 且 right<n 时，仍会尝试添加 '('，随后递归入口发现 left>n，立即返回。官解 open<n 不成立时直接跳过这条选择，少一次无效调用。例如 n=2、path="((" 时，官解只探索添加 ')'。

官解从 open=close=0 出发：只有 open<n 才增加 open，只有 close<open 才增加 close，因此始终维持 0<=close<=open<=n，不会进入超额或右括号过多的状态，不需要入口再写用户版的两项剪枝。两版的大 O 复杂度相同。

### 3. 修改本层计数与只计算下一层参数

```cpp
// 用户：改变本层 left，所以递归返回后恢复
++left;
backtracking(res, path, n, left, right);
--left;

// 官解：仅计算下一层 open 的值，没有改变本层 open
backtrack(ans, cur, open + 1, close, n);
```

当本层 open=1，官解把数值 2 传给下一层的 open，本层仍为 1，递归返回后无需 --open。用户写 ++left 先把本层 left 从 1 改成 2，因此需要 --left 恢复。两者都是 int 按值传递；按值传递不意味着主动修改的本层变量会在递归返回时自动恢复。cur/path 是引用共享对象，两版都必须 pop_back 撤销所加的字符。

可以只改变计数传法，不必同时改变状态分类：用户版追加 '(' 后递归传 left+1，追加 ')' 后传 right+1，就可分别删掉对应 ++/--，保留 push_back/pop_back。

### 4. 收集条件不同，依赖的是同一合法前缀约束

用户版在入口剪枝后满足 right<=left<=n，所以 right==n 能推出 left==n。官解中 cur.size()==open+close，且 close<=open<=n；当长度等于 2n 时，两种括号只能各为 n 个。两种结束条件在各自约束下等价，单独“长度够了”并不能证明任意括号串有效，有效性来自沿途的选择条件。

用户 const int& n 与官解 int n 都正确；对于小整数，按值传 int 简单且开销很小。官解辅助函数默认 private，用户放在 public；这只影响能否从类外调用辅助函数，不改变搜索。

### 本轮复核与复习口诀

成员变量版、用户局部变量版、所给官解各对 n=1..8 与独立位掩码枚举参考一致，每版覆盖 2055 个合法组合。官解测试仅将长度比较右侧显式转换为 size_t，算法未改；g++ C++17 开启 -Wall -Wextra -pedantic 编译无警告。新局部变量版的引用 path 返回后为空，同对象反复调用与已返回结果副本独立性检查通过。没有在仓库创建本地题目脚手架；局部变量版为本地验证正确，不补称用户已再次力扣提交。

口诀：左括号未用完就能放；右括号有左可配才能放；两种都能放就分别探索；长度够了存，改过的状态再撤销。记“本层变量是否被修改”，不要把所有计数都机械加减。
