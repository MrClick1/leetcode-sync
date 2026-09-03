#include <bits/stdc++.h>

using namespace std;

class MedianFinder {
    vector<int> data;
    int size = 0;
public:
    MedianFinder() {
        // 初始化你的数据结构
        size = 0;
    }

    void addNum(int num) {
        // 添加一个整数
        data.push_back(num);
        sort(data.begin(), data.end());
        size++;
    }

    double findMedian() {
        // 返回当前所有元素的中位数
        if (size % 2 == 0) {
            int left = (size-1) / 2;
            int right = size / 2;

            return static_cast<double>((data[left] + data[right])) / 2;
        }

        return static_cast<double>(data[size / 2]);
    }
};
