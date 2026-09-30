# 79. 单词搜索

- 开始日期：2026-09-30。
- 状态：用户 path 修正版已 LeetCode 提交通过；用户随后独立改为 k 下标版，逻辑正确，尚待该版本的力扣提交反馈。C++17 对两种用户版、所给官解及助手整理版，各用 3153 个输入与独立参考算法复核一致。已补记差异、简化方式与记忆框架，不将通过等同于熟练。
- 学习方式：在 LeetCode 网页端编写，只记录笔记，不创建本地脚手架。新题不提前提示，用户本轮请求诊断后再展开分析。

## 题目要求

给定二维字符网格 board 和单词 word，判断能否按单词的字母顺序，由水平或竖直相邻的单元格构成该单词。同一个单元格不能在同一次构词中重复使用。

## 用户提供的示例

```text
board = [['A','B','C','E'],
         ['S','F','C','S'],
         ['A','D','E','E']]

word = "ABCCED" -> true
word = "SEE"    -> true
word = "ABCB"   -> false
```

## 数据范围与进阶要求

- 1 <= m, n <= 6。
- 1 <= word.length <= 15。
- board 和 word 仅由大小写英文字母组成。
- 进阶要求：考虑搜索剪枝，使更大网格上的搜索更快。

## 本轮进度

题面中的“已解答”不作为本轮通过证据。用户随后明确反馈修正版已通过，以下先保留首版及诊断历史，再记录通过代码与官解对照。用户继续在 LeetCode 网页写题，本地只维护笔记。

## 用户首版代码

```cpp
class Solution {
public:
    vector<pair<int, int>> directions = {
        {-1, 0}, // 上
        {1, 0},  // 下
        {0, -1}, // 左
        {0, 1},  // 右
    };

    bool backtracking(const vector<vector<char>>& board, string& path,
                      int r, int c, vector<vector<bool>>& visited,
                      const string& word) {
        if (path == word) return true;

        bool flag = false;
        for (int i = 0; i < 4; ++i) {
            int newR = r + directions[i].first;
            int newC = c + directions[i].second;

            if (newR >= 0 && newR < board.size() &&
                newC >= 0 && newC < board[0].size() &&
                visited[newR][newC] == false) {
                path.push_back(board[newR][newC]);
                visited[newR][newC] = true;
                flag = backtracking(board, path, newC, newR, visited, word);
                visited[newR][newC] = false;
                path.pop_back();

                if (flag == true) break;
            }
        }
        return flag;
    }

    bool exist(vector<vector<char>>& board, string word) {
        int n = (int)board.size();
        int m = (int)board[0].size();
        string path;
        vector<vector<bool>> visited(n, vector<bool>(m, false));
        bool flag = false;

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                flag = backtracking(board, path, i, j, visited, word);
                if (flag == true) break;
            }
            if (flag == true) break;
        }
        return flag;
    }
};
```

## 问题一：起点没有加入路径

exist 从 (i,j) 调用 backtracking 时，path 仍为空，visited[i][j] 也为 false。辅助函数没有把当前位置加入路径，只尝试它的邻居；因此第一次收集的字母是邻居的字母，而不是指定起点的字母。

最小反例：board=[['A']]、word="A"。原版 path=""，不等于 word；唯一单元格没有合法邻居，四个方向均跳过，返回 false，但正确答案为 true。

保留用户现有结构时，起点也应成对选择、撤销，并先确认它匹配首字母：

```cpp
if (board[i][j] != word[0]) continue;

path.push_back(board[i][j]);
visited[i][j] = true;
flag = backtracking(board, path, i, j, visited, word);
visited[i][j] = false;
path.pop_back();
```

已有起点也必须被标记，否则加入它后，后续邻居搜索可能再次使用这个单元格。先撤销再执行用户原有的成功 break，可保证返回 true 时也恢复现场。

## 问题二：下一层的行、列参数传反

声明的坐标顺序是 r, c，但递归调用写成 newC, newR。类型都是 int，能编译，却把列数当行数、行数当列数：

```cpp
// 错误
flag = backtracking(board, path, newC, newR, visited, word);

// 与声明及所选单元格一致
flag = backtracking(board, path, newR, newC, visited, word);
```

