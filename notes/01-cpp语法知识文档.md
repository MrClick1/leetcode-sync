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

#### 一次添加多个元素

`push_back` 一次只能添加一个 `vector` 元素。`vector<int>` 的元素类型是 `int`：

```cpp
vector<int> nums;
nums.push_back(1);
nums.push_back(2);
nums.push_back(3);
```

如果想用一条语句连续添加多个整数，可以使用 `insert` 和花括号初始化列表：

```cpp
nums.insert(nums.end(), {1, 2, 3});
```

`{1, 2, 3}` 是花括号初始化列表，具体类型由上下文决定；这里会作为 `initializer_list<int>` 传给 `insert`。它不是一个可以脱离上下文单独声明的固定容器类型。

如果外层容器的元素类型本身是 `vector<int>`，则 `push_back` 可以接收一个 `vector<int>`：

```cpp
vector<vector<int>> groups;
groups.push_back({1, 2, 3}); // 添加一个 vector<int>
```

但下面的写法不成立，因为 `nums.push_back` 需要一个 `int`，而不是三个 `int`：

```cpp
// vector<int> nums;
// nums.push_back({1, 2, 3});
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

函数形式：

```cpp
string substr(size_t start = 0, size_t length = string::npos) const;
```

最重要的含义：

```text
start  = 从哪个下标开始截取
length = 最多截取多少个字符
```

`substr` 的第二个参数是长度，不是结束下标。

```cpp
string s = "hello";

string a = s.substr(1, 3); // "ell"：从下标 1 开始，取 3 个字符
string b = s.substr(2);    // "llo"：从下标 2 一直取到末尾
string c = s.substr();     // "hello"：默认从 0 开始取到末尾
```

下标示意：

```text
字符： h e l l o
下标： 0 1 2 3 4

s.substr(1, 3)
             └─ 从下标 1 开始取 3 个字符：e、l、l
```

##### 根据左右边界计算 `length`

如果窗口使用闭区间 `[left, right]`，包含左右两个端点：

```cpp
string part = s.substr(left, right - left + 1);
```

例如：

```cpp
string s = "abcdef";
int left = 1;
int right = 3;

string part = s.substr(left, right - left + 1); // "bcd"
```

如果窗口使用半开区间 `[left, right)`，不包含 `right`：

```cpp
string part = s.substr(left, right - left);
```

记忆：

```text
闭区间长度   = right - left + 1
半开区间长度 = right - left
```

##### `substr` 返回新字符串

`substr` 不会修改原字符串，而是返回截取出来的新字符串：

```cpp
string s = "hello";
string part = s.substr(1, 3);

// s 仍然是 "hello"
// part 是 "ell"
```

如果希望保存结果，必须接收返回值：

```cpp
string answer = s.substr(start, length);
```

##### 长度超过末尾不会报错

如果 `start` 合法，但 `length` 超过剩余字符数量，`substr` 会自动截取到字符串末尾：

```cpp
string s = "hello";
string part = s.substr(3, 100); // "lo"
```

因此下面两种写法效果相同：

```cpp
s.substr(2);
s.substr(2, string::npos);
```

`string::npos` 是一个特殊值，在这里表示“一直到末尾”。

##### `start` 越界会抛出异常

合法起点范围是：

```text
0 <= start <= s.size()
```

- `start < s.size()`：从对应字符开始截取。
- `start == s.size()`：返回空字符串。
- `start > s.size()`：抛出 `std::out_of_range`。

```cpp
string s = "hello";

s.substr(5); // ""
s.substr(6); // 抛出 std::out_of_range
```

尤其要注意，`substr` 的参数类型是无符号的 `size_t`。如果把 `-1` 作为起点：

```cpp
int bestStart = -1;
string answer = s.substr(bestStart, bestLength);
```

`-1` 会转换成一个非常大的无符号整数，因此可能看到类似错误：

```text
basic_string::substr: __pos (which is 18446744073709551615)
> this->size()
```

在“最小覆盖子串”一类题中，无解时应先判断哨兵值：

```cpp
return bestStart == -1
    ? ""
    : s.substr(bestStart, bestLength);
