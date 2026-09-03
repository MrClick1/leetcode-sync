#include <bits/stdc++.h>

using namespace std;

class MedianFinder {
    priority_queue<int, vector<int>, less<int>> maxHeap; // 大顶堆，存储较小的一半元素
    priority_queue<int, vector<int>, greater<int>> minHeap; // 小顶堆
    int lsize;
    int rsize;
public:
    MedianFinder() {
        // 初始化你的数据结构
        lsize = 0;
        rsize = 0;
    }

    void addNum(int num) {
        // 添加一个整数
        // 空
        if (lsize == 0) {
            maxHeap.push(num);
            lsize++;
        }else if (rsize == 0){
            // left 不为空，right 为空
            int smallerMid = maxHeap.top();
            if (num <= smallerMid) {
                maxHeap.push(num);
                int temp = maxHeap.top();
                maxHeap.pop();

                minHeap.push(temp);
                rsize++;
            }else {
                minHeap.push(num);
                rsize++;
            }
        }else {
            // 两边都不为空
            int smallerMid = maxHeap.top();
            int largerMid = minHeap.top();

            if (lsize == rsize) {
                if (num <= smallerMid) {
                    maxHeap.push(num);
                    lsize++;
                } else {
                    minHeap.push(num);
                    // 调整为 maxHeap 比 minHeap 多一个元素
                    int temp = minHeap.top();
                    minHeap.pop();
                    maxHeap.push(temp);
                    lsize++;
                }
            }else if (lsize == rsize + 1) {
                if (num <= smallerMid) {
                    maxHeap.push(num);

                    int temp = maxHeap.top();
                    maxHeap.pop();
                    minHeap.push(temp);

                    rsize++;
                }else {
                    minHeap.push(num);
                    rsize++;
                }
            }
        }
    }

    double findMedian() {
        // 返回当前所有元素的中位数
        if (lsize == rsize + 1) {
            return (double)maxHeap.top();
        }

        int lMid = maxHeap.top();
        int rMid = minHeap.top();

        return (double)((lMid + rMid)) / 2;
    }
};
