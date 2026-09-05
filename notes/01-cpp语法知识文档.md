# C++ 语法与标准库速查（个人版）

> 用途：集中记录刷题过程中实际遇到的 C++ 语法、类型和标准库 API 问题。后续遇到新问题时，继续补充到对应分类。

## 目录

1. [STL 容器与类型](#1-stl-容器与类型)
2. [`<algorithm>` 常用算法](#2-algorithm-常用算法)
3. [其他重要语法](#3-其他重要语法)
4. [快速查询表](#4-快速查询表)

## 1. STL 容器与类型

### 1.1 `vector`：动态数组

#### 初始化

```cpp
vector<int> nums = {1, 2, 3};
vector<int> zeros(5, 0);  // 5 个 0
```

#### 常用操作

```cpp
nums.size();       // 元素数量，返回 size_t
nums.empty();      // 是否为空
nums.push_back(4); // 在末尾添加元素
nums.pop_back();   // 删除末尾元素，不返回被删除的值
nums.front();      // 第一个元素
nums.back();       // 最后一个元素
nums[i];           // 不做越界检查
nums.at(i);        // 越界时抛出异常
nums.clear();      // 删除全部元素
```

#### `reserve` 与 `resize`

```cpp
nums.reserve(100); // 只预留容量，nums.size() 不变
nums.resize(100);  // 元素数量变为 100
```

- `reserve` 主要用于减少多次扩容
- `resize` 会实际增加或删除元素

### 1.2 `string`：字符串

```cpp
string s = "hello";

s.size();          // 字符数量，返回 size_t
s.empty();         // 是否为空
s[0];              // 读取或修改字符，不检查越界
s.at(0);           // 越界时抛出异常
s.push_back('!');  // 末尾添加字符
s.pop_back();      // 删除末尾字符
```

#### 截取子串

```cpp
string part = s.substr(start, length);
string suffix = s.substr(start); // 从 start 一直到末尾
```

`substr` 的第二个参数是长度，不是结束下标。

### 1.3 `unordered_map`：哈希表

#### 统计字符出现次数

```cpp
unordered_map<char, int> need;

for (char c : t) {
    need[c]++;
}
```

`need[c]++` 的过程：

1. 如果 `c` 不存在，`operator[]` 自动插入这个 key
2. `int` 的默认值为 `0`
3. 再执行自增

#### key 的数量

```cpp
size_t keyCount = need.size();
int required = static_cast<int>(need.size());
```

例如 `t = "AABC"`：

```text
t.size()    == 4  // 字符总数
need.size() == 3  // A、B、C 三个不同 key
```

#### 判断 key 是否存在

C++17 写法：

```cpp
if (need.count(c) > 0) {
    // c 存在
}
```

或者：

```cpp
auto it = need.find(c);
if (it != need.end()) {
    cout << it->second;
}
```

不要为了判断存在性直接访问 `need[c]`：

```cpp
if (need[c] > 0) {
    // c 不存在时，need[c] 会把它插入哈希表
}
```

#### 遍历哈希表

```cpp
for (const auto& [key, value] : need) {
    cout << key << ": " << value << '\n';
}
```

这是 C++17 的结构化绑定语法。

如果不使用结构化绑定，可以把每个元素看作 `pair<const Key, T>`：

```cpp
for (const auto& entry : need) {
    cout << entry.first << ": " << entry.second << '\n';
}
```

使用迭代器时，迭代器指向键值对，因此通过 `->first` 和 `->second` 访问：

```cpp
for (auto it = need.begin(); it != need.end(); ++it) {
    cout << it->first << ": " << it->second << '\n';
}
```

如果要修改 value，可以使用非常量引用；key 仍然不能修改：

```cpp
for (auto& [key, value] : need) {
    value++;
}
```

#### 常用操作

```cpp
need.size();       // key 数量
need.empty();      // 是否为空
need.erase(c);     // 删除 key c
need.clear();      // 清空
```

`unordered_map` 的查找、插入和删除平均为 O(1)，最坏情况下可能退化。

### 1.4 `unordered_set`：无序集合

`unordered_set` 只保存 key，不保存 value，插入重复元素时会自动去重：

```cpp
unordered_set<int> uset;
uset.insert(3);
uset.insert(3); // 不会产生第二个 3
```

#### 遍历 `unordered_set`

最常用的是范围 `for`：

```cpp
for (const auto& value : uset) {
    cout << value << '\n';
}
```

也可以使用迭代器：

```cpp
for (auto it = uset.begin(); it != uset.end(); ++it) {
    cout << *it << '\n';
}
```

`unordered_set` 的遍历顺序不保证有序，也不保证每次运行都相同。如果需要升序结果，应复制到 `vector` 后排序，或直接使用 `set`。

#### 常用操作

```cpp
uset.count(value); // 存在返回 1，不存在返回 0
uset.find(value);  // 返回迭代器，找不到时返回 end()
uset.erase(value);
uset.size();
uset.empty();
uset.clear();
```

集合元素不能通过迭代器直接修改，因为修改后可能破坏哈希结构；需要删除旧值再插入新值。

### 1.5 `queue`：队列

```cpp
queue<int> que;

que.push(10);             // 队尾加入元素
int current = que.front();// 读取队首
que.pop();                // 删除队首
```

注意：

- `front()` 读取队首元素，但不删除
- `pop()` 删除队首元素，返回 `void`
- 不能写 `int x = que.pop()`
- BFS 中忘记 `pop()` 会反复处理同一个元素，造成死循环或 TLE

典型 BFS 取队首写法：

```cpp
while (!que.empty()) {
    int current = que.front();
    que.pop();

    // 处理 current
}
```

### 1.6 `priority_queue`：优先队列 / 堆

使用 `priority_queue` 需要包含：

```cpp
#include <queue>
```

#### 默认是大顶堆

```cpp
priority_queue<int> maxHeap;

maxHeap.push(3);
maxHeap.push(1);
maxHeap.push(5);

int largest = maxHeap.top(); // 5，只读取堆顶
maxHeap.pop();               // 删除堆顶，不返回元素
```

常用接口：

```cpp
maxHeap.push(value); // 插入元素
maxHeap.emplace(value);
maxHeap.top();       // 读取优先级最高的元素
maxHeap.pop();       // 删除堆顶，返回 void
maxHeap.empty();     // 是否为空
maxHeap.size();      // 元素数量，返回 size_t
```

和 `queue` 一样，`pop()` 只删除、不返回元素。应该先 `top()`，再 `pop()`：

```cpp
while (!maxHeap.empty()) {
    int current = maxHeap.top();
    maxHeap.pop();

    // 处理 current
}
```

调用 `top()` 或 `pop()` 前必须确认堆不为空。

#### 基本类型的小顶堆

`priority_queue` 的三个模板参数依次是元素类型、底层容器和比较器：

```cpp
#include <functional>
#include <vector>

priority_queue<int, vector<int>, greater<int>> minHeap;
```

此时 `minHeap.top()` 返回最小值。

```cpp
minHeap.push(3);
minHeap.push(1);
minHeap.push(5);

int smallest = minHeap.top(); // 1
```

#### 自定义类型的小顶堆

存放指针或自定义结构体时，需要提供比较器。例如按照链表节点的 `val` 建立小顶堆：

```cpp
struct CompareListNode {
    bool operator()(const ListNode* left, const ListNode* right) const {
        return left->val > right->val;
    }
};

priority_queue<
    ListNode*,
    vector<ListNode*>,
    CompareListNode
> minHeap;
```

比较器最容易写反。可以这样理解：

```text
Compare(left, right) 返回 true
表示 left 的优先级比 right 低
```

所以小顶堆使用 `left->val > right->val`：值更大的节点优先级更低，值最小的节点就位于堆顶。值相等时顺序不确定，但不影响有序结果。

第 23 题中，只把每条非空链表当前的头节点放入最小堆：

```cpp
for (ListNode* head : lists) {
    if (head != nullptr) {
        minHeap.push(head);
    }
}
```

每次取出堆顶节点后，如果它还有下一个节点，就将 `node->next` 放入堆中。堆中最多保存 `k` 个节点。

#### 复杂度和限制

- `top()`：O(1)
- `push()` / `emplace()`：O(log n)
- `pop()`：O(log n)
- `empty()` / `size()`：O(1)
- 不支持像 `vector` 一样通过下标访问，也不提供直接遍历接口

### 1.7 STL 容器的共同规则

#### `size()` 通常返回 `size_t`

```cpp
size_t vectorLength = nums.size();
size_t stringLength = s.size();
size_t keyCount = need.size();
```

#### `begin()` 与 `end()`

```cpp
nums.begin(); // 指向第一个元素
nums.end();   // 指向最后一个元素之后的位置
```

`end()` 不指向有效元素，不能直接解引用。

## 2. `<algorithm>` 常用算法

使用这些算法时应包含：

```cpp
#include <algorithm>
```

### 2.1 迭代器区间：左闭右开

标准库算法通常接收 `[first, last)`：

```text
包含 first 指向的元素
不包含 last 指向的元素
```

例如操作下标 `[left, right]`：

```cpp
reverse(nums.begin() + left, nums.begin() + right + 1);
```

### 2.2 `swap`：交换

```cpp
swap(nums[left], nums[right]);
```

用于原地交换两个类型兼容的对象。

### 2.3 `reverse`：反转区间

```cpp
reverse(nums.begin(), nums.end());
reverse(nums.begin() + start, nums.end());
```

`reverse` 会原地修改容器。

### 2.4 `sort`：排序

```cpp
sort(nums.begin(), nums.end());
```

默认升序。题目明确禁止 `sort` 时不能使用。

自定义降序：

```cpp
sort(nums.begin(), nums.end(), greater<int>());
```

### 2.5 `min` 与 `max`

```cpp
int smaller = min(a, b);
int larger = max(a, b);
```

更新最优答案时常见：

```cpp
answer = min(answer, currentLength);
```

两个参数的类型应兼容，避免一个是 `int`、另一个是 `size_t` 导致模板推导失败。

## 3. 其他重要语法

### 3.1 函数参数：值、引用与 `const` 引用

#### 按值传递：得到一份副本

```cpp
void work(vector<int> nums) {
    nums[0] = 100;
}
```

修改 `nums` 不影响调用者，但复制整个 `vector` 需要 O(n) 时间和空间。

#### 非 `const` 引用：修改原对象

```cpp
void sortColors(vector<int>& nums) {
    nums[0] = 0;
}
```

- `&` 表示引用，不复制容器
- 函数内修改会作用到调用者传入的对象
- “原地修改”题目经常使用这种参数

#### `const` 引用：只读且不复制

```cpp
void printVector(const vector<int>& nums) {
    cout << nums.size();
}
```

只读的复杂对象参数通常优先使用 `const T&`。

### 3.2 花括号初始化与函数参数对应

```cpp
static void check(vector<int> nums, int expected);

check({1, 1, 2}, 1);
```

这里不是传入两个数组：

- `{1, 1, 2}` 构造第一个参数 `vector<int> nums`
- 第二个 `1` 是 `int expected`

### 3.3 范围 `for` 循环

#### 读取元素副本

```cpp
for (int x : nums) {
    cout << x;
}
```

修改 `x` 不会改变 `nums`。

#### 修改容器元素

```cpp
for (int& x : nums) {
    x *= 2;
}
```

#### 只读且避免复制复杂对象

```cpp
for (const string& word : words) {
    cout << word;
}
```

`char`、`int` 等小类型通常直接按值读取：

```cpp
for (char c : t) {
    // 使用 c
}
```

### 3.4 `size_t`、`int` 与类型转换

`vector::size()` 和 `string::size()` 返回无符号类型 `size_t`。

`static_cast<目标类型>(值)` 是 C++ 的显式类型转换语法。它会产生一个转换后的新值，不会修改原变量：

```cpp
size_t count = need.size();
int required = static_cast<int>(count);
```

在 `int required = static_cast<int>(need.size());` 中，`need.size()` 原本是 `size_t`，`static_cast<int>` 明确地把它转换为 `int`，再赋值给 `required`。直接写 `int required = need.size();` 通常也能编译，但属于隐式转换，可能产生编译器警告；显式转换能清楚表达“这里有意改变类型”。

只有在确定数值能放进目标类型时才应这样转换。LeetCode 76 中最多只有有限种英文字母，`need.size()` 很小，因此转换为 `int` 是安全的。现代 C++ 通常优先使用 `static_cast<int>(value)`，而不是含义不够明确的 C 风格写法 `(int)value`。

#### 为什么现代 C++ 不优先使用 `(int)value`

C 风格强制转换仍然是合法语法，但一次 `(目标类型)value` 可能承担普通数值转换、去除 `const`、指针重新解释等多种不同含义，代码本身没有清楚表达转换意图，也可能接受过于危险的转换。C++ 的命名转换把用途拆开，例如 `static_cast` 用于常规且编译期可检查的转换，`const_cast` 专门处理 `const`，`reinterpret_cast` 表示底层指针或位层面的重新解释。

```cpp
double value = 3.8;
int a = (int)value;                  // 合法，但转换种类不够明确
int b = static_cast<int>(value);    // 明确表示普通数值转换
```

对于不相关的指针类型，C 风格转换可能直接接受，而 `static_cast` 会拒绝：

```cpp
double* source = nullptr;
int* p1 = (int*)source;                    // 可能编译，但非常危险
// int* p2 = static_cast<int*>(source);     // 编译错误，阻止不合理转换
```

命名转换也更容易在代码中搜索，例如可以直接搜索 `static_cast` 找出全部常规显式转换。结论不是“永远不能使用 C 风格转换”，而是在现代 C++ 代码中，优先选用能准确表达意图、让编译器提供更多检查的命名转换。

如果下标需要递减到 `-1`，应使用有符号整数，并先转换再做减法：

```cpp
int i = static_cast<int>(nums.size()) - 2;

while (i >= 0) {
    i--;
}
```

错误写法：

```cpp
size_t i = nums.size() - 2;
while (i >= 0) {
    i--; // size_t 不会变成 -1，会发生无符号下溢
}
```

也要避免转换前先做可能下溢的减法：

```cpp
int i = static_cast<int>(nums.size() - 2); // 减法先以 size_t 计算
```

#### 前置 `--`、后置 `--` 与比较

表达式：

```cpp
if (--freq[sum] == 0) {
    freq.erase(sum);
}
```

按语法可理解为：

```cpp
if ((--freq[sum]) == 0) {
    //...
}
```

`freq[sum]` 先取得哈希表中对应值，前置递减 `--` 立即把它减 1，并把“减完后的值”作为表达式结果；随后这个新值再与 `0` 比较。若原值是 `1`，比较的是 `0 == 0`，条件成立，然后删除 key。

后置递减的含义不同：

```cpp
if (freq[sum]-- == 0) {
    // 比较递减前的旧值
}
```

若原值是 `1`，条件比较 `1 == 0` 为假，比较之后才把值减为 `0`。因此回溯时需要“先减再判断”，应使用前置 `--`，或者拆开写得更直观：

```cpp
--freq[sum];
if (freq[sum] == 0) {
    freq.erase(sum);
}
```

这里 `--` 的优先级高于 `==`，所以表达式分组为 `(--freq[sum]) == 0`；不过理解这类代码时，重点是前置递减返回新值，后置递减返回旧值，而不是只依赖运算符优先级。

### 3.5 `if`、`else if` 与独立判断

`else if` 只有在前面的条件不成立时才执行。

错误示例：选择候选人后，本轮投票被跳过：

```cpp
if (count == 0) {
    candidate = nums[i];
} else if (nums[i] == candidate) {
    count++;
}
```

如果两个动作在同一轮都可能执行，应使用两个独立的 `if`：

```cpp
if (count == 0) {
    candidate = nums[i];
}

if (nums[i] == candidate) {
    count++;
} else {
    count--;
}
```

### 3.6 `break`、`continue` 与 `return`

#### `break` 只结束最内层循环

```cpp
for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < cols; ++col) {
        if (condition) {
            break; // 只结束 col 循环
        }
    }
}
```

#### `continue` 跳过当前这一轮

```cpp
for (int x : nums) {
    if (x < 0) {
        continue;
    }
    cout << x;
}
```

#### `return` 结束整个函数

```cpp
if (answerFound) {
    return answer;
}
```

### 3.7 作用域与变量遮蔽

不要在内层作用域重新声明同名变量：

```cpp
for (int i = 0; i < rows; ++i) {
    for (int i = 0; i < cols; ++i) { // 内层 i 遮蔽外层 i
    }
}
```

应使用不同且有含义的名字：

```cpp
for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < cols; ++col) {
    }
}
```

### 3.8 `auto` 与结构化绑定

`auto` 让编译器根据右侧表达式推导类型：

```cpp
auto it = need.find(c);
```

C++17 可以拆解键值对：

```cpp
for (const auto& [key, value] : need) {
    cout << key << value;
}
```

`const auto&` 表示只读引用，可以避免复制每个键值对。

### 3.9 头文件与命名空间

刷题环境常见简写：

```cpp
#include <bits/stdc++.h>
using namespace std;
```

`<bits/stdc++.h>` 是 GCC/Clang 常见的非标准聚合头文件。更可移植的写法是按需包含：

```cpp
#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>
```

## 4. 快速查询表

### STL

| 需求 | 写法 | 注意 |
|---|---|---|
| `vector` 元素数量 | `nums.size()` | 返回 `size_t` |
| `string` 截取子串 | `s.substr(start, length)` | 第二个参数是长度 |
| 哈希表 key 数量 | `need.size()` | 不是所有 value 的总和 |
| 判断哈希 key | `need.count(c) > 0` | 不会插入 key |
| 查找哈希 key | `need.find(c) != need.end()` | 可通过迭代器读取 value |
| 哈希计数 | `need[c]++` | 不存在时自动插入并初始化为 0 |
| 队列读取队首 | `que.front()` | 不删除元素 |
| 队列删除队首 | `que.pop()` | 返回 `void` |
| 大顶堆 | `priority_queue<int> heap` | 默认堆顶是最大值 |
| 小顶堆 | `priority_queue<int, vector<int>, greater<int>> heap` | 堆顶是最小值 |
| 读取堆顶 | `heap.top()` | 不删除元素，调用前检查非空 |
| 删除堆顶 | `heap.pop()` | 返回 `void` |

### `<algorithm>`

| 需求 | 写法 | 注意 |
|---|---|---|
| 原地交换 | `swap(a, b)` | 两个对象类型需兼容 |
| 反转区间 | `reverse(first, last)` | 操作 `[first, last)` |
| 升序排序 | `sort(first, last)` | 操作 `[first, last)` |
| 取较小值 | `min(a, b)` | 参数类型应兼容 |
| 取较大值 | `max(a, b)` | 参数类型应兼容 |

### 其他语法

| 需求 | 写法 | 注意 |
|---|---|---|
| 只读且不复制参数 | `const T& value` | 函数内不能修改 |
| 修改调用者对象 | `T& value` | 修改会作用于原对象 |
| 显式类型转换 | `static_cast<int>(value)` | 转换后再进行有符号运算 |
| 范围循环中修改元素 | `for (auto& x : values)` | 必须使用引用 |

## 维护约定

- 新语法问题优先补充到对应的 STL、`<algorithm>` 或其他语法章节
- STL 新容器单独增加三级标题，例如 `stack`、`set`、`map`
- 如果问题来自具体题目，保留能够复现问题的最小示例
- 算法原理仍写在各题复盘中；本文只记录 C++ 语言、类型和标准库用法