```

##### 常见错误

错误一：把第二个参数当成结束下标。

```cpp
// 想截取闭区间 [left, right]
s.substr(left, right);              // 错误理解
s.substr(left, right - left + 1);   // 正确
```

错误二：无解时直接使用 `-1` 作为起点。

```cpp
int start = -1;
// s.substr(start, length); // 可能抛出 out_of_range
```

错误三：以为 `substr` 会修改原字符串。

```cpp
s.substr(1, 3); // 返回值没有接收，s 不会发生变化
```

##### 复杂度

构造结果字符串需要复制截取到的字符，因此时间和额外空间通常都是：

```text
O(截取结果的长度)
```

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

使用时包含头文件：

```cpp
#include <unordered_set>
```

`unordered_set<T>` 只保存类型为 `T` 的 key，不保存额外的 value。集合中的元素不会重复，并且不保证遍历顺序。

```cpp
unordered_set<int> numbers;
unordered_set<char> window;
unordered_set<string> words;
```

#### `insert`：插入元素

最常见的写法：

```cpp
unordered_set<int> uset;
uset.insert(3);
uset.insert(3); // 不会产生第二个 3
```

`insert` 的返回值是：

```cpp
pair<iterator, bool>
```

- `first`：指向集合中该元素的迭代器。
- `second`：本次是否真的插入了新元素。

```cpp
auto result = uset.insert(3);

if (result.second) {
    // 原来不存在 3，本次插入成功
} else {
    // 原来已经存在 3，没有重复插入
}
```

C++17 可以使用结构化绑定：

```cpp
auto [it, inserted] = uset.insert(3);
if (inserted) {
    cout << "插入成功";
}
```

如果不关心是否插入成功，可以直接写：

```cpp
uset.insert(3);
```

还可以使用 `emplace`，根据参数直接构造元素：

```cpp
unordered_set<string> words;
words.emplace("hello");
```

对于 `int`、`char` 这类简单类型，`insert` 已经足够直观。

#### `erase`：删除元素

按照值删除：

```cpp
uset.erase(3);
```

如果元素不存在，`erase` 不会报错。按照值删除时，返回实际删除的元素数量；对 `unordered_set` 来说只能是 `0` 或 `1`：

```cpp
size_t removed = uset.erase(3);

if (removed == 1) {
    // 删除成功
} else {
    // 集合中原本没有 3
}
```

也可以按照迭代器删除：

```cpp
auto it = uset.find(3);
if (it != uset.end()) {
    uset.erase(it);
}
```

遍历时删除元素，不能在删除后继续使用已经失效的旧迭代器。可以接收 `erase` 返回的下一个迭代器：

```cpp
for (auto it = uset.begin(); it != uset.end();) {
    if (*it < 0) {
        it = uset.erase(it);
    } else {
        ++it;
    }
}
```

#### `count` 与 `find`：判断元素是否存在

`count` 返回集合中指定元素的数量。因为 `unordered_set` 不允许重复，所以结果只有 `0` 或 `1`：

```cpp
if (uset.count(3)) {
    // 存在 3
}

if (!uset.count(3)) {
    // 不存在 3
}
```

`find` 返回迭代器：

```cpp
auto it = uset.find(3);

if (it != uset.end()) {
    // 找到了，*it 就是 3
} else {
    // 没找到
}
```

只需要判断“是否存在”时，`count` 更简洁；找到后还要使用或删除对应迭代器时，使用 `find`。

C++20 还提供：

```cpp
uset.contains(3);
```

但使用 C++17 时应继续使用 `count` 或 `find`。

#### 在滑动窗口中的用法

第 3 题“无重复字符的最长子串”中：

```cpp
unordered_set<char> window;
```

集合表示当前窗口中已经出现的字符。

```cpp
while (window.count(s[right])) {
    window.erase(s[left]);
    ++left;
}

