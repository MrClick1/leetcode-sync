# 146. LRU 缓存（三刷）

状态：2026-09-23 用户确认本轮代码在 LeetCode 提交通过；仍感觉指针操作不够熟练。此前记录：[一刷](../2026-09-01/0146-lru-cache.md)、[二刷](../2026-09-14/0146-lru-cache.md)。

## 两个结构各管一件事

```text
unordered_map<key, Node*>：按 key 平均 O(1) 找节点
head ⇄ 最新使用 ⇄ ... ⇄ 最久未使用 ⇄ tail：维护使用顺序
```

`head` 和 `tail` 是哨兵节点，不存缓存项。链表中的每个真实节点恰好对应哈希表中的一条记录。`get` 命中、`put` 更新已有 key，都算“使用”，必须把节点移到头部。容量超限时，`tail->prev` 就是要淘汰的节点。

## 写代码时先固定三个链表操作

```cpp
void removeNode(DLinkedNode* node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

void insertIntoHead(DLinkedNode* node) {
    node->prev = head;
    node->next = head->next;
    head->next->prev = node;
    head->next = node;
}

void moveToHead(DLinkedNode* node) {
    removeNode(node);
    insertIntoHead(node);
}
```

头插前是 `head ⇄ oldFirst`，头插后是 `head ⇄ node ⇄ oldFirst`。前两行让新节点指向两侧，后两行让两侧指回新节点。`removeNode` 只能处理已经在链表中的节点：`new` 出来的节点 `prev/next` 仍为 `nullptr`，不能先摘除。

## get 与 put 的操作清单

| 操作 | 顺序 |
|---|---|
| `get(key)` 未命中 | 返回 -1，不改链表 |
| `get(key)` 命中 | 哈希表找节点 → 移到头部 → 返回值 |
| `put(key,value)` 已存在 | 更新值 → 移到头部 |
| `put(key,value)` 不存在 | 创建节点 → 直接头插 → 写入哈希表 → 容量超限则淘汰尾节点 |

淘汰时按以下顺序处理，确保链表、哈希表和节点生命周期同步：

```cpp
DLinkedNode* victim = tail->prev;
removeNode(victim);
umap.erase(victim->key); // 必须在 delete 前读取 key
delete victim;
--size;
```

## 本轮调试轨迹

1. 最初 `insertIntoHead` 内部先调用 `removeNode`：新节点还没有进入链表，`prev/next` 为空，发生空指针访问。
2. 拆分 `moveToHead` 与 `insertIntoHead` 后，头插只改了 `head->next` 和旧首节点的 `prev`，漏设新节点自己的 `prev/next`，之后移动或删除时再次访问空指针。
3. 头插四条连接补齐后，淘汰仍只从链表摘除并 `delete`，忘了 `umap.erase(key)`。之后 `get` 命中已释放节点，引发 AddressSanitizer 的 `heap-use-after-free`。
4. 补上哈希表删除后，用户确认提交通过。

这三类错误可以用一句话检查：**新节点先接完整，旧节点才摘除；淘汰时链表、映射、内存一起清理。**

## 容量为 2 的手动追踪

```text
put(1,1)   head ⇄ 1 ⇄ tail       map={1}
put(2,2)   head ⇄ 2 ⇄ 1 ⇄ tail   map={1,2}
get(1)     head ⇄ 1 ⇄ 2 ⇄ tail   返回 1
put(3,3)   head ⇄ 3 ⇄ 1 ⇄ tail   淘汰 2，map={1,3}
get(2)     未命中，返回 -1
```

## 代码整理建议

- `insertIntoHead` 可改名 `addToHead`，突出它是纯头插；`moveToHead` 则明确是摘除加头插。
- `umap.count(key)` 后再用 `umap[key]` 会做两次哈希查找，可用 `auto it = umap.find(key)` 一次定位；两种写法在这里都正确。
- `size` 可以保留，也可以统一用 `umap.size()`，避免维护两份数量状态。
- `delete(tailNode)` 合法，习惯上写 `delete tailNode;`。
- 哨兵节点的 `key/val` 在当前算法中不会被读取，默认构造函数没有初始化它们不影响本题；初始化为 0 更便于检查。
- 本题提交版未定义析构函数，因此对象销毁时仍在缓存中的节点和两个哨兵节点没有主动释放。练习或工程代码可补析构清理；若类拥有裸指针，复制语义也需另行处理。

这些是可读性和资源管理建议，用户已通过的核心算法无需改动。

## 复杂度与复习方法

`get`、`put` 平均 O(1)；缓存中最多有 `capacity` 个真实节点，空间 O(capacity)。

复写时建议按顺序默写：`Node` → 构造头尾哨兵 → `removeNode` → `insertIntoHead` → `moveToHead` → `get` → `put` 的已存在/新建/淘汰三个分支。每写完一步就用“链表里有哪些 key，map 里有哪些 key”核对两边一致。
