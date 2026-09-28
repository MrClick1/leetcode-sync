# 912. 排序数组：快排、归并与堆排序

状态：2026-09-28 用户确认最终版本 LeetCode 提交通过，代码审阅正确。本轮未重新运行本地测试；大量相等元素仍可使此二路划分退化。本题作为第 347 题快速选择的前置练习。

后续进度：递归及迭代调整版堆排序均已由用户确认提交通过；归并排序已讨论合并后写回和返回数组的问题，尚未收到修正后提交通过的反馈。下方保留快排原记录，并补充其他方法。

## 学习状态：暂缓，三种排序均待复习

2026-09-28 用户表示快排、归并、堆排序仍然半生不熟，先记录，不继续展开；下一题切换至 215. 数组中的第 K 个最大元素。提交通过不等于已熟练掌握，后续按用户意愿回顾，不自动安排提醒。

- 快排：复习分区不变量、i/j 从当前子区间开始、基准归位后递归排除基准；注意大量重复值时的退化。
- 归并：复习先递归再合并、两个有序区间的指针移动、temp 写回 nums 的下标映射；最终修正版通过状态待确认。
- 堆排序：复习 len 统一表示数量、自底向上建堆、堆顶与末尾交换后缩小堆，以及递归/迭代下沉的终止条件。
- 建议复习方式：先说清函数前提与返回后保证，再手动模拟小数组，最后独立写代码检查边界。

### 本次重点疑问：为什么 largest == i 就可以停止？

maxHeapify 只修复当前节点，前提是左右子树已分别成堆，并不能把任意子树直接变成堆。例如数组 [10,5,8,20]，仅调整根会因 10 大于两个孩子而停止，但 5 的孩子 20 仍违反堆性质。

buildMaxHeap 从最后一个非叶子节点往前处理：孩子下标大于父节点，所以孩子子树已经处理好，叶子本身天然成堆。这保证处理根之前，隐藏在下层的大值已经被提升。

下沉交换后，只有被换下去的元素可能违反堆性质，其孩子子树未被破坏，因此只沿 largest 对应的路径继续。排序阶段交换根与末尾并缩小堆后，剩余左右子树仍然成堆，也满足调用前提。

- 递归版：largest == i 时不再调用自己（也可显式 return）；否则递归进入较大孩子。
- 迭代版：没有左孩子时 while 自然结束；largest == i 时 break；否则交换并令 i=largest。
- 用户迭代版已确认通过：while (2*i+1<len) 与所有 child<len 一致；左孩子检查可省略，排序循环 i>=0 的最后一轮可省略，改为 i>0。

## 最终代码

保留用户的单向扫描、while 循环和随机基准写法，仅删除 quicksort 内未使用的 n，并明确下标注释。

```cpp
class Solution {
public:
    int partition(vector<int>& nums, int left, int right) {
        int pivotIdx = rand() % (right - left + 1) + left;
        int pivot = nums[pivotIdx];
        swap(nums[right], nums[pivotIdx]);

        int i = left - 1; // 已收集的“小于 pivot 区域”的末尾下标
        int j = left;    // 当前子区间中下一个待检查的下标
        while (j < right) {
            if (nums[j] < pivot) {
                ++i;
                swap(nums[i], nums[j]);
            }
            ++j;
        }

        ++i;
        swap(nums[i], nums[right]);
        return i;
    }

    void quicksort(vector<int>& nums, int left, int right) {
        if (left >= right) return;
        int pos = partition(nums, left, right);
        quicksort(nums, left, pos - 1);
        quicksort(nums, pos + 1, right);
    }

    vector<int> sortArray(vector<int>& nums) {
        int n = (int)nums.size();
        quicksort(nums, 0, n - 1);
        return nums;
    }
};
```

## partition 的职责与不变量

只处理闭区间 [left,right]；把基准放到最终位置 pos，左侧元素都小于基准，右侧元素都大于等于基准。返回的 pos 是基准位置，不是另一种分区算法中的分割线。