window.insert(s[right]);
```

对应关系是：

```text
count  -> 检查字符是否已经在窗口中
erase  -> 左边界移动时，删除离开窗口的字符
insert -> 将右边界的新字符加入窗口
```

窗口始终无重复，所以这里只需要集合，不需要使用 `unordered_map<char, int>` 记录频次。

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
uset.insert(value); // 插入；已经存在时不会重复插入
uset.count(value); // 存在返回 1，不存在返回 0
uset.find(value);  // 返回迭代器，找不到时返回 end()
uset.erase(value); // 删除；不存在时不会报错
uset.size();        // 元素数量
uset.empty();       // 是否为空
uset.clear();       // 删除全部元素
```

`unordered_set` 没有 `operator[]`，不能写 `uset[key]`。集合元素也不能通过迭代器直接修改，因为修改 key 后可能破坏哈希结构；需要删除旧值再插入新值。

插入、查找和删除的平均时间复杂度为 `O(1)`，极端哈希冲突时最坏可能退化为 `O(n)`。

### 1.5 `queue`：队列

#### 队列、双端队列与栈：操作速查（2026-09-18 复习）

把队首放在左边、队尾放在右边：普通队列从右边加入、左边取出；双端队列两边都可以操作。

| 操作 | `queue<int> q` | `deque<int> dq` | `stack<int> st`（对照） |
|---|---|---|---|
| 加入 | `q.push(x)`：队尾 | `dq.push_front(x)` / `dq.push_back(x)` | `st.push(x)`：栈顶 |
| 查看 | `q.front()` / `q.back()` | `dq.front()` / `dq.back()` | `st.top()` |
| 删除 | `q.pop()`：队首 | `dq.pop_front()` / `dq.pop_back()` | `st.pop()`：栈顶 |
| 判空 | `q.empty()` | `dq.empty()` | `st.empty()` |
| 数量 | `q.size()` | `dq.size()` | `st.size()` |
| 下标访问 | 不支持 | `dq[i]` / `dq.at(i)` | 不支持 |

记忆：`front/back/top` 负责读取；`pop` 系列负责删除，返回 `void`。读取和删除都应保证容器非空。`queue` 没有 `top()`，`deque` 没有不带方向的 `push()` 或 `pop()`。

```cpp
queue<int> q;
q.push(10);             // [10]
q.push(20);             // [10, 20]
q.push(30);             // [10, 20, 30]
int current = q.front(); // current = 10，队列不变
q.pop();                // [20, 30]
```

先读取再删除，和前序遍历中先 `st.top()` 再 `st.pop()` 的结构一致。但栈后进先出，队列先进先出：树的这份前序迭代代码用栈，按层遍历通常用队列，不能直接替换而期待相同顺序。

`queue` 是先进先出（FIFO）的容器：最早进入队列的元素最先离开。使用时需要包含：

```cpp
#include <queue>

queue<int> que;
queue<TreeNode*> nodes;
queue<pair<int, int>> positions;
```

#### 常用操作

```cpp
que.push(10);       // 在队尾加入 10，返回 void
que.emplace(20);    // 在队尾直接构造元素

int first = que.front(); // 读取队首，不删除
int last = que.back();   // 读取队尾，不删除

que.pop();           // 删除队首，返回 void
que.empty();         // 是否为空
que.size();          // 元素数量，返回 size_t
```

操作顺序可以记成：

```text
push / emplace → 从 back 进入
front          → 查看最早进入的元素
pop            → 删除最早进入的元素
```

#### 正确取出队首

`pop()` 只删除元素，不返回元素，因此不能写：

```cpp
// int current = que.pop(); // 错误，pop() 返回 void
```

应该先读取，再删除：

```cpp
while (!que.empty()) {
    int current = que.front();
    que.pop();

    // 处理 current
}
```

调用 `front()`、`back()` 或 `pop()` 前必须确认队列非空，否则属于未定义行为：

```cpp
if (!que.empty()) {
    int current = que.front();
    que.pop();
}
```

如果需要在 `pop()` 后继续使用队首值，应先复制出来；对 `que.front()` 返回元素取得的引用会在该元素被删除后失效。若队列保存的是指针，复制出的指针值是否仍然有效，取决于它所指对象的生命周期。

#### `pair` 入队与结构化绑定

网格 BFS 经常把行列坐标放入队列：

