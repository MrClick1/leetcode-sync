#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    int longestValidParentheses(string s) {
        int result = 0;
        int n = static_cast<int>(s.size());
        // 保存虚拟边界、未匹配左括号和未匹配右括号的下标。
        stack<int> stk;
        stk.push(-1);

        for (int i = 0; i < n; ++i) {
            char c = s[i];

            if (c == '(') {
                stk.push(i);
            } else {
                if (stk.top() == -1) {
                    // 当前右括号无法匹配，成为新的连续子串边界。
                    stk.push(i);
                } else if (s[stk.top()] == '(') {
                    stk.pop();
                    result = max(result, i - stk.top());
                } else {
                    // 栈顶是之前未匹配的右括号，当前下标成为更新的边界。
                    stk.push(i);
                }
            }
        }

        return result;
    }
};