基准先存入局部变量，并将对应数组元素放到 right。扫描过程中：

| 范围 | 含义 |
|---|---|
| [left,i] | 已收集的小于 pivot 的元素 |
| [i+1,j-1] | 已检查的大于等于 pivot 的元素 |
| [j,right-1] | 尚未检查的元素 |
| right | 暂存的基准元素 |

开始时小元素区域为空，所以 i=left-1；第一个待检查位置为 j=left。left-1 只是边界标记，先 ++i 再访问 nums[i]，不会访问负下标。

若 nums[j] < pivot，先扩大小元素区域，再交换到区域末尾。扫描结束后，i+1 就是基准应放的位置。基准归位后，左右区间还不一定内部有序，递归负责继续处理。

## [4,2,5,1,3] 的首轮模拟

假设选中的基准为末尾 3：

| 步骤 | 数组 | i |
|---|---|---|
| 初始 | [4,2,5,1,3] | -1 |
| 检查 4，不小于 3 | [4,2,5,1,3] | -1 |
| 检查 2，扩大区域并交换 | [2,4,5,1,3] | 0 |
| 检查 5，不小于 3 | [2,4,5,1,3] | 0 |
| 检查 1，扩大区域并交换 | [2,1,5,4,3] | 1 |
| 基准与下标 i+1=2 交换 | [2,1,3,4,5] | 返回 2 |

下一步仅递归 [0,1] 与 [3,4]，不再包含已经就位的下标 2。

## 本轮错误与原因

1. `rand() % (right-left)` 在左右相等时取模 0；闭区间长度应为 right-left+1，并在调用 partition 前判断 left>=right。
2. 递归范围重新使用 0、n-1：破坏“只负责当前子区间”的约定，可能扩大区间、循环递归并触发 stack-overflow。错误栈显示 rand，并不意味着 rand 是根因，需查看连续重复的 quicksort 调用。
3. 左递归包含 pos：当 pos==right 时子区间不缩小。当前返回基准位置的算法必须使用 [left,pos-1]、[pos+1,right]。
4. 换成单向扫描后仍固定 i=-1、j=0：只有 left==0 时才符合含义。递归到右侧区间后会重新扫描前缀，甚至返回小于 left 的 pos。必须改为 i=left-1、j=left。
5. 例如 [1,1,1]，错误初始化会让 partition(1,2) 返回 0，右递归再次调用 quicksort(1,2)，无限重复。
6. 早期双向交换写法中的 swap(nums[left],pivot) 是与局部变量交换，且当时基准已在相遇处，是多余操作；当前单向扫描末尾的 swap(nums[i],nums[right]) 则负责让末尾基准归位，不能删除。

## 和官解的关系

官解把“随机选择基准并放到末尾”和“单向扫描划分”拆成两个函数。用户版本将它们合在 partition 中，算法相同。

官解使用 for(j=left; j<right; ++j)，用户使用 while，效果相同。官解最后交换 i+1 并返回 i+1，用户先 ++i 再交换并返回 i，也等价。

官解的 srand(time(NULL)) 用于设置伪随机种子，不影响排序逻辑正确性，也不能解决相等元素的退化。rand 取模的随机性与范围受实现影响，不应把它视为严格均匀的通用随机采样器。

## 复杂度与局限

一次长度为 s 的 partition 是 O(s)，额外工作空间 O(1)。分区较均衡时，共 O(log n) 层，每层总工作量 O(n)，所以总时间 O(n log n)，递归栈空间 O(log n)。通常的随机快排期望分析以适当的随机选择与键分布/重复值处理为前提；本版本不能无条件保证对任意重复值输入都期望 O(n log n)。

分区极不均衡时，处理规模依次为 n、n-1、n-2……，总时间 O(n²)，递归栈深度 O(n)。全相等数组会使严格 < 条件始终为假，每次 pos=left；无论随机选哪个基准都是同一个值，仍然退化。