```cpp
queue<pair<int, int>> que;

que.push({row, col}); // 构造 pair 后入队
que.emplace(row, col);// 直接在队尾构造 pair

auto [r, c] = que.front();
que.pop();
```

#### 普通 BFS 模板

```cpp
queue<int> que;
que.push(start);

while (!que.empty()) {
    int current = que.front();
    que.pop();

    for (int next : graph[current]) {
        if (visited[next]) continue;

        visited[next] = true;
        que.push(next);
    }
}
```

通常应在节点入队时立刻标记 `visited`，避免同一个节点被不同来源重复加入队列。

#### 按层 BFS 模板

每轮开始时保存当前层元素数量：

```cpp
int depth = 0;

while (!que.empty()) {
    int levelSize = static_cast<int>(que.size());

    for (int i = 0; i < levelSize; ++i) {
        int current = que.front();
        que.pop();

        // 将下一层节点加入 que
    }

    ++depth;
}
```

必须先保存 `levelSize`。处理当前层时队列会不断加入下一层节点，如果循环条件直接使用不断变化的 `que.size()`，层级边界会混在一起。

#### 遍历和清空限制

`queue` 是容器适配器，不提供迭代器，也不能使用下标或范围 `for`：

```cpp
// que[0];                 // 不支持
// for (int x : que) {}    // 不支持
```

如果只是依次处理并清空队列，可以不断读取队首并 `pop()`：

```cpp
while (!que.empty()) {
    cout << que.front() << '\n';
    que.pop();
}
```

`queue` 没有 `clear()`。若只想清空，也可以交换一个同类型的空队列：

```cpp
queue<int> emptyQueue;
que.swap(emptyQueue);
```

#### 常见错误

- 忘记 `pop()`：反复处理同一个队首，可能造成死循环或 TLE。
- 空队列调用 `front()`、`back()` 或 `pop()`：未定义行为。
- 把 `front()` 当成删除操作：它只读取，不会改变队列。
- 认为 `pop()` 会返回被删除值：它返回 `void`。
- 在按层 BFS 中使用实时变化的 `que.size()` 作为当前层循环边界。
- 节点出队时才标记访问，导致同一节点被重复入队；一般在入队时标记。

### 1.6 `deque`：双端队列

`deque` 是 double-ended queue（双端队列），可以在队首和队尾高效地插入、删除元素：

```cpp
#include <deque>

deque<int> deq;
deque<int> values = {1, 2, 3};
```

#### 两端操作

先用一个完整例子记住方向。以下方括号左侧表示队首，右侧表示队尾：

```cpp
deque<int> dq;
dq.push_back(20);   // [20]
dq.push_front(10);  // [10, 20]
dq.push_back(30);   // [10, 20, 30]

int first = dq.front(); // 10，只读取
int last = dq.back();   // 30，只读取

dq.pop_front();     // [20, 30]
dq.pop_back();      // [20]
```

`push/pop` 表示加入/删除，`front/back` 表示队首/队尾。双端队列的修改操作把这两部分组合起来记忆即可。

```cpp
deq.push_front(1);    // 队首加入元素
deq.push_back(2);     // 队尾加入元素
deq.emplace_front(0); // 在队首直接构造元素
deq.emplace_back(3);  // 在队尾直接构造元素

int first = deq.front(); // 读取队首，不删除
int last = deq.back();   // 读取队尾，不删除

deq.pop_front();      // 删除队首，返回 void
deq.pop_back();       // 删除队尾，返回 void
```

和 `queue::pop()` 一样，`pop_front()`、`pop_back()` 都只删除、不返回元素。调用 `front()`、`back()`、`pop_front()` 或 `pop_back()` 前必须先确认容器非空。

#### 容量、清空与随机访问

```cpp
deq.empty();     // 是否为空
deq.size();      // 元素数量，返回 size_t
deq.clear();     // 删除全部元素

deq[i];          // 随机访问，不检查越界
deq.at(i);       // 随机访问，越界时抛出异常
```

`deque` 支持像 `vector` 一样使用下标，但内部存储通常不是一整段连续内存，因此不能把它当作连续数组使用，也没有 `data()` 接口。

#### 遍历

`deque` 是完整容器，提供迭代器，可以使用范围 `for`：

