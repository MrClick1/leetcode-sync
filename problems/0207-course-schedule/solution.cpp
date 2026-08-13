#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
        // TODO: 在这里实现你的解法
        vector<vector<int>> adj(numCourses);   // 邻接表
        vector<int> indegree(numCourses, 0);   // 入度

        for (auto& p : prerequisites) {
            int a = p[0], b = p[1];
            adj[b].push_back(a);               // b -> a
            indegree[a]++;                     // a 的入度 +1
        }

        queue<int> q;
        for (int i = 0; i < numCourses; i++)
            if (indegree[i] == 0) q.push(i);   // 没有先修课的课先入队

        int done = 0;                          // 学完的课程数
        while (!q.empty()) {
            int c = q.front();
            q.pop();
            done++;
            // TODO: 遍历 adj[c] 里的每一门后继课：
            //       indegree[后继]--，如果变成 0 就入队
            for (int i = 0; i < (int)adj[c].size(); i++) {
                int courseNum = adj[c][i];
                indegree[courseNum]--;
                if (indegree[courseNum] == 0) {
                    q.push(courseNum);
                }
            }
        }

        return done == numCourses;             // 学完的课不够 = 有环
    }
};