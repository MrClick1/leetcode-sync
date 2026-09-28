# 215. 数组中的第 K 个最大元素

状态：2026-09-28 用户确认手工大顶堆 + 迭代下沉版本 LeetCode 提交通过，代码审阅正确。本轮未另建本地脚手架或重新编译测试。快速选择已讨论多种划分、重复值与边界，但尚未收到最终修正版通过反馈，仍需复习。

## 题意与目标

重复元素要分别计数，不是第 k 个不同的元素。例如 [3,2,3,1,2,4,5,5,6] 的第 4 大为 4。升序排列后的目标下标为 n-k。

堆方法判题通过，但最坏 O(n+k log n) 不满足题面线性时间目标。随机快速选择通常按期望 O(n) 分析，最坏仍为 O(n²)。不要把提交通过、复杂度达标和熟练掌握混为一谈。

## 用户通过版本：移除 k-1 个最大值

```cpp
class Solution {
public:
    void maxHeapify(vector<int>& nums, int root, int len) {
        while (2 * root + 1 < len) {
            int leftChild = 2 * root + 1;
            int rightChild = 2 * root + 2;
            int largest = root;

            if (leftChild < len && nums[leftChild] > nums[largest]) {
                largest = leftChild;
            }
            if (rightChild < len && nums[rightChild] > nums[largest]) {
                largest = rightChild;
            }

            if (largest != root) {
                swap(nums[root], nums[largest]);
                root = largest;
            } else {
                break;
            }
        }
    }

    void buildHeap(vector<int>& nums, int len) {
        for (int i = len / 2 - 1; i >= 0; --i) {
            maxHeapify(nums, i, len);
        }
    }

    int findKthLargest(vector<int>& nums, int k) {
        int len = (int)nums.size();
        buildHeap(nums, len);

        for (int i = 0; i < k - 1; ++i) {
            swap(nums[len - 1], nums[0]);
            --len;
            maxHeapify(nums, 0, len);
        }

        return nums[0];
    }
};
```

len 始终是堆内元素数量，有效下标为 [0,len)。数组长度并未改变，只是已经移除的最大值放在数组末尾，不再参与调整。堆顶是当前堆最大值，移除前 k-1 个最大值后，堆顶就是第 k 大。

maxHeapify 的前提：左右子树已经分别成堆，只有当前父节点可能过小。自底向上建堆保证这个前提；移除堆顶后也是只有新根可能违反堆性质。largest==root 时可以结束，原因不只是父节点大于直接孩子，还包括孩子子树原本已成堆。

## 与官方堆解法对照

用户提供的官方代码也以 heapSize 表示元素数量，使用 child<heapSize。这与此前 912 官解以 len 表示末尾下标的约定不同，不能把两道题官解的边界条件混用。

| 比较点 | 用户版本 | 本题官方版本 |
|---|---|---|
| 下沉方式 | 交换后 root=largest，继续 while | 交换后递归 maxHeapify(a,largest,heapSize) |
| 停止条件 | 无左孩子时结束；自己最大时 break | 自己最大时不再递归；叶子没有合法孩子 |
| 堆大小定义 | len 是数量，[0,len) | heapSize 是数量，[0,heapSize) |
| 建堆起点 | len/2-1 | heapSize/2-1 |
| 移除次数 | i 从 0 到 k-2，表示次数 | i 从 n-1 到 n-k+1，表示堆末尾下标 |
| 交换位置 | len-1 | i；每轮也等于 heapSize-1 |
| 额外空间 | O(1) | O(log n) 递归栈 |
| 最坏时间 | O(n+k log n) | O(n+k log n) |

例如 n=6、k=3：

| 轮次 | 用户计数 i | 官解下标 i | 当前堆大小 | 交换位置 |
|---|---:|---:|---:|---:|
| 1 | 0 | 5 | 6 | 5 |
| 2 | 1 | 4 | 5 | 4 |

两种写法都移除 2 个最大值，然后返回剩余堆顶。k=1 时不移除，直接返回初始最大值；k=n 时移除 n-1 个，剩余唯一元素是最小值。

用户 maxHeapify 中 leftChild<len 已经由 while (2*root+1<len) 保证，可省略这次重复检查；右孩子可能不存在，rightChild<len 必须保留。这只是可选整理，不影响正确性或复杂度。