```cpp
for (const int value : deq) {
    cout << value << '\n';
}
```

也可以使用迭代器：

```cpp
for (auto it = deq.begin(); it != deq.end(); ++it) {
    cout << *it << '\n';
}
```

#### `queue` 与 `deque` 的区别

| 对比项 | `queue` | `deque` |
|---|---|---|
| 类型 | 容器适配器 | 完整容器 |
| 加入元素 | 只开放队尾 `push` | `push_front`、`push_back` |
| 删除元素 | 只开放队首 `pop` | `pop_front`、`pop_back` |
| 下标访问 | 不支持 | 支持 |
| 迭代器 / 范围 `for` | 不支持 | 支持 |
| `clear()` | 不提供 | 提供 |

`queue` 默认通常使用 `deque` 作为底层容器，但它故意只开放先进先出的操作。如果题目需要同时操作两端，应直接使用 `deque`。

#### 单调队列模板

滑动窗口最大值等问题常用 `deque` 保存数组下标，并让下标对应的值从队首到队尾单调递减：

```cpp
deque<int> deq;

for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
    // 删除已经离开窗口的下标
    while (!deq.empty() && deq.front() <= i - k) {
        deq.pop_front();
    }

    // 删除队尾不可能再成为最大值的元素
    while (!deq.empty() && nums[deq.back()] <= nums[i]) {
        deq.pop_back();
    }

    deq.push_back(i);

    if (i >= k - 1) {
        result.push_back(nums[deq.front()]);
    }
}
```

这里保存下标而不是数值，是因为既要读取元素大小，也要判断元素是否已经离开窗口。队首始终是当前窗口最大值的下标。

#### 复杂度与常见错误

- 队首、队尾插入或删除：`O(1)`。
- 随机下标访问：`O(1)`。
- 中间位置插入或删除：通常为 `O(n)`。
- 空 `deque` 调用 `front()`、`back()` 或删除操作：未定义行为。
- `pop_front()` 和 `pop_back()` 返回 `void`，不能用它们接收被删除值。
- `deque` 不保证连续存储，也不像 `vector` 那样提供 `reserve()`。

### 1.7 `priority_queue`：优先队列 / 堆

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

### 1.8 STL 容器的共同规则

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

### 1.9 `push`、`push_back` 与 `insert` 怎么选择

这些名字都表示“加入元素”，但它们属于不同类型的容器接口，并不是任意容器都能使用。

#### 最重要的判断规则

```text
顺序容器，明确从哪一端加入：push_back / push_front
栈、队列、堆，只暴露一个加入入口：push
集合、映射，按照 key 组织元素：insert
```

#### 容器速查

| 容器 | 常用加入操作 | 含义 |
|---|---|---|
| `vector<T>` | `push_back(value)` | 在动态数组末尾加入一个 `T` |
| `string` | `push_back(ch)` | 在字符串末尾加入一个字符 |
| `deque<T>` | `push_front(value)` / `push_back(value)` | 从队首或队尾加入 |
| `list<T>` | `push_front(value)` / `push_back(value)` | 从链表头部或尾部加入 |
| `stack<T>` | `push(value)` | 压入栈顶 |
| `queue<T>` | `push(value)` | 加入队尾 |
| `priority_queue<T>` | `push(value)` | 加入堆中并维持堆序 |
| `set<T>` / `unordered_set<T>` | `insert(value)` | 按 key 插入，重复元素不会再次加入 |
| `map<K,V>` / `unordered_map<K,V>` | `insert({key, value})` | 插入一组 key-value，key 不重复 |

#### 为什么 `vector` 使用 `push_back`

`vector` 是有顺序的动态数组，必须明确元素加入的位置。最常见的是追加到末尾：

```cpp
vector<int> nums;
nums.push_back(10);
nums.push_back(20);
```

`push_back` 一次加入一个“元素类型”的对象：

```cpp
vector<int> nums;
nums.push_back(1);               // 元素类型是 int

vector<vector<int>> answers;
answers.push_back({-1, 0, 1});   // 元素类型是 vector<int>
```

`vector` 也有 `insert`，但它主要用于在指定迭代器位置插入：

