# 560. 和为 K 的子数组

状态：✅ 已解决（2026-09-07，前缀和 + 哈希表解法通过 LeetCode）

## 题目要求

给定整数数组 `nums` 和整数 `k`，统计和为 `k` 的连续非空子数组数量。

## 核心公式：前缀和

设：

```text
prefix[i] = nums[0] + nums[1] + ... + nums[i]
```

下标 `j` 到 `i` 的子数组和为：

```text
prefix[i] - prefix[j - 1]
```

要让它等于 `k`，就需要之前出现过的前缀和：

```text
prefix[j - 1] = prefix[i] - k
```

因此遍历当前前缀和 `prefix` 时，查询 `prefix - k` 已经出现了多少次，就能知道以当前位置结尾的合法子数组数量。

## 用户通过版本

用户先构造完整前缀和数组，再用哈希表统计前缀和出现次数：

```cpp
class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        int res = 0;
        int n = static_cast<int>(nums.size());

        vector<int> presum(n, 0);
        presum[0] = nums[0];

        for (int i = 1; i < n; i++) {
            presum[i] = presum[i - 1] + nums[i];
        }

        unordered_map<int, int> umap;
        umap[0] = 1;

        for (int i = 0; i < n; i++) {
            int d = presum[i] - k;
            auto it = umap.find(d);
            if (it != umap.end()) {
                res += it->second;
            }
            umap[presum[i]]++;
        }

        return res;
    }
};
```

这版已经通过 LeetCode。`umap[0] = 1` 表示数组开始前存在一个和为 `0` 的空前缀，可以统计从下标 `0` 开始的子数组。

## 为什么必须累加出现次数

不能只写：

```cpp
res++;
```

因为同一个前缀和可能出现多次，每一次都对应一个不同的起点。

例如：

```text
nums = [1, -1, 0], k = 0
前缀和 = [1, 0, 0]
```

最后一个前缀和为 `0` 时，之前的 `0` 出现了两次，分别对应：

- `[0]`
- `[1, -1, 0]`

所以当前应该增加 `2`，即：

```cpp
res += it->second;
```

而不是只增加 `1`。最终三个合法子数组是 `[1,-1]`、`[0]` 和 `[1,-1,0]`。

## 查询与插入的顺序

处理当前前缀和时，必须先查询，再记录当前前缀和：

```cpp
res += freq[prefix - k];
freq[prefix]++;
```

这样 `freq` 只表示当前元素之前的前缀和，不会把空子数组计算进去。特别是 `k == 0` 时，如果先插入当前前缀和，当前前缀会和自己匹配，造成错误计数。

## 更简洁的一次遍历写法

不必额外保存 `presum` 数组，可以边遍历边累计：

```cpp
class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        long long prefix = 0;
        int result = 0;
        unordered_map<long long, int> freq;
        freq[0] = 1;

        for (int num : nums) {
            prefix += num;

            auto it = freq.find(prefix - k);
            if (it != freq.end()) {
                result += it->second;
            }

            freq[prefix]++;
        }

        return result;
    }
};
```

## 复杂度

用户的两遍前缀和实现和一次遍历版本的时间复杂度都是平均 `O(n)`，哈希表空间复杂度为 `O(n)`。一次遍历版本额外省去了 `presum` 数组。

## C++ 注意点

- `unordered_map` 需要同时指定 key 和 value 类型：`unordered_map<int, int>`。
- `res += it->second` 使用的是某个前缀和的出现次数，而不是只加一次。
- `int` 在本题约束下通常足够，但使用 `long long` 保存前缀和更稳妥。
- 用户通过版本中的 `presum[0]` 依赖题目保证数组非空；若写通用函数，需要先处理空数组。

## 复盘

本题从回溯思路转为前缀和哈希表。回溯会枚举子序列且复杂度过高；前缀和公式把“连续子数组和为 `k`”转化为“之前前缀和为 `prefix-k`”。本轮修正了哈希表模板参数缺失和只增加 `1` 的计数错误，最终提交通过。
