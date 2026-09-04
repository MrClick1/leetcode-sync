#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>

#include "solution.cpp"

static void check(const std::string& input, int expected,
                  const std::string& context) {
    const int actual = Solution().longestValidParentheses(input);
    if (actual != expected) {
        std::cerr << "FAIL: " << context
                  << " (expected " << expected
                  << ", got " << actual
                  << ", length " << input.size() << ")\n";
        if (input.size() <= 80) {
            std::cerr << "  input: \"" << input << "\"\n";
        }
        std::exit(1);
    }
}

static void testExamples() {
    check("(()", 2, "example 1");
    check(")()())", 4, "example 2");
    check("", 0, "example 3");
}

static void testBasicCases() {
    check("(", 0, "single left parenthesis");
    check(")", 0, "single right parenthesis");
    check("()", 2, "one pair");
    check(")(", 0, "reversed pair");
    check("())", 2, "extra right parenthesis");
    check("((()", 2, "extra left parentheses");
    check("()(()", 2, "valid parts separated by an unmatched left parenthesis");
    check("(()(()", 2, "nested but incomplete suffix");
}

static void testNestedAndConnectedCases() {
    check("()()", 4, "two adjacent pairs");
    check("(())", 4, "one nested pair");
    check("((()))", 6, "three nested pairs");
    check("(()())", 6, "mixed nesting");
    check("()(())", 6, "adjacent and nested parts connect");
    check("()(()())", 8, "pair followed by nested group");
    check(")(()())(", 6, "valid group between unmatched ends");
}

static void testResetAndMixedCases() {
    check("())((())", 4, "unmatched right parenthesis resets the start");
    check(")()())()()(", 4, "multiple valid regions");
    check("(()())())(()", 8, "long valid prefix and incomplete suffix");
    check("(()(((()", 2, "many unmatched left parentheses");
    check("())(())(()())", 10, "choose the longer region after a reset");
    check("(()())(()", 6, "valid prefix with incomplete suffix");
    check("())()(()))(()())", 6, "several regions with equal best length");
}

static void testMaximumLengthCases() {
    check(std::string(30000, '('), 0, "maximum length: all left parentheses");
    check(std::string(30000, ')'), 0, "maximum length: all right parentheses");

    std::string adjacent;
    adjacent.reserve(30000);
    for (int i = 0; i < 15000; ++i) {
        adjacent += "()";
    }
    check(adjacent, 30000, "maximum length: adjacent pairs");

    const std::string nested = std::string(15000, '(')
                             + std::string(15000, ')');
    check(nested, 30000, "maximum length: fully nested");

    const std::string separated = std::string(5000, '(')
                                + std::string(5000, ')')
                                + ")"
                                + std::string(6000, '(')
                                + std::string(6000, ')');
    check(separated, 12000, "large valid regions separated by an unmatched right parenthesis");

    const std::string unmatchedEnds = ")" + std::string(14999, '(')
                                    + std::string(14999, ')') + "(";
    check(unmatchedEnds, 29998, "maximum length with unmatched ends");
}

static int bruteForceLongestValidParentheses(const std::string& input) {
    int best = 0;
    const int n = static_cast<int>(input.size());

    for (int left = 0; left < n; ++left) {
        int balance = 0;
        for (int right = left; right < n; ++right) {
            balance += input[right] == '(' ? 1 : -1;
            if (balance < 0) {
                break;
            }
            if (balance == 0) {
                best = std::max(best, right - left + 1);
            }
        }
    }

    return best;
}

static void testRandomCases() {
    std::mt19937 generator(20260904);
    std::uniform_int_distribution<int> lengthDistribution(0, 100);
    std::uniform_int_distribution<int> parenthesisDistribution(0, 1);

    constexpr int caseCount = 10000;
    for (int caseIndex = 0; caseIndex < caseCount; ++caseIndex) {
        const int length = lengthDistribution(generator);
        std::string input;
        input.reserve(length);

        for (int i = 0; i < length; ++i) {
            input += parenthesisDistribution(generator) == 0 ? '(' : ')';
        }

        check(input, bruteForceLongestValidParentheses(input),
              "random case " + std::to_string(caseIndex));
    }

    std::cout << "Random differential tests passed ("
              << caseCount << " cases)." << std::endl;
}

int main() {
    testExamples();
    testBasicCases();
    testNestedAndConnectedCases();
    testResetAndMixedCases();
    testMaximumLengthCases();
    testRandomCases();

    std::cout << "Scaffold tests passed!" << std::endl;
    return 0;
}