所选单元格和 visited 标记仍为 (newR,newC)，下一层却从 (newC,newR) 搜索。路径中的最后一个字符与用于找邻居的位置不再一致，可能漏掉合法路径，也可能连接不相邻的单元格。不应笼统声称这一行一定立即越界：原版访问邻居前还有边界检查，但当前搜索坐标已错。

例如网格两行分别为 "ABC"、"DEF"。选择 C=(0,2) 后，原版下一层坐标变成 (2,0)，它向上查看 D=(1,0)，可能构造 "CD"。C 与 D 实际并不相邻，却返回 true。

## 问题三：没有保持路径为 word 的前缀

if(path==word) 只判断是否完成。若 word="ABC"，当前路径已经是 "AX"，未来只能往后追加，不能把第二个字符 X 改为 B；这条分支不可能成功，应停止，而不是继续遍历邻居。

在起点匹配 word[0]、路径始终为合法前缀的前提下，下一个邻居需要匹配 word[path.size()]：

```cpp
// 放在边界合法、未访问的检查之后，加入邻居之前
if (board[newR][newC] != word[path.size()]) continue;
```

path.size() 表示已匹配多少个字符，也是下一字符在 word 中的下标。比如 path="AB" 时，下一字符为 word[2]='C'。保持上述前缀约束后，递归入口的 path==word 会在匹配完时立即返回，因此探索邻居时 path.size()<word.size()，索引合法。

如果尚未建立前缀约束，单独在入口补 if(path.size()>=word.size()) return false（放在成功检查之后）也能限制无用长度，但仍会搜索已经不匹配的前缀；不能代替逐字符匹配。

原版不是必须无限递归：visited 限制路径中已加入的单元格不重复，搜索最终能结束。但它没有在字符不匹配或长度无意义时及时停止，会枚举大量与目标无关的路径，网格变大后可能超时。

## 一次调用应维护的关系

当前 (r,c) 是已选择路径的最后一个单元格；path 是 word 的已匹配前缀；visited 标记且仅标记当前路径中的单元格。选邻居时同时加入字母、标记访问，再从同一个邻居坐标递归；返回后解除标记、弹出字母。

用户已有四方向定义、邻居边界检查及邻居 visited/path 成对撤销的框架可以保留。只需先针对起点、行列顺序与字符匹配三处修改，不必立即更换整套实现。

## 本轮诊断证据

按用户原版代码逐句建立 JavaScript 模型（包括 newC/newR 的错误顺序），检查以下小例子。是行为模型核验，不是原始 C++ 的编译运行，也不是修正版已通过的证明。

| board | word | 正确答案 | 首版模型结果 |
|---|---|---|---|
| 一格 "A" | A | true | false |
| 两行 "ABC"、"DEF" | ABC | true | false |
| 两行 "ABC"、"DEF" | CD | false | true |

第三例模型记录所选单元格为 (0,2)、(1,0)，证实不相邻的字母被错误连接。原版在该两行网格搜索不存在的 "ZZZ"，仍进入 151 次递归调用，说明没有首字母/前缀匹配的浪费；不据此给出大规模实测耗时。

相关语法：[形参、实参按位置对应](../01-cpp语法知识文档.md)。首版诊断阶段保持未提交；用户随后修正并确认通过，下文记录本轮复核及整理。

## 用户通过的 path 版本

```cpp
class Solution {
public:
    vector<pair<int, int>> directions = {
        {-1, 0}, // 上
        {1, 0},  // 下
        {0, -1}, // 左
        {0, 1},  // 右
    };

    bool backtracking(const vector<vector<char>>& board, string& path,
                      int r, int c, vector<vector<bool>>& visited,
                      const string& word) {
        if (path == word) return true;
        bool flag = false;

        for (int i = 0; i < 4; ++i) {
            int newR = r + directions[i].first;
            int newC = c + directions[i].second;
            if (newR >= 0 && newR < board.size() &&
                newC >= 0 && newC < board[0].size() &&
                visited[newR][newC] == false) {
                if (board[newR][newC] != word[(int)path.size()]) continue;
                path.push_back(board[newR][newC]);
                visited[newR][newC] = true;
                flag = backtracking(board, path, newR, newC, visited, word);
                visited[newR][newC] = false;
                path.pop_back();
                if (flag == true) break;
            }
        }
        return flag;
    }

    bool exist(vector<vector<char>>& board, string word) {
        int n = (int)board.size();
        int m = (int)board[0].size();
        string path;
        vector<vector<bool>> visited(n, vector<bool>(m, false));
        bool flag = false;

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                path.push_back(board[i][j]);
                visited[i][j] = true;
                flag = backtracking(board, path, i, j, visited, word);
                visited[i][j] = false;
                path.pop_back();
                if (flag == true) break;
            }
            if (flag == true) break;
        }
        return flag;
    }
};
```