```cpp
nums.insert(nums.begin(), 5);          // 在开头插入一个 5
nums.insert(nums.end(), {1, 2, 3});    // 在末尾插入多个元素
```

只是在末尾追加一个元素时，优先使用语义更直接的 `push_back`。

#### 为什么 `stack`、`queue` 使用 `push`

这些是容器适配器，加入位置由数据结构本身决定，不需要调用者选择方向：

```cpp
stack<int> stk;
stk.push(10);       // 只能压入栈顶

queue<int> que;
que.push(10);       // 只能加入队尾

priority_queue<int> heap;
heap.push(10);      // 加入堆，内部自动调整位置
```

虽然 `queue` 实际从队尾加入，但它的接口仍然叫 `push`，因为使用者只关心“入队”，不需要直接操作底层容器的尾部。

#### 为什么集合与映射使用 `insert`

集合与映射不是按照“头部、尾部”组织元素，而是按照 key 组织元素，所以使用 `insert`：

```cpp
unordered_set<char> window;
window.insert('a');

unordered_map<string, int> score;
score.insert({"Alice", 100});
```

它们没有 `push`、`push_back`，也不能由调用者指定元素最终位于哪个位置。

#### `emplace` 系列

许多容器还有对应的 `emplace` 操作，作用与加入元素相近，但会直接使用参数构造元素：

```cpp
vector<int> nums;
queue<pair<int, int>> positions;
unordered_set<char> window;

nums.emplace_back(10);       // vector：末尾构造
positions.emplace(1, 2);     // queue：直接构造 pair<int, int>
window.emplace('a');         // unordered_set：直接构造并插入
```

刷题初期可以先记住 `push` / `push_back` / `insert`；需要构造 `pair` 或复杂对象时，再考虑 `emplace`。

#### 常见错误

```cpp
unordered_set<int> uset;
// uset.push(1);             // 错误：集合使用 insert

vector<int> nums;
// nums.push(1);             // 错误：vector 末尾加入使用 push_back

queue<int> que;
// que.push_back(1);         // 错误：queue 使用 push
```

记忆口诀：

```text
数组末尾 push_back；
双端容器带方向；
栈队堆只说 push；
哈希红黑树用 insert。
```

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

#### 基本写法

```cpp
sort(nums.begin(), nums.end());
```

`sort(first, last)` 会原地排序半开区间 `[first, last)`，平均时间复杂度为 `O(n log n)`。
不传第三个参数时，默认使用元素类型的 `<` 运算符，因此通常表现为升序。

自定义降序：

```cpp
sort(nums.begin(), nums.end(), greater<int>());
```

也可以写成：

```cpp
sort(nums.rbegin(), nums.rend());
```

题目明确禁止 `sort` 时不能使用。

#### `vector<vector<int>>` 默认怎样排序

`vector` 之间默认采用**字典序**比较，规则类似单词在字典中的排列：

1. 先比较下标 `0` 的元素；
2. 如果相等，继续比较下标 `1`；
3. 依次向后，直到找到不同元素；
4. 如果公共部分全部相同，较短的 `vector` 排在前面。

例如：

```cpp
vector<vector<int>> values = {
    {4, 7}, {1, 4}, {1, 3, 5}, {1, 3}
};

sort(values.begin(), values.end());

// 排序结果：
// {1, 3}, {1, 3, 5}, {1, 4}, {4, 7}
```

在合并区间题中，每个元素都是 `{start, end}`，长度固定为 `2`：

```cpp
sort(intervals.begin(), intervals.end());
```

它等价于：

- 先按照 `start` 升序；
- `start` 相同时，再按照 `end` 升序。

所以第 56 题可以直接使用默认排序。

#### 使用 lambda 自定义排序规则

`sort` 的第三个参数可以传入比较器：

```cpp
sort(first, last, comp);
```

比较器 `comp(a, b)` 的含义是：

> 如果 `a` 应当排在 `b` 前面，就返回 `true`。

lambda 是一种没有函数名、可以直接写在使用位置的函数。它的完整语法是：

```cpp
[捕获列表](参数列表) -> 返回类型 {
    函数体
}
```

