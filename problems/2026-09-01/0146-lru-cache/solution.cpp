#include <bits/stdc++.h>

using namespace std;

struct DListNode {
    int key;
    int val;
    DListNode* next;
    DListNode* pre;

    DListNode(): key(0), val(0), next(nullptr), pre(nullptr) {}
    DListNode(int _key, int _val): key(_key), val(_val), next(nullptr), pre(nullptr) {}
    // ListNode(int _val, ListNode* _next): val(_val), next(_next) {}
};

class LRUCache {
private:
    int size;
    int capacity;
    unordered_map<int, DListNode*> umap; // [key, DListNode*]
    DListNode* head;
    DListNode* tail;

public:
    explicit LRUCache(int capacity) {
        // 在这里完成初始化
        size = 0;
        this->capacity = capacity;

        head = new DListNode();
        tail = new DListNode();
        head->next = tail;
        tail->pre = head;
    }

    int get(int key) {
        // 在这里实现你的解法
        if (umap.find(key) != umap.end()) {
            DListNode* node = umap[key];
            // 放到队头
            removeNode(node);
            insertIntoHead(node);
            return node->val;
        }

        return -1;
    }

    void put(int key, int value) {
        // 在这里实现你的解法
        if (umap.find(key) != umap.end()) {
            // LRU 中有 对应节点，更新节点，插入链表头，更新 umap
            DListNode* node = umap[key];
            node->val = value;
            removeNode(node);
            insertIntoHead(node);
            umap[key] = node;
        }else {
            DListNode* node = new DListNode(key, value);

            // 新增节点有可能会导致超过 capacity
            if (size != capacity) {
                // 没超过 capacity，直接插入链表头，插入 umap，size++
                insertIntoHead(node);
                umap[key] = node;
                size++;
            }else {
                // 超过 capacity
                // 删除尾节点
                DListNode* removedNode = tail->pre;
                // 删除双向链表中的尾节点，清除 umap 中的尾节点，插入并且更新 umap
                removeNode(removedNode);
                umap.erase(umap.find(removedNode->key));
                insertIntoHead(node);
                umap[key] = node;
            }
        }
    }

    void insertIntoHead(DListNode* node) {
        DListNode* nextNode = head->next;
        head->next = node;
        nextNode->pre = node;
        node->pre = head;
        node->next = nextNode;
    }

    void removeNode(DListNode* node) {
        DListNode* preNode = node->pre;
        DListNode* nextNode = node->next;
        preNode->next = nextNode;
        nextNode->pre = preNode;
    }
};