这一版修正了起点选择与坐标顺序，并只追加下一字符匹配的邻居。成功后同样先恢复 visited/path 再跳出循环，恢复现场正确。

### 一个尚可优化的细节：起点首字母未筛选

用户通过版没有加入此前建议的 if(board[i][j]!=word[0]) continue。不同首字母的路径仍会尝试匹配 word[1]、word[2] 等，浪费搜索；它并非从每个入口都维持“完整 path 为 word 前缀”的不变量。若要保留 path 方案，先筛掉错误起点可建立这个不变量。

也不要把这版笼统判定为“必然越界”：若错误起点后面的字符恰好匹配完，path.size()==word.size() 但 path!=word，后续邻居检查可能读取 word[word.size()]。std::string 的该位置是可读取的终止字符 '\0'，题目网格只有字母，不能匹配它，因此不会继续增长路径。大于 size() 的下标仍非法，不能推广到任意下标，更不能推广到 vector。依据：[字符串下标访问](https://eel.is/c++draft/string.access)、[字符串终止字符](https://eel.is/c++draft/basic.string.general)。实际写题优先显式匹配与停止，不依赖终止字符作为隐式终止条件。

## 与所给官解的区别

用户提供 [力扣官方题解](https://leetcode.cn/problems/word-search/solutions/411613/dan-ci-sou-suo-by-leetcode-solution/) 的完整代码，本轮直接按贴出的代码对照；另行浏览标准草案只为核对上面的字符串边界细节。

| 对照点 | 用户通过版 | 所给官解 |
|---|---|---|
| 进度记录 | 保存 path，以 path.size() 得到下一字符下标 | 只保存当前待匹配下标 k |
| 当前字符何时加入/匹配 | 调用者先加入当前格；函数主要检查邻居下一字符 | 函数入口检查 board[i][j]==word[k] |
| 搜索完成 | path==word | 当前字符匹配且 k==word.size()-1 |
| 访问标记归谁管理 | 调用者标记/撤销所选格，起点也在 exist 处理 | 当前调用标记/撤销当前格，exist 只枚举起点 |
| 返回成功 | 多个 flag、循环 break 后返回 | exist 找到一个成功起点就 return true |

### 核心简化：只记录进度，不构造已有字符串

一次 dfs(r,c,k) 的任务：在当前路径的已用格约束下，从 (r,c) 开始尝试匹配 word[k...]。word[0..k-1] 已匹配，当前格尚待检查 word[k]。它不是“k 个字母已经含当前格匹配完”的意思。

在当前字符匹配后，官解 k 等于用户已加入当前格时的 path.size()-1；用户的 word[path.size()] 检查的是下一字符，对应官解下一层的 word[k+1]。不要把两个入口阶段混为一谈。

本题只问是否存在，不要求输出走过的路径，因此不用再保存那些已经确定的字符。省掉 path 参数、push_back/pop_back 和完整字符串比较；保留 visited，因为同一个单元格不能在当前路径重复使用。

### 官解一层的五步与恢复位置

1. 当前字符不等于 word[k]，返回 false：不是这条路径。
2. 当前字符匹配，且 k 是最后一个下标，返回 true：整个单词完成。
3. 标记 visited[r][c]=true：当前格供更深的调用避开。
4. 尝试四个邻居，下一层传 k+1；有一个成功即可停止尝试。
5. 撤销 visited[r][c]=false，返回本层结果。

最后一个字符成功时，当前调用还没有做第 3 步标记，所以第 2 步可以直接返回，没有属于本层的新标记需要恢复。祖先调用已经做了标记，接到成功后 break，仍执行末尾撤销，再继续把 true 传上去。

在已经标记后若想直接 return true，也可以先撤销再返回；不要跳过恢复现场。未恢复不一定导致这个只找一个答案的单次 exist 出错，但会破坏辅助函数返回时恢复路径状态的约定。

### 小例子：一行 ABC，word="ABC"

dfs(0,0,0) 检查 A 对 word[0]，标记 A；沿右边进入 dfs(0,1,1)，检查 B，标记 B；再沿右边进入 dfs(0,2,2)，检查 C，k 已为最后下标，返回 true。B 的调用解除 B 标记后返回 true；A 的调用解除 A 标记后返回 true。全程不用构造 "A"、"AB"、"ABC" 三个字符串。

### 其他差异不必机械照抄

- 用户 directions 是成员变量，已有只初始化一次的好处；官解在每次 check 中构造方向 vector，重复工作可以避免，整理版保留成员方向表。
- vector<vector<int>> 与 vector<vector<bool>> 都能表达是否访问过；本题只需两种状态，bool 版本可保留。它不是由 path 变 k 的关键。
- board/word 搜索中不修改，可以使用 const 引用。用户原版已有此写法，整理版沿用。
- exist 找到成功起点可立即 return true；前提是本次递归已完成自身状态恢复。无需维护两个嵌套循环的成功 break。
- k+1 是按值传递给下一层，不改变本层 k，无需 k--，与 22 括号生成传 open+1 相同。

## 用户随后写出的 k 下标版

下面保留用户原样逻辑，仅调整排版。这是用户独立修改的版本，不再需要 path；本地复核通过，不提前记为再次力扣提交通过。

```cpp
class Solution {
public:
    vector<pair<int, int>> directions = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };

    bool backtracking(const vector<vector<char>>& board, int k, int r, int c,
                      vector<vector<bool>>& visited, const string& word) {
        if (board[r][c] != word[k]) return false;
        if (k == static_cast<int>(word.size()) - 1 && board[r][c] == word[k]) {
            return true;
        }

        bool flag = false;
        for (int i = 0; i < 4; ++i) {
            int newR = r + directions[i].first;
            int newC = c + directions[i].second;
            if (newR >= 0 && newR < board.size() &&
                newC >= 0 && newC < board[0].size() &&
                visited[newR][newC] == false) {
                visited[newR][newC] = true;
                flag = backtracking(board, k + 1, newR, newC, visited, word);
                visited[newR][newC] = false;
                if (flag == true) return true;
            }
        }
        return flag;
    }

    bool exist(vector<vector<char>>& board, string word) {
        int n = (int)board.size();
        int m = (int)board[0].size();
        vector<vector<bool>> visited(n, vector<bool>(m, false));
        bool flag = false;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                visited[i][j] = true;
                flag = backtracking(board, 0, i, j, visited, word);
                visited[i][j] = false;
                if (flag == true) return true;
            }
        }
        return flag;
    }
};
```

### 逻辑正确：进度检查与访问标记各自一致

入口检查当前格是否匹配 word[k]，失败就停止，匹配最后一个字符就成功；只有尚未完成时才探索邻居 k+1。因此访问 word[k] 时 k 始终在合法范围内，所有起点也会检查 word[0]，解决了旧 path 版遗漏首字母筛选的问题。

用户仍保留“调用者管理所选格的标记”，没有完全改成官解的“本层管理当前格”：

| 标记分工 | 用户新 k 版 | 所给官解 |
|---|---|---|
| 起点 | exist 标记，调用后撤销 | exist 不标记，由 check 处理 |
| 邻居 | 父调用标记邻居，子调用返回后父调用撤销 | 子调用标记自己的当前格，返回前自己撤销 |
| 匹配失败或最后字符成功 | 直接返回，调用者仍会撤销当前格 | 返回前尚未标记当前格，无本层标记需撤销 |

两种分工都正确，关键是不要混用。用户入口的 visited[r][c] 已由调用者置 true，是当前路径的一部分；不应再简单加 if(visited[r][c]) return false，否则会拒绝每一个起点。若改为官解的本层管理方式，必须同时移除调用者的预先标记。

用户当前成功返回也会恢复现场，因为顺序是：

```cpp
visited[newR][newC] = true;
bool found = backtracking(board, k + 1, newR, newC, visited, word);
visited[newR][newC] = false; // 先撤销本层选择的邻居
if (found) return true;     // 再把成功传回去
```

本层返回时自己的当前格仍由父调用负责撤销；exist 也在成功返回前撤销起点。不是每一个提前 return 都有问题，而是“本层修改的共享状态是否恢复”要检查清楚。

### 保留框架即可简化的两处

第一处，入口已排除字符不匹配，成功条件无需重复比较：

```cpp
if (board[r][c] != word[k]) return false;
if (k == static_cast<int>(word.size()) - 1) return true;
```

第二处，两函数中只要 flag 为 true 就已经立即返回，因此循环正常结束时不可能还有成功分支；末尾 return flag 可以直接写 return false。flag 也可以按上面的小段代码定义在每次调用的位置，不必放在整个函数开头。都是简化，不是修复逻辑错误。

记忆：坐标决定在哪个格子，k 决定匹配哪个字母，visited 决定当前路径哪些格子不能再用；进入一个邻居就传 k+1。标记与撤销成对放在调用者，或者成对放在本层，只选一套一致的分工。

## 按上述任务定义整理的 k 版本

下面是助手的等价整理版，不是用户已经独立写出的新版。将边界、访问标记与字符匹配统一在入口检查，使每个起点与邻居调用使用相同逻辑。

```cpp
class Solution {
    const vector<pair<int, int>> directions = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };

    bool dfs(const vector<vector<char>>& board, const string& word,
             vector<vector<bool>>& visited, int r, int c, int k) {
        int rows = static_cast<int>(board.size());
        int cols = static_cast<int>(board[0].size());

        if (r < 0 || r >= rows || c < 0 || c >= cols) return false;
        if (visited[r][c] || board[r][c] != word[k]) return false;
        if (k == static_cast<int>(word.size()) - 1) return true;

        visited[r][c] = true;
        bool found = false;
        for (const auto& dir : directions) {
            if (dfs(board, word, visited,
                    r + dir.first, c + dir.second, k + 1)) {
                found = true;
                break;
            }
        }
        visited[r][c] = false;
        return found;
    }

public:
    bool exist(vector<vector<char>>& board, string word) {
        int rows = static_cast<int>(board.size());
        int cols = static_cast<int>(board[0].size());
        if (word.size() > static_cast<size_t>(rows * cols)) return false;

        vector<vector<bool>> visited(rows, vector<bool>(cols, false));
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (dfs(board, word, visited, r, c, 0)) return true;
            }
        }
        return false;
    }
};
```

word 长于格子总数可直接 false，因为每格最多使用一次；这条剪枝容易解释，可先记住。当前阶段先熟悉“一个下标记录进度、一个格子标记访问”，不急于叠加更多剪枝。

## 通过后的复核与复杂度

系统临时目录用 g++ -std=c++17 -O2 -D_GLIBCXX_ASSERTIONS 编译：用户通过的 path 版、用户随后写出的 k 版、所给官解、助手整理版，各对 13 个固定、2940 个穷举、200 个确定种子随机输入，共 3153 个输入，与独立广度优先枚举简单路径的参考一致。参考保存路径文字和位掩码，不用逐字符前缀剪枝，枚举到 word 长度为止。

固定用例含题目三个示例、单格成功/失败、不可重复用格、非方形网格、先前 ABC 漏报与 CD 误报反例、错误首字母但后续可匹配、重复字母使用不同格及大小写区分。穷举覆盖行数 1..2、列数 1..3 的全部 A/B 网格与长度 1..4 的全部 A/B 单词；随机覆盖最大 3×3 网格及长度 1..6 单词。不是对题目最大规模的所有输入做穷举。

全部复用同一组 Solution 对象，board 与 word 不变，用户 path 版辅助函数返回后 path 恢复、四版 visited 恢复均通过。用户新 k 版还直接检查辅助调用前后标记完全相同：调用者预先标记的起点仍保留，后代标记均撤销；调用者随后解除起点标记，整个 visited 为 false。当前记录为用户 path 版已力扣通过；用户新 k 版只收到代码，尚待力扣提交反馈，官解/助手版为本地参考。临时测试不进入仓库，不建题目脚手架。

令 L=word.length。k 方案最坏时间 O(mn×3^L)：枚举 mn 个起点，第一步最多 4 个方向，后续不能回到上一格，最多 3 个继续方向；已访问约束和边界还能减少分支。辅助空间 O(mn+L)，包括 visited 与递归栈。指数最坏搜索不会因删掉 path 就变成线性；简化主要减少冗余状态与操作。

记忆：先定义 dfs(r,c,k) 从当前格匹配 word[k...]；不匹配就退，匹配到末尾就成；未完成则标记、找四邻、撤销；外层每格都能当起点。与 17 电话号码题类似，k 固定当前目标字符，循环只枚举本层格子候选，不枚举 word 的其他位置。
