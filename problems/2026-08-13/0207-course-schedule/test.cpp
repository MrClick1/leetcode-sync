#include <iostream>
#include <vector>

#include "solution.cpp"

static void check(int numCourses, std::vector<std::vector<int>> prerequisites,
                  bool expected) {
    bool got = Solution().canFinish(numCourses, prerequisites);
    if (got != expected) {
        std::cerr << "FAIL: numCourses=" << numCourses
                  << " expected " << (expected ? "true" : "false")
                  << ", got " << (got ? "true" : "false") << "\n";
        std::exit(1);
    }
}

int main() {
    // 示例 1、2
    check(2, {{1, 0}}, true);
    check(2, {{1, 0}, {0, 1}}, false);

    // 边界：没有先修课程
    check(1, {}, true);
    check(5, {}, true);

    // 自环：一门课先修自己，不可能
    check(1, {{0, 0}}, false);

    // 链式依赖
    check(3, {{1, 0}, {2, 1}}, true);

    // 三元环
    check(3, {{1, 0}, {2, 1}, {0, 2}}, false);

    // 环不在 0 号课程上（孤立子图里有环）
    check(4, {{1, 0}, {2, 3}, {3, 2}}, false);

    // 有向无环的复杂依赖
    check(4, {{1, 0}, {2, 0}, {3, 1}, {3, 2}}, true);

    // 菱形依赖（无环）
    check(4, {{1, 0}, {2, 0}, {3, 1}, {3, 2}, {2, 1}}, true);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}