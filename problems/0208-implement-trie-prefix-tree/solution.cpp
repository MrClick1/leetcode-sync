#include <bits/stdc++.h>

using namespace std;

class TrieNode {
public:
    TrieNode* children[26] = {nullptr};     // 26 个孩子，对应 'a'~'z'
    bool isEnd = false;
};

class Trie {
private:
    TrieNode* root;

public:
    Trie() {
        root = new TrieNode();
    }

    TrieNode* findNode(const string& s) {
        TrieNode* cur = root;
        for (char c : s) {
            int idx = c - 'a';
            if (cur->children[idx] == nullptr) return nullptr;
            cur = cur->children[idx];
        }
        return cur;
    }

    void insert(string word) {
        // 先找到 word 的前缀节点
        TrieNode* cur = findNode(word);
        if (cur != nullptr) {
            // 前缀节点存在，说明 word 已经存在于 Trie 中，将 cur->isEnd 设置为 true 即可
            cur->isEnd = true;
        }else {
            // 前缀节点不存在，从头开始插入
            cur = root;
            for (char c : word) {
                int idx = c - 'a';
                if (cur->children[idx]) {
                    cur = cur->children[idx];
                }else {
                    TrieNode* node = new TrieNode();
                    cur->children[idx] = node;
                    cur = node;
                }
            }
            cur->isEnd = true;
        }
    }

    bool search(string word) {
        TrieNode* cur = root;
        
        // 从头开始找，中间找不到 -> false 
        // 结束时找到了，但是 isEnd 为 false - > false
        // 找到最后了，且 isEnd = true -> true
        int count = 0;
        for (char c : word) {
            int idx = c - 'a';
            if (cur->children[idx]) {
                cur =  cur->children[idx];
                count++;
            }else {
                return false;   // 中间找不到
            }
        }

        return cur->isEnd;
    }

    bool startsWith(string prefix) {
        TrieNode* cur = root;
        // 从头开始找，找完就行
        for (char c : prefix) {
            int idx = c - 'a';
            if (cur->children[idx]) {
                cur = cur->children[idx];
            }else return false;
        }

        return true;
    }
};