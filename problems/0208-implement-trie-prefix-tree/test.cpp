#include <iostream>
#include <string>

#include "solution.cpp"

static void expect(bool got, bool expected, const std::string& what) {
    if (got != expected) {
        std::cerr << "FAIL: " << what
                  << " expected " << (expected ? "true" : "false")
                  << ", got " << (got ? "true" : "false") << "\n";
        std::exit(1);
    }
}

int main() {
    // 官方示例
    {
        Trie trie;
        trie.insert("apple");
        expect(trie.search("apple"), true, "search apple");
        expect(trie.search("app"), false, "search app");
        expect(trie.startsWith("app"), true, "startsWith app");
        trie.insert("app");
        expect(trie.search("app"), true, "search app after insert");
    }

    // 单字符
    {
        Trie trie;
        trie.insert("a");
        expect(trie.search("a"), true, "search a");
        expect(trie.search("b"), false, "search b");
        expect(trie.startsWith("a"), true, "startsWith a");
        expect(trie.startsWith("ab"), false, "startsWith ab");
    }

    // 前缀是已插入词的完整词
    {
        Trie trie;
        trie.insert("hello");
        expect(trie.search("he"), false, "search he");
        expect(trie.startsWith("he"), true, "startsWith he");
        expect(trie.search("hello"), true, "search hello");
    }

    // 长公共前缀
    {
        Trie trie;
        trie.insert("abcd");
        trie.insert("abef");
        expect(trie.search("abc"), false, "search abc");
        expect(trie.startsWith("ab"), true, "startsWith ab");
        expect(trie.startsWith("abe"), true, "startsWith abe");
        expect(trie.search("abef"), true, "search abef");
        expect(trie.search("abefg"), false, "search abefg");
    }

    // 重复插入
    {
        Trie trie;
        trie.insert("abc");
        trie.insert("abc");
        expect(trie.search("abc"), true, "search abc twice");
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}