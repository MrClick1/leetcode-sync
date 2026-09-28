# 347. 前 K 个高频元素

状态：2026-09-28 用户确认 LeetCode 提交通过，代码审阅正确。当前为哈希计数 + 全量大顶堆版本，进阶复杂度优化待学习。本轮未创建本地脚手架或重新运行测试。

## 用户通过版本

```cpp
class Solution {
public:
    struct MyCompare {
        bool operator()(const pair<int, int>& p1, const pair<int, int>& p2) {
            return p1.second < p2.second;
        }
    };

    vector<int> topKFrequent(vector<int>& nums, int k) {
        int n = (int)nums.size();
        unordered_map<int, int> umap;
        for (int i = 0; i < n; ++i) {
            ++umap[nums[i]];
        }

        priority_queue<pair<int, int>, vector<pair<int, int>>, MyCompare> pque;
        for (const auto& p : umap) {
            pque.push(p);
        }

        vector<int> res;
        for (int i = 0; i < k; ++i) {
            const auto& topP = pque.top();
            res.push_back(topP.first);
            pque.pop();
        }
        return res;
    }
};
```

## 每种数据的含义

- umap：数字 → 出现次数；不存在的 key 通过 operator[] 初始化为 0，再递增。
- 堆中 pair：first 是数字，second 是频次。
- 比较器返回 p1.second < p2.second，表示频次低的 p1 应让位，因此 top 是频次最高的元素。
- 每个不同数字只入堆一次，连续读取并弹出 k 个堆顶即可得到答案。题目保证 k 不超过不同数字个数，取答案循环不会提前遇到空堆。
- 相同频次的元素在堆中的先后不保证，但题目允许任意返回顺序，且保证前 k 个高频元素集合唯一，所以无需增加平局比较规则。

## 本轮错误

### 把函数名当作比较器类型

原来的 MyCompare 是 Solution 的普通成员函数，而 priority_queue 第三个模板参数需要类型，因此报错：template argument for template type parameter must be a type。

改成 struct MyCompare，并在其中定义 operator() 后，它就是可实例化的比较器类型。仅给原成员函数加 static，仍不能直接把函数名填到这个类型参数位置。

也可以使用 lambda，并写成 priority_queue<T, vector<T>, decltype(cmp)> pq(cmp)。类型与对象的详细对照见 [C++ 语法文档](../01-cpp语法知识文档.md) 的 priority_queue 小节。

补充：官方通过 static cmp 与 decltype(&cmp) 使用普通函数指针作为比较器。static 消除对 Solution 对象的依赖，decltype(&cmp) 提供类型，构造时 q(cmp) 提供函数指针；它与当前 struct + operator() 是两种合法写法。详见 [C++ 语法文档](../01-cpp语法知识文档.md) 的“3.14 static 关键字”和 STL 1.7 对应示例。

### 忘记 pop

top() 只读取不删除。循环里不 pop，会反复读到同一个最高频数字，例如示例 1 得到 [1,1]。

当前代码先将 topP.first 复制进结果，再 pop，是正确顺序。不要在 pop 后继续使用旧的 topP 引用；原栈顶位置可能被其他堆元素替代或已失效。

## 可选的写法整理

比较器不修改自身状态，可以在参数列表后加 const（当前不加也已通过）：

```cpp
bool operator()(const pair<int, int>& p1, const pair<int, int>& p2) const {
    return p1.second < p2.second;
}
```

取答案也可以简写为：

```cpp
res.push_back(pque.top().first);
pque.pop();
```

res.reserve(k) 可提前预留结果空间；它不改变 size，也不是正确性要求。

## 复杂度与进阶状态

设 n 是数组长度，m 是不同数字的个数。按哈希操作平均 O(1) 分析：

- 计数：平均 O(n)。
- m 个元素逐个 push 入堆：O(m log(m+1))。
- 取出 k 个元素：O(k log(m+1))。
- 合计：平均哈希假设下 O(n + (m+k) log(m+1))，因为 k <= m，也可写为 O(n + m log(m+1))。
- 辅助空间 O(m)，结果空间 O(k)。

当 m 与 n 同阶时，当前堆操作可达到 O(n log n)，因此判题通过不代表满足“优于 O(n log n)”的进阶要求。可后续学习频次桶（结合哈希计数，期望 O(n)）或选择算法。维护大小 k 的小顶堆适合 k 较小时优化，但 k 与 n 同阶时，其通用上界仍可能是 O(n log n)，不要无条件说它严格优于 O(n log n)。

## 示例复核

- [1,1,1,2,2,3]，k=2：频次分别为 3、2、1，取出 1、2。
- [1]，k=1：唯一元素入堆并弹出，结果 [1]。
- 负数或 0：比较依据为频次，不影响算法。
- k=m：所有不同数字各返回一次；答案顺序不限。