建堆 O(n)，不是紧确 O(n log n)：大部分节点靠近叶子，下沉距离很短。随后 k-1 次移除各最多 O(log n)，合计 O(n+k log n)。迭代调整不增加递归栈，所以辅助空间为 O(1)。

## 堆版本错误复盘：不能同时减 len 和 i

原来写 swap(nums[len-i-1],nums[0])，但每轮 len 已递减，又减 i 导致交换位置往前跳。i 只计数，末尾位置只用当前 len-1。

例如 [3,2,1]、k=3：首轮后为 [2,1 | 3]，len=2。第二轮正确末尾是 1，错误公式 2-1-1=0 却让堆顶自交换，最终返回 2 而不是 1。

## 快速选择：已学习，最终通过待确认

最初的单向划分返回基准归位下标 pos，快速选择只递归目标下标所在一侧，且必须 return 子调用结果。单元素终止应返回 nums[left]，不能返回下标 left。

大量相等值时，单向划分用 <= 会把相等值全部放一边。用户超时附件有 100000 个数字，其中 99991 个为 1；附件没有 k。随机基准不能解决全相等值退化，每轮只缩小一个元素时扫描总量为 O(n²)。把 <= 单独改为 < 只是将重复值集中到另一边。

学习中涉及的划分语义应区分：

| 方式 | 返回/确定内容 | 查找下一侧的范围 |
|---|---|---|
| 基准归位划分 | 基准最终下标 pos | [left,pos-1] 或 [pos+1,right]；pos==target 可立即返回 |
| 官方双向扫描划分 | 分界线 j，基准不一定位于 j | [left,j] 或 [j+1,right]；不能因 j==target 直接返回 |
| 三路划分 | 小于、等于、大于基准三个区间 | 目标落在相等段即返回，否则只处理外侧一段 |

### 保留“返回基准下标”的双向扫描

以下为讨论后的正确参考，未标记为用户提交通过。基准先存于 right，扫描仅处理 [left,right-1]，相等值也停下；交换后双方继续移动，避免重复值集中在单侧。

```cpp
int partition(vector<int>& nums, int left, int right) {
    int pivotIdx = left + rand() % (right - left + 1);
    swap(nums[pivotIdx], nums[right]);
    int pivot = nums[right];
    int l = left, r = right - 1;

    while (l <= r) {
        while (l <= r && nums[l] < pivot) ++l;
        while (l <= r && nums[r] > pivot) --r;
        if (l <= r) {
            swap(nums[l], nums[r]);
            ++l;
            --r;
        }
    }

    swap(nums[l], nums[right]);
    return l;
}
```

扫描后 [left,l-1] 均 <=pivot，[l,right-1] 均 >=pivot，所以可把末尾基准交换到 l。换出去的 nums[l] 可以留在右侧；不能直接放到 r，因为 nums[r] 可能更小，会被换到右侧，甚至 r 可退到 left-1。

退出时 l>r，但不一定紧邻：扫描的守卫使交错差值最多为 1；若两指针在同一相等元素处自交换后各移一步，差值为 2。此时中间那个元素已处理且等于基准，没有未知位置被跳过。

### 边界错误：内层不能写 l<r

[l,r] 是待处理闭区间，l==r 时仍有一个元素。用户将内层守卫写为 l<r，就跳过最后元素的分类，随后仍自交换并前进，导致不正确划分。

反例 [2,1]，基准 1：l=r=0。内层 l<r 跳过判断后返回 pos=1，数组仍是 [2,1]，第 1 大错误返回 1。改成 l<=r 后，右扫描会把 r 减为 -1，最后基准归位到 0，得到 [1,2]。

另一个错误是 swap(nums[left],pivot)：pivot 是局部值副本，交换它会将一个数组元素搬出数组，破坏元素集合。需要交换数组中的两个位置。此前“基准随两次交换来回移动”的写法，在相遇时基准已经位于 l，不能与现在“基准留在 right，最后一次归位”的写法混用。

## 复习顺序

1. 先口述堆解法：建大顶堆、移除 k-1 次最大值、返回堆顶。
2. 区分计数变量与堆大小，独立写出 k=1、k=n 的边界。
3. 快速选择先明确 partition 返回基准下标还是分界线，再写递归范围。
4. 用 [2,1] 和全相等数组模拟最后一个元素与指针交错，确认内层守卫。
5. 912 的三种排序仍按用户意愿留待复习，不自动推进下一题。