因此提交通过与最坏复杂度保证是两件事。本题提出的 O(n log n) 目标，并不能由这份随机二路快排提供最坏情况保证。后续可学三路划分改善重复值情况；要求最坏 O(n log n) 时可用归并、堆排序或内省排序。

## 复写顺序

1. 明确当前负责 [left,right]，先写 left>=right 的终止条件。
2. 随机基准放末尾，初始化 i=left-1、j=left。
3. 扫描其余元素，小于基准的移入前面的区域。
4. 基准归位，返回位置 pos。
5. 递归两侧且排除 pos。

第 347 题的快速选择只递归需要继续寻找的一侧；快排要递归两侧。用户决定先练熟快排，再继续快速选择，不自动推进下一种算法。

## 归并排序：已讨论，待确认最终实现

mergeSort(nums,temp,l,r) 返回时，应保证 nums[l...r] 有序。先递归 [l,mid] 与 [mid+1,r]，再合并；mid 是切分位置，没有已归位基准，不能照搬快排的 mid-1。

用户将本轮合并结果从 temp[0] 起写入，但忘记复制回 nums，导致上层仍读取未排序的两个区间。cnt=0 的写法本身合法，合并后应执行：

```cpp
for (int p = 0; p < cnt; ++p) {
    nums[l + p] = temp[p];
}
```

入口返回 nums，而不是 temp。单元素输入 [5] 时递归立即返回，temp 仍是 [0]，直接返回 temp 就会错误。共享 temp 是安全的，因为子问题的结果已经及时写回 nums，临时空间可在下一次合并时复用。

归并最坏时间 O(n log n)，辅助数组 O(n)，递归栈 O(log n)，合计额外空间 O(n)。

## 堆排序：通过版本与整理

大顶堆使父节点不小于其孩子，数组根 nums[0] 是最大值。每次交换根与堆末尾，将最大值排到最终位置，再缩小堆，修复根节点。

用户的 len 表示元素数量，范围为 [0,len)。以下保留递归版本，做两处等价整理：只在最大位置改变时交换，将排序循环结束条件改为 i>0，省去最后的自交换和空堆调整。

```cpp
class Solution {
public:
    void maxHeapify(vector<int>& nums, int i, int len) {
        int leftChild = 2 * i + 1;
        int rightChild = 2 * i + 2;
        int largest = i;
        if (leftChild < len && nums[leftChild] > nums[largest]) {
            largest = leftChild;
        }
        if (rightChild < len && nums[rightChild] > nums[largest]) {
            largest = rightChild;
        }
        if (largest != i) {
            swap(nums[i], nums[largest]);
            maxHeapify(nums, largest, len);
        }
    }

    void buildMaxHeap(vector<int>& nums, int len) {
        for (int i = len / 2 - 1; i >= 0; --i) {
            maxHeapify(nums, i, len);
        }
    }

    void heapSort(vector<int>& nums) {
        int len = (int)nums.size();
        buildMaxHeap(nums, len);
        for (int i = len - 1; i > 0; --i) {
            swap(nums[i], nums[0]);
            --len;
            maxHeapify(nums, 0, len);
        }
    }

    vector<int> sortArray(vector<int>& nums) {
        heapSort(nums);
        return nums;
    }
};
```

用户原循环 i>=0 的末轮会交换 nums[0] 与自身，再以 len=0 调用调整。题目保证原数组非空，当前实现会短路跳过孩子访问，最后只自交换实际存在的 nums[0]，没有因此数组越界；但这是多余且含义不清晰的一轮，建议用 i>0。

## 为什么先自底向上建堆，再从根向下调整

maxHeapify 的前提是左右子树已各自成堆，只有当前节点可能过小。建堆时从最后一个非叶子节点向前处理，确保这个前提成立；只调整根不能保证整棵树成堆。

交换时选当前节点和两个孩子中最大的。如果较大孩子上移，被换下去的元素可能仍小于自己的新孩子，因此只需沿它下沉的那条路径继续调整，不必同时递归两侧。

