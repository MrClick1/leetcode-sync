#include <cstdlib>
#include <iostream>
#include <string>

#include "solution.cpp"

using namespace std;

static void expectEqual(int actual, int expected, const string& context) {
    if (actual != expected) {
        cerr << "FAIL: " << context
             << " (expected " << expected
             << ", got " << actual << ")" << endl;
        exit(1);
    }
}

static void testExample() {
    LRUCache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    expectEqual(cache.get(1), 1, "example: get existing key 1");

    cache.put(3, 3);
    expectEqual(cache.get(2), -1, "example: key 2 should be evicted");

    cache.put(4, 4);
    expectEqual(cache.get(1), -1, "example: key 1 should be evicted");
    expectEqual(cache.get(3), 3, "example: get key 3");
    expectEqual(cache.get(4), 4, "example: get key 4");
}

static void testCapacityOne() {
    LRUCache cache(1);
    expectEqual(cache.get(0), -1, "capacity one: initially empty");

    cache.put(0, 0);
    expectEqual(cache.get(0), 0, "capacity one: zero key and value");

    cache.put(0, 100000);
    expectEqual(cache.get(0), 100000, "capacity one: update existing key");

    cache.put(10000, 7);
    expectEqual(cache.get(0), -1, "capacity one: old key should be evicted");
    expectEqual(cache.get(10000), 7, "capacity one: new key should remain");
}

static void testUpdateRefreshesRecency() {
    LRUCache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    cache.put(1, 10);
    cache.put(3, 3);

    expectEqual(cache.get(2), -1, "update: key 2 should be least recently used");
    expectEqual(cache.get(1), 10, "update: key 1 should have the new value");
    expectEqual(cache.get(3), 3, "update: key 3 should remain");
}

static void testGetRefreshesRecency() {
    LRUCache cache(3);
    cache.put(1, 10);
    cache.put(2, 20);
    cache.put(3, 30);

    expectEqual(cache.get(1), 10, "get refresh: access key 1");
    cache.put(4, 40);

    expectEqual(cache.get(2), -1, "get refresh: key 2 should be evicted");
    expectEqual(cache.get(1), 10, "get refresh: key 1 should remain");
    expectEqual(cache.get(3), 30, "get refresh: key 3 should remain");
    expectEqual(cache.get(4), 40, "get refresh: key 4 should remain");
}

static void testMissingGetDoesNotChangeOrder() {
    LRUCache cache(2);
    cache.put(5, 50);
    cache.put(6, 60);

    expectEqual(cache.get(99), -1, "missing get: absent key");
    cache.put(7, 70);

    expectEqual(cache.get(5), -1, "missing get: key 5 should still be evicted");
    expectEqual(cache.get(6), 60, "missing get: key 6 should remain");
    expectEqual(cache.get(7), 70, "missing get: key 7 should remain");
}

static void testMaximumCapacity() {
    LRUCache cache(3000);
    for (int key = 0; key < 3000; ++key) {
        cache.put(key, key + 1);
    }

    expectEqual(cache.get(0), 1, "maximum capacity: refresh key 0");
    cache.put(3000, 3001);

    expectEqual(cache.get(1), -1, "maximum capacity: key 1 should be evicted");
    expectEqual(cache.get(0), 1, "maximum capacity: refreshed key 0 should remain");
    expectEqual(cache.get(3000), 3001, "maximum capacity: new key should remain");
}

int main() {
    testExample();
    testCapacityOne();
    testUpdateRefreshesRecency();
    testGetRefreshesRecency();
    testMissingGetDoesNotChangeOrder();
    testMaximumCapacity();

    cout << "Scaffold tests passed!" << endl;
    return 0;
}
