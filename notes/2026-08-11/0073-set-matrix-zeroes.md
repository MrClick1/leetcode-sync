# 73. 矩阵置零 —— 复盘笔记

状态：✅ 2026-10-06 用户行列标记数组版已力扣通过，额外空间 O(m+n)；首次练习的 O(1) 空间版本复盘保留在下方。

## 2026-10-06 标记数组通过版本

先记录原矩阵中所有零的行列，再统一修改；新写入的零不会触发额外的行列清零。

```cpp
class Solution {
public:
    void setZeroes(vector<vector<int>>& matrix) {
        int m = (int)matrix.size();
        int n = (int)matrix[0].size();
        vector<bool> row(m, false);
        vector<bool> col(n, false);

        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (matrix[i][j] == 0) {
                    row[i] = true;
                    col[j] = true;
                }
            }
        }
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (row[i] || col[j]) {
                    matrix[i][j] = 0;
                }
            }
        }
    }
};
```

时间 O(mn)，额外空间 O(m+n)，结果直接写回输入矩阵。

## 用 assign 清零整行

`matrix[i]` 是一个独立的 `vector<int>`，可以执行 `matrix[i].assign(n, 0)`，将这一行替换为 n 个零。`assign(数量, 值)` 修改当前 vector 并返回 void；不要把返回值再赋给 matrix[i]。若只想覆盖已有元素且保持长度，也可以使用 `fill(matrix[i].begin(), matrix[i].end(), 0)`。

一列由各行的第 j 个元素组成，没有单独的列 vector，因此清列需要遍历行。第一遍标记不变，第二遍也可以这样组织：

```cpp
for (int i = 0; i < m; ++i) {
    if (row[i]) {
        matrix[i].assign(n, 0);
    }
}
for (int j = 0; j < n; ++j) {
    if (col[j]) {
        for (int i = 0; i < m; ++i) {
            matrix[i][j] = 0;
        }
    }
}
```

判断条件始终读取第一遍保存的 row、col，不再根据修改后的矩阵查找新的零。不能在扫描原矩阵时，遇到零就立即 assign 一整行，否则后续可能把新写入的零当成原有零，扩大清零范围。此替代写法仍为 O(mn) 时间、O(m+n) 额外空间。

assign 的常用重载与 resize 区别见 [C++ 语法笔记](../01-cpp语法知识文档.md#resize-与-assign)。

## 首次练习的常量空间版本

以下错误与要点属于首次练习，本轮标记数组版通过不代表已重新掌握常量空间实现。

### 遇到的问题

1. **第一版**：把所有 0 的坐标存进 `vector`，再逐个清行/列
   - 空间最坏 O(mn)（全 0 时存了 2mn 个 int）
   - 时间最坏 O(mn·(m+n))：同一行/列会被重复清零
2. **O(1) 版第一次翻车**：清零动作污染了标记
   - 先清第一行时，把存在第一行里的"列标记"全冲掉了
   - 后面按列遍历时读到被污染的标记，连锁把整张矩阵清成全 0
3. **O(1) 版第二次翻车**：检测"第一行是否有 0"的循环误写成遍历全矩阵
   - 任何位置有 0 都会把 `firstRowZero` 置 true，第一行被误清

### 解题要点

- 用矩阵自身做标记：`matrix[i][0]` 记第 i 行，`matrix[0][j]` 记第 j 列
- 两个 bool（`firstRowZero` / `firstColZero`）兜住第一行/第一列原本是否有 0
  - 因为 `matrix[0][0]` 同时是"行标记"和"列标记"，有歧义
- 正确顺序：
  1. 先记录第一行/列原本是否含 0
  2. 只扫内部区域（i≥1, j≥1）打标记
  3. 只清内部区域（不碰标记区）
  4. 最后按 bool 处理第一行/第一列
- 关键教训：**标记区和数据区不能互相污染**——先读完标记，再动数据

### 复杂度

- 时间：O(mn)
- 空间：O(1)
