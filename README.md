# LeetCode 本地练习仓

这个目录用于和 Codex 一起做离线刷题练习：我出题并提供测试用例，你写解法并跑本地测试，我再帮你 review 和讨论优化。全程不需要访问 LeetCode 官网。

## 目录结构

```
leetcode/
  handoff.md         # 个人疑难点与进度交接
  notes/             # 每道题的复盘笔记
  problems/
    001-two-sum/
      problem.md       # 题目描述、约束、示例
      solution.<ext>   # 你的解法
      test.<ext>       # 本地测试用例（我出或你写）
      notes.md         # 思路、复杂度、复盘（可选）
```

每道题一个子目录，命名建议 `题号-题名`。

## 练习流程

1. 我给出题目描述、输入输出约束和示例用例
2. 你编写 `solution.<ext>`，先自己想思路再动手
3. 运行本地测试验证（`test.<ext>`），包括我给的示例和边界用例
4. 我把结果和你的代码做 review，讨论时间/空间复杂度与优化方向

## 运行测试（以 73 题为例）

```powershell
cd C:\Users\BHJ4SZH\Desktop\Study\leetcode\problems\0073-set-matrix-zeroes
g++ -std=c++17 test.cpp -o test.exe
.\test.exe
```

> 如果提示 `g++ 不是可识别的命令`：说明当前终端是在安装编译器之前打开的。
> 彻底关闭并重开终端（VS Code 需要完全退出重开），或者先在当前终端里执行：
>
> ```powershell
> $env:Path = [Environment]::GetEnvironmentVariable('Path','User') + ';' + [Environment]::GetEnvironmentVariable('Path','Machine')
> ```

## 约定

- 只写你自己的解法，不要先查答案；卡住超过 20 分钟可以向我要提示
- 每题完成后在 `notes.md` 里记录思路和复杂度，方便回头复习
- 不确定语言或题目难度时直接问我
