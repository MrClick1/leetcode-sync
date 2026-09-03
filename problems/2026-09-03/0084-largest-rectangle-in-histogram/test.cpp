#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>

#include "solution.cpp"

static void check(const std::vector<int>& input, int expected) {
    std::vector<int> heights = input;
    int actual = Solution().largestRectangleArea(heights);

    if (actual != expected) {
        std::cerr << "FAIL: expected " << expected
                  << ", got " << actual
                  << " (length " << input.size() << ")\n";
        if (input.size() <= 20) {
            std::cerr << "  heights: [";
            for (std::size_t i = 0; i < input.size(); ++i) {
                if (i > 0) {
                    std::cerr << ",";
                }
                std::cerr << input[i];
            }
            std::cerr << "]\n";
        }
        std::exit(1);
    }
}

// Independent reference: enumerate every interval and its minimum height.
static int bruteForceMaxArea(const std::vector<int>& heights) {
    const int n = static_cast<int>(heights.size());
    int best = 0;
    for (int left = 0; left < n; ++left) {
        int minHeight = heights[left];
        for (int right = left; right < n; ++right) {
            minHeight = std::min(minHeight, heights[right]);
            best = std::max(best, minHeight * (right - left + 1));
        }
    }
    return best;
}

static void checkRandomCases() {
    std::mt19937 generator(20260903);
    std::uniform_int_distribution<int> length(1, 80);
    std::uniform_int_distribution<int> smallHeight(0, 5);
    std::uniform_int_distribution<int> fullHeight(0, 10000);

    for (int caseIndex = 0; caseIndex < 10000; ++caseIndex) {
        std::vector<int> heights(length(generator));
        for (int& height : heights) {
            // Small heights make zeros and equal-height plateaus frequent.
            height = caseIndex % 2 == 0 ? smallHeight(generator)
                                        : fullHeight(generator);
        }
        check(heights, bruteForceMaxArea(heights));
    }
}

int main() {
    // Official examples
    check({2, 1, 5, 6, 2, 3}, 10);
    check({2, 4}, 4);

    // One bar and zero heights
    check({0}, 0);
    check({7}, 7);
    check({10000}, 10000);
    check({0, 0, 0, 0}, 0);

    // Equal, increasing, and decreasing heights
    check({3, 3, 3, 3}, 12);
    check({2, 2}, 4);
    check({1, 2, 3, 4, 5}, 9);
    check({5, 4, 3, 2, 1}, 9);

    // Zeros separate rectangles
    check({2, 0, 2}, 2);
    check({0, 2, 2, 0}, 4);
    check({2, 2, 0, 3, 3}, 6);

    // Mixed heights and repeated plateaus
    check({2, 1, 2}, 3);
    check({1, 3, 1}, 3);
    check({3, 1, 3, 2, 2}, 6);
    check({4, 2, 0, 3, 2, 5}, 6);
    check({6, 2, 5, 4, 5, 1, 6}, 12);
    check({5, 5, 1, 5, 5}, 10);
    check({2, 2, 1, 2, 2}, 5);

    // Maximum length and maximum possible area
    check(std::vector<int>(100000, 10000), 1000000000);
    check(std::vector<int>(100000, 0), 0);

    std::vector<int> alternating(100000, 0);
    for (std::size_t i = 0; i < alternating.size(); i += 2) {
        alternating[i] = 10000;
    }
    check(alternating, 10000);

    checkRandomCases();
    std::cout << "Random differential tests passed (10000 cases).\n";
    std::cout << "Scaffold tests passed!" << std::endl;
    return 0;
}
