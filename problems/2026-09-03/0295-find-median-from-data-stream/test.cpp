#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "solution.cpp"

static void expectMedian(double actual, double expected,
                         const std::string& context) {
    if (!std::isfinite(actual) || std::abs(actual - expected) > 1e-5) {
        std::cerr << std::setprecision(17)
                  << "FAIL: " << context
                  << " (expected " << expected << ", got " << actual << ")\n";
        std::exit(1);
    }
}

static void checkStream(const std::vector<int>& nums,
                        const std::vector<double>& expected,
                        const std::string& name) {
    if (nums.size() != expected.size()) {
        std::cerr << "FAIL: invalid test fixture for " << name << '\n';
        std::exit(1);
    }

    MedianFinder finder;
    for (std::size_t i = 0; i < nums.size(); ++i) {
        finder.addNum(nums[i]);
        const std::string context = name + ": after insertion "
                                  + std::to_string(i + 1);
        expectMedian(finder.findMedian(), expected[i], context);
        // Repeated queries must not remove data or change the median.
        expectMedian(finder.findMedian(), expected[i], context + " (repeat)");
    }
}

static void testExample() {
    MedianFinder finder;
    finder.addNum(1);
    finder.addNum(2);
    expectMedian(finder.findMedian(), 1.5, "example: after 1 and 2");
    finder.addNum(3);
    expectMedian(finder.findMedian(), 2.0, "example: after 1, 2 and 3");
}

static void testFixedStreams() {
    checkStream({0}, {0.0}, "single zero");
    checkStream({7}, {7.0}, "single positive");
    checkStream({-100000}, {-100000.0}, "single minimum");
    checkStream({100000}, {100000.0}, "single maximum");

    checkStream({1, 2, 3, 4, 5, 6},
                {1, 1.5, 2, 2.5, 3, 3.5}, "increasing");
    checkStream({6, 5, 4, 3, 2, 1},
                {6, 5.5, 5, 4.5, 4, 3.5}, "decreasing");
    checkStream({-1, -2, -3, -4, -5},
                {-1, -1.5, -2, -2.5, -3}, "negative fractions");
    checkStream({-5, 10, -10, 0, 5, 3},
                {-5, 2.5, -5, -2.5, 0, 1.5}, "mixed signs");
    checkStream({5, 15, 1, 3, 8, 7, 9, 10, 6},
                {5, 10, 5, 4, 5, 6, 7, 7.5, 7}, "unsorted stream");

    checkStream({4, 4, 4, 4}, {4, 4, 4, 4}, "equal values");
    checkStream({0, 0, 0}, {0, 0, 0}, "zero values");
    checkStream({2, 2, 1, 1, 3, 3},
                {2, 2, 2, 1.5, 2, 2}, "mixed duplicates");
    checkStream({-100000, 100000, -100000, 100000},
                {-100000, 0, -100000, 0}, "alternating limits");
    checkStream({100000, 100000}, {100000, 100000}, "equal maximums");
    checkStream({-100000, -100000}, {-100000, -100000}, "equal minimums");
    checkStream({-100000, -99999}, {-100000, -99999.5}, "fraction near minimum");
    checkStream({5, 1, 2}, {5, 3, 2}, "regression: reverse first pair");
}

static void testIndependentInstances() {
    MedianFinder first;
    MedianFinder second;
    first.addNum(5);
    second.addNum(-8);
    expectMedian(first.findMedian(), 5.0, "first instance");
    expectMedian(second.findMedian(), -8.0, "second instance");

    first.addNum(10);
    second.addNum(10);
    expectMedian(first.findMedian(), 7.5, "first instance after update");
    expectMedian(second.findMedian(), 1.0, "second instance after update");
}

static void testLargeStreams() {
    // Each instance below receives exactly 50000 add/query calls in total.
    constexpr int count = 25000;
    {
        MedianFinder finder;
        for (int i = 0; i < count; ++i) {
            const int value = i - 12500;
            finder.addNum(value);
            const double expected = (-12500.0 + value) / 2.0;
            expectMedian(finder.findMedian(), expected,
                         "large increasing: insertion " + std::to_string(i + 1));
        }
    }
    {
        MedianFinder finder;
        for (int i = 0; i < count; ++i) {
            const int value = 12500 - i;
            finder.addNum(value);
            const double expected = (12500.0 + value) / 2.0;
            expectMedian(finder.findMedian(), expected,
                         "large decreasing: insertion " + std::to_string(i + 1));
        }
    }
    {
        MedianFinder finder;
        for (int i = 0; i < count; ++i) {
            finder.addNum(i % 2 == 0 ? -100000 : 100000);
            const double expected = i % 2 == 0 ? -100000.0 : 0.0;
            expectMedian(finder.findMedian(), expected,
                         "large alternating: insertion " + std::to_string(i + 1));
        }
    }
    {
        MedianFinder finder;
        for (int i = 0; i < count; ++i) {
            finder.addNum(100000);
            expectMedian(finder.findMedian(), 100000.0,
                         "large duplicates: insertion " + std::to_string(i + 1));
        }
    }
    {
        MedianFinder finder;
        // 49999 additions followed by one query: [-24999, ..., 24999].
        for (int i = -24999; i <= 24999; ++i) {
            finder.addNum(i);
        }
        expectMedian(finder.findMedian(), 0.0, "large batch before first query");
    }
}

static void testRandomStreams() {
    std::mt19937 generator(20260903);
    std::uniform_int_distribution<int> length(1, 100);
    std::uniform_int_distribution<int> fullValue(-100000, 100000);
    std::uniform_int_distribution<int> smallValue(-5, 5);
    std::uniform_int_distribution<int> specialIndex(0, 4);
    const int specialValues[] = {-100000, -1, 0, 1, 100000};

    for (int caseIndex = 0; caseIndex < 10000; ++caseIndex) {
        MedianFinder finder;
        std::vector<int> sorted;
        const int count = length(generator);
        for (int i = 0; i < count; ++i) {
            int num;
            if (caseIndex % 3 == 0) {
                num = fullValue(generator);
            } else if (caseIndex % 3 == 1) {
                num = smallValue(generator);
            } else {
                num = specialValues[specialIndex(generator)];
            }

            finder.addNum(num);
            // Independent reference: keep every value in a sorted vector.
            sorted.insert(std::lower_bound(sorted.begin(), sorted.end(), num), num);
            const std::size_t n = sorted.size();
            const double expected = n % 2 == 1
                ? static_cast<double>(sorted[n / 2])
                : (static_cast<double>(sorted[n / 2 - 1]) + sorted[n / 2]) / 2.0;
            const std::string context = "random case " + std::to_string(caseIndex)
                                      + ": insertion " + std::to_string(i + 1)
                                      + ", value " + std::to_string(num);
            expectMedian(finder.findMedian(), expected, context);
            expectMedian(finder.findMedian(), expected, context + " (repeat)");
        }
    }
}

int main() {
    testExample();
    testFixedStreams();
    testIndependentInstances();
    testLargeStreams();
    testRandomStreams();

    std::cout << "Random differential tests passed (10000 streams).\n";
    std::cout << "Scaffold tests passed!" << std::endl;
    return 0;
}