其中 `-> 返回类型` 通常可以省略，由编译器根据 `return` 自动推导：

```cpp
[](const vector<int>& a, const vector<int>& b) {
    return a[0] < b[0];
}
```

逐部分理解：

```cpp
[]                         // 捕获列表：不使用外部局部变量
(const vector<int>& a,     // 第一个待比较的元素
 const vector<int>& b)     // 第二个待比较的元素
{
    return a[0] < b[0];    // 返回 bool：a 是否应该排在 b 前面
}
```

- `[]`：捕获列表。空的 `[]` 表示函数体不使用所在函数中的局部变量；
- `(const vector<int>& a, const vector<int>& b)`：参数列表。`sort` 每次取两个元素交给它比较；
- `const`：承诺不修改正在比较的元素；
- `&`：使用引用，避免复制整个 `vector<int>`；
- `{ ... }`：函数体；
- 两个分支都返回 `bool`，因此可以省略显式的 `-> bool`。

把返回类型写全也可以：

```cpp
[](const vector<int>& a, const vector<int>& b) -> bool {
    return a[0] < b[0];
}
```

它和下面这个普通函数表达的是相同的比较规则：

```cpp
bool compare(const vector<int>& a, const vector<int>& b) {
    return a[0] < b[0];
}
```

区别只是 lambda 不需要单独起名字，可以直接作为 `sort` 的第三个参数传入。`sort` 会在排序过程中多次调用它，例如比较 `intervals[0]` 与 `intervals[1]`。

捕获列表中也可以使用外部变量。例如，根据 `ascending` 决定升序还是降序：

```cpp
bool ascending = true;

sort(nums.begin(), nums.end(),
     [ascending](int a, int b) {
         if (ascending) {
             return a < b;
         }
         return a > b;
     });
```

这里 `[ascending]` 表示把外部变量 `ascending` 按值复制进 lambda。初学 `sort` 时，先熟悉空捕获 `[]` 即可。

例如，区间按起点升序，起点相同时按终点降序：

```cpp
sort(intervals.begin(), intervals.end(),
     [](const vector<int>& a, const vector<int>& b) {
         if (a[0] != b[0]) {
             return a[0] < b[0];   // 起点小的在前
         }
         return a[1] > b[1];       // 起点相同，终点大的在前
     });
```

记忆方法：不要死记“升序写 `<`、降序写 `>`”，而是把代码读成：

```cpp
return a.xxx < b.xxx;
// a.xxx 更小时，a 排在 b 前面
```

#### 自定义对象按成员变量排序

```cpp
struct Student {
    string name;
    int score;
    int age;
};

vector<Student> students;
```

按成绩降序；成绩相同时，按年龄升序：

```cpp
sort(students.begin(), students.end(),
     [](const Student& a, const Student& b) {
         if (a.score != b.score) {
             return a.score > b.score;  // 成绩高的在前
         }
         return a.age < b.age;          // 年龄小的在前
     });
```

参数写成 `const Student&`，可以避免复制对象，同时保证比较函数不修改对象。

#### 使用普通比较函数

除了 lambda，也可以单独定义函数：

```cpp
bool compareStudent(const Student& a, const Student& b) {
    if (a.score != b.score) {
        return a.score > b.score;
    }
    return a.age < b.age;
}

sort(students.begin(), students.end(), compareStudent);
```

如果比较函数写在 `Solution` 类中，可以把它声明为 `static`：

```cpp
class Solution {
public:
    static bool compareInterval(const vector<int>& a,
                                const vector<int>& b) {
        return a[0] < b[0];
    }

    vector<vector<int>> merge(vector<vector<int>>& intervals) {
        sort(intervals.begin(), intervals.end(), compareInterval);
        // ...
    }
};
```

LeetCode 中临时使用的排序规则，通常用 lambda 最直观。

#### 比较器最重要的限制

相等时必须返回 `false`，因此通常使用 `<` 或 `>`，不要使用 `<=` 或 `>=`：

```cpp
// 正确
return a.score < b.score;

// 错误：a 和 b 相等时，compare(a, b) 与 compare(b, a) 都会返回 true
return a.score <= b.score;
```