排序阶段交换根与末尾后，剩余左右子树仍然成堆，只有根可能失效，所以只需 maxHeapify(nums,0,len)。

## 与官方迭代版对照：len 的含义不同

| 约定 | 用户版本 | 官方版本 |
|---|---|---|
| len 含义 | 元素数量 | 最后一个有效下标 |
| 初始化 | nums.size() | nums.size()-1 |
| 有效范围 | [0,len) | [0,len] |
| 孩子有效条件 | child < len | child <= len |
| 首轮放置位置 | len-1 | len |
| 继续向下 | 递归 maxHeapify(...,largest,len) | 设置 i=large，进入下一轮循环 |

例如 5 个元素：用户 len=5、最大下标 4；官方 len=4、最大下标同样为 4。数量与末尾下标都可以使用，但初始化、比较条件与排序起点必须配套。

官方 `(i << 1) + 1` 在本题非负且较小的下标范围内等价于 `2*i+1`；`<<` 是左移运算符，不必为了性能把清楚的乘法改成位运算。

```cpp
for (; (i << 1) + 1 <= len;) { /* ... */ }
// 等价控制结构（此处 len 是最后有效下标）：
while (2 * i + 1 <= len) { /* ... */ }
```

条件表示当前节点还有左孩子。如果连左孩子都没有，就已经是叶子，无需继续。这个条件也保证循环中的 lson<=len，官解 if 再检查一次合法但冗余；右孩子仍须单独检查是否存在。

官解先比较左孩子与父节点，再拿右孩子与当前最大候选比较；large 是三者最大值的下标，不一定是孩子。若 large!=i，交换并令 i=large，等价于用户递归进入被换下去的位置；否则 break。

官方建堆从 len/2 开始：换成元素数 n 后是 (n-1)/2。用户从 n/2-1 开始。n 为偶数时两者相同，n 为奇数时官解会多处理一个叶子；例如 n=5，官解从下标 2 开始，用户从下标 1 开始。叶子调整直接结束，仍然正确。

## 沿用户的数量约定改为迭代调整

仅替换 maxHeapify，其他函数保持上面的整理版即可：

```cpp
void maxHeapify(vector<int>& nums, int i, int len) {
    while (2 * i + 1 < len) {
        int leftChild = 2 * i + 1;
        int rightChild = 2 * i + 2;
        int largest = i;
        if (nums[leftChild] > nums[largest]) largest = leftChild;
        if (rightChild < len && nums[rightChild] > nums[largest]) {
            largest = rightChild;
        }
        if (largest == i) break;
        swap(nums[i], nums[largest]);
        i = largest;
    }
}
```

例如 [1,9,8,7,6,5,4] 的左右子树已成堆，只修复根：i=0，选下标1的9，得到 [9,1,8,7,6,5,4]；i=1，选下标3的7，得到 [9,7,8,1,6,5,4]；i=3 已是叶子，结束。递归和迭代走的是同一条 0→1→3 路径。

## 堆排序易错点与复杂度

- 下标从 0 开始，孩子是 2*i+1、2*i+2；2*i、2*i+1 属于从 1 开始的编号方式。
- len 定义为数量时不能初始化成 nums.size()-1，否则末尾元素从建堆起就被排除。
- 建堆要处理所有非叶子节点，不能只处理根。
- 先把根交换到末尾，再缩小堆，最后调整；已排序后缀不得重新纳入堆。
- largest 比 largestChild 更准确，因为最大值也可能就是父节点。
- 建堆 O(n)，排序阶段 O(n log n)，整体最坏 O(n log n)。建堆不能简单按 n 次完整下沉算紧确复杂度，多数节点靠近叶子，下沉距离短。
- 用户递归调整版额外栈空间 O(log n)；官解迭代调整版额外空间 O(1)。两者都不需要另建数组，堆排序通常不稳定。

上述堆排序代码依据审阅与用户提交结果记录，本轮没有额外编译测试。
