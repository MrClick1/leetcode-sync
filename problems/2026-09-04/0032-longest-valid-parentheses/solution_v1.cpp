#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    int longestValidParentheses(string s) {
        int result = 0;
        int n = static_cast<int>(s.size());
        stack<int> stk;    // 栈
        stk.push(-1);

        for (int i = 0; i < n; i++) {
            char c = s[i];

            if (c == '(') {
                stk.push(i);
            }else {
                if (stk.top() == -1) {
                    stk.push(i);
                }else if (s[stk.top()] == '(') {
                    stk.pop();
                    result = max(result, i - stk.top());
                }else {
                    stk.push(i);
                }
            }
        }

        return result;
    }
};