`sort` 要求比较器满足严格弱序。初学时可以记成：

> 相等的两个对象，谁也不能被判定为“必须排在另一个前面”。

另外，`sort` 需要随机访问迭代器，可用于 `vector`、`array`、`deque`，不能直接用于 `list`；链表容器应使用 `list.sort()`。

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
#include <deque>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>
```

### 3.10 前置 `++`、后置 `++` 与同一表达式多次访问

单独理解时：

```cpp
int a = left++; // 先把 left 的旧值赋给 a，再让 left 加 1
int b = ++left; // 先让 left 加 1，再把新值赋给 b
```

但是，下面两种写法都不能使用：

```cpp
nums[left] == nums[left++];
nums[left] == nums[++left];
```

原因是 `left` 一方面被 `nums[left]` 读取，另一方面又在同一个表达式中被递增；`==` 的两个操作数之间没有规定的求值先后顺序。读取和修改之间没有被正确排序，属于未定义行为。不能依赖某次运行中看起来的结果，也不能判断“前面的 `left` 一定会不会受到 `++` 影响”。

如果想比较当前位置和下一个位置，应明确拆开：

```cpp
if (left + 1 < static_cast<int>(nums.size()) &&
    nums[left] == nums[left + 1]) {
    ++left;
}
```

如果确实需要先保存旧下标，再递增，也应分成多条语句：

```cpp
int oldLeft = left;
++left;
bool same = nums[oldLeft] == nums[left];
```

原则：同一条表达式中不要同时读取某个变量并修改它；把递增、下标访问和比较拆开，代码更安全，也更容易确认执行顺序。

## 4. 快速查询表

### STL

| 需求 | 写法 | 注意 |
|---|---|---|
| `vector` 元素数量 | `nums.size()` | 返回 `size_t` |
| `string` 截取子串 | `s.substr(start, length)` | 第二个参数是长度 |
| 截取闭区间 `[left,right]` | `s.substr(left, right-left+1)` | 包含 `right`，所以长度加一 |
| 截取半开区间 `[left,right)` | `s.substr(left, right-left)` | 不包含 `right` |
| 截取到字符串末尾 | `s.substr(start)` | `start == s.size()` 时返回空串 |
| 哈希表 key 数量 | `need.size()` | 不是所有 value 的总和 |
| 判断哈希 key | `need.count(c) > 0` | 不会插入 key |
| 查找哈希 key | `need.find(c) != need.end()` | 可通过迭代器读取 value |
| 哈希计数 | `need[c]++` | 不存在时自动插入并初始化为 0 |
| `vector` 末尾加入 | `nums.push_back(value)` | 一次加入一个 `value_type` |
| `deque` 两端加入 | `deq.push_front(value)` / `deq.push_back(value)` | 必须明确方向 |
| 集合插入 | `uset.insert(value)` | 自动去重，不使用 `push` |
| 映射插入 | `umap.insert({key, value})` | key 已存在时不会覆盖原 value |
| 队尾加入元素 | `que.push(value)` | 返回 `void` |
| 队列读取队首 | `que.front()` | 不删除元素 |
| 队列读取队尾 | `que.back()` | 不删除元素 |
| 队列删除队首 | `que.pop()` | 返回 `void` |
| 队列是否为空 | `que.empty()` | 调用 `front` / `back` / `pop` 前检查 |
| 队列元素数量 | `que.size()` | 返回 `size_t` |
| 双端队列队首加入 | `deq.push_front(value)` | 平均 `O(1)` |
| 双端队列队尾加入 | `deq.push_back(value)` | 平均 `O(1)` |
| 双端队列删除队首 | `deq.pop_front()` | 返回 `void` |
| 双端队列删除队尾 | `deq.pop_back()` | 返回 `void` |
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
| 降序排序 | `sort(first, last, greater<T>())` | `T` 是元素类型 |
| 嵌套 `vector` 默认排序 | `sort(values.begin(), values.end())` | 按元素逐项进行字典序比较 |
| 自定义排序 | `sort(first, last, comp)` | `comp(a, b)` 为真表示 `a` 在 `b` 前；相等时必须为假 |
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
