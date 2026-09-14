# 146. LRU 缓存（二刷）

状态：✅ 已解决

> 一刷笔记：[2026-09-01/0146-lru-cache.md](../2026-09-01/0146-lru-cache.md)

## 核心设计

LRU 要同时解决两个问题：

1. 根据 key 在平均 `O(1)` 时间内找到缓存项。
2. 在 `O(1)` 时间内更新“最近使用 / 最久未使用”的顺序。

因此使用：

```text
unordered_map<int, DLinkedNode*> + 双向链表
```

- 哈希表负责定位：`key -> 节点地址`。
- 双向链表负责顺序：`head` 一侧最新，`tail` 一侧最旧。
- `head`、`tail` 使用虚拟节点，避免为空、首节点、尾节点等边界分支。

```text
head <-> 最近使用 <-> ... <-> 最久未使用 <-> tail
```

所以：

```cpp
head->next   // MRU

tail->pre    // LRU
```

## 为什么必须是双向链表

哈希表可以直接拿到某个节点指针，但如果要在 `O(1)` 时间内把这个节点从原位置摘掉，必须同时知道它的前驱和后继：

```cpp
node->pre->next = node->next;
node->next->pre = node->pre;
```

单链表只有 `next`，拿到当前节点后无法直接知道前驱，通常还要从头查找，无法满足 `O(1)`。

## 三个基础操作

### 1. 删除节点

```cpp
void removeNode(DLinkedNode* node) {
    node->pre->next = node->next;
    node->next->pre = node->pre;
}
```

### 2. 插入到头部

```cpp
void insertIntoHead(DLinkedNode* node) {
    node->next = head->next;
    node->pre = head;
    head->next = node;
    node->next->pre = node;
}
```

### 3. 移动到头部

不是新的底层操作，本质是：

```text
moveToHead = removeNode + insertIntoHead
```

```cpp
void moveToHead(DLinkedNode* node) {
    removeNode(node);
    insertIntoHead(node);
}
```

## get

```text
key 不存在 -> -1
key 存在   -> 哈希表定位 -> moveToHead -> 返回 value
```

`get` 也属于一次“使用”，所以命中后必须刷新节点的最近使用位置。

## put

### key 已存在

```text
更新 value -> moveToHead
```

节点地址没有变化，不需要重新创建节点。

### key 不存在

```text
new 节点
-> 插入链表头
-> 加入哈希表
-> size++
```

如果：

```cpp
size > capacity
```

则 `tail->pre` 就是最久未使用节点：

```text
从链表摘除
-> 从 unordered_map erase
-> delete 节点
-> size--
```

节点必须保存 `key`，因为淘汰时首先通过链表得到的是 `Node*`，需要 `node->key` 才能同步删除哈希表映射。

## 本次二刷实际踩到的坑

### 1. head / tail 只是声明指针，没有创建节点

最初写成：

```cpp
DLinkedNode* head;
DLinkedNode* tail;

head->next = tail;
tail->pre = head;
```

此时 `head`、`tail` 是未初始化指针，解引用会产生未定义行为，并出现类似：

```text
0xbebebebebebebebe
```

的运行时错误。

必须先创建真实节点：

```cpp
head = new DLinkedNode();
tail = new DLinkedNode();
```

### 2. get 中 return 不能写在 moveToHead 前面

错误结构：

```cpp
return node->val;
moveToHead(node);
```

`return` 后面的代码永远不会执行。LRU 中读取也算使用，所以要先：

```cpp
moveToHead(node);
return node->val;
```

### 3. 新节点要同时写入链表和哈希表

只执行：

```cpp
insertIntoHead(node);
```

不够，还必须：

```cpp
umap[key] = node;
```

否则链表里虽然存在节点，`get(key)` 仍无法通过哈希表找到。

### 4. 淘汰节点后释放内存

本次最终版补上：

```cpp
delete tNode;
```

避免每次淘汰都遗留堆内存。完整工程代码还可以增加析构函数，释放缓存中剩余节点以及两个虚拟节点。

## 构造函数：初始化列表 vs 函数体赋值

本次还复习了：

```cpp
LRUCache(int capacity) {
    size = 0;
    this->capacity = capacity;
}
```

属于成员已经进入初始化阶段以后，在构造函数体中执行赋值。

更推荐的 C++ 写法是成员初始化列表：

```cpp
LRUCache(int capacity)
    : size(0), capacity(capacity) {
}
```

可以记成：

> 初始化列表是在成员“出生时”直接给初值；构造函数体中的 `=` 是成员已经存在后再赋值。

对于 `int` 区别很小，但类对象成员可能少一次默认构造 + 赋值，并且 `const` 成员、引用成员必须通过初始化列表初始化。

## 本次最终代码

```cpp
struct DLinkedNode {
    int key, val;
    DLinkedNode* pre;
    DLinkedNode* next;
    DLinkedNode(): key(0), val(0), pre(nullptr), next(nullptr) {}
    DLinkedNode(int _key, int _val): key(_key), val(_val), pre(nullptr), next(nullptr) {}
};

class LRUCache {
    int size;
    int capacity;
    unordered_map<int, DLinkedNode*> umap;
    DLinkedNode* head;
    DLinkedNode* tail;
public:
    LRUCache(int capacity) {
        size = 0;
        this->capacity = capacity;
        head = new DLinkedNode();
        tail = new DLinkedNode();
        head->next = tail;
        tail->pre = head;
    }

    int get(int key) {
        if (umap.find(key) != umap.end()) {
            DLinkedNode* node = umap[key];
            moveToHead(node);
            return node->val;
        }
        return -1;
    }

    void put(int key, int value) {
        if (umap.find(key) == umap.end()) {
            DLinkedNode* node = new DLinkedNode(key, value);
            insertIntoHead(node);
            umap[key] = node;
            size++;

            if (size > capacity) {
                DLinkedNode* tNode = tail->pre;
                removeNode(tNode);
                umap.erase(tNode->key);
                delete(tNode);
                size--;
            }
        } else {
            DLinkedNode* node = umap[key];
            node->val = value;
            moveToHead(node);
        }
    }

    void moveToHead(DLinkedNode* node) {
        removeNode(node);
        insertIntoHead(node);
    }

    void removeNode(DLinkedNode* node) {
        node->pre->next = node->next;
        node->next->pre = node->pre;
    }

    void insertIntoHead(DLinkedNode* node) {
        node->next = head->next;
        node->pre = head;
        head->next = node;
        node->next->pre = node;
    }
};
```

## 面试表达

> 我使用哈希表加双向链表实现 LRU。哈希表保存 `key -> Node*`，用于平均 `O(1)` 定位节点；双向链表按照访问时间维护节点，头部表示最近使用，尾部表示最久未使用。`get` 命中后将节点移动到头部；`put` 如果 key 已存在就更新 value 并移动到头部，如果不存在则创建节点插入头部。当容量超限时删除尾部节点并同步删除哈希表记录。双向链表可以在已知节点指针的情况下 `O(1)` 删除，因此 `get` 和 `put` 平均都是 `O(1)`。

## 复杂度

- `get`：平均 `O(1)`。
- `put`：平均 `O(1)`。
- 空间复杂度：`O(capacity)`。

## 复习关键词

`unordered_map + 双向链表`、虚拟头尾节点、`key -> Node*`、`head` 最新、`tail` 最旧、`moveToHead = remove + insert`、哈希表与链表同步、野指针、成员初始化列表。
