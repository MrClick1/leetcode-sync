#include <bits/stdc++.h>

using namespace std;

struct ListNode {
    int val;
    ListNode* next;

    ListNode(): val(0), next(nullptr) {}
    ListNode(int _val): val(_val), next(nullptr) {}
    ListNode(int _val, ListNode* _next): val(_val), next(_next) {}
};

class LRUCache {
private:
    int size;
    int capacity;
    unordered_map<int, ListNode*> umap;
    ListNode* head;

public:
    explicit LRUCache(int capacity) {
        // 在这里完成初始化
        size = 0;
        this->capacity = capacity;

        head = new ListNode();
    }

    int get(int key) {
        // 在这里实现你的解法
        if (umap.find(key) != umap.end()) {
            ListNode* node = umap[key];
            insertIntoHead(node);
        }else {
            return -1;
        }
    }

    void put(int key, int value) {
        // 在这里实现你的解法
        (void)key;
        (void)value;
    }

    void insertIntoHead(ListNode* node) {

    }

};
