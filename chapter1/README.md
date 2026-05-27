# 第1章：绪论

## 1.1

整个1.1章节完全抽象过度，尤其以`二元组表示`最为明显。据我所知，主流国外教材完全没有涉及这些晦涩难懂的概念，对实际编程/思考毫无用处。

在1.1.3部分的代码有个小错误：

```c
struct {
  int no;
  char name[8];
  char sex[2];
  char class[4];
} Stud[7] = {{1, "张斌", "男", "9901"}, ... }};
```

如果使用UTF-8编码，一个汉字一般需要三个字节，加上最后一个`\0`，那么`sex`的大小就需要至少为4。合理的做法是：

- 使用枚举
- 更大空间的数组
- 使用`wchar_t`

P.17 的一元二次方程求根示例也有边界问题。函数 `solution(..., double &x1, double &x2)` 在判别式 `d==0` 的分支中只给 `x1` 赋值并返回1。如果调用者仍然读取 `x2`，会读到未初始化值。更合理的写法是重根时同时设置两个返回值：

```c
x1 = x2 = -b / (2 * a);
```

此外，该示例还应该先判断 `a` 是否为0，否则会出现除以0；对于浮点数判别式，也不建议直接使用 `d==0`，而应该使用误差范围判断。

此外，1.1.3部分对存储结构类型的划分稍显混乱，一般很少见到“索引存储结构”的提法。

本书对于指针使用的风格（比如 `struct Studnode * next`）不符合C语言的习惯（应该是出于印刷排版的考虑），但在真实编程中，**Code Style Matters**、**Name also Matters**。参考[命名约定](https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/naming.html)。现代C/C++代码一般用`clang`自动格式化，可选的风格包括：LLVM、Google和GNU等。

```c
typedef struct StudNode
{
  int no;
  char name[8];
  char sex[2];
  char class[4];
  struct StudNode *next;
} StudType;
```

1.1.5提到“自动变量（automatic variable）”的概念，作为对*duration*补充，读者需要知道：

- Automatic: exists only within blocks
- Static: exist for the entire duration of the program
- Thread: objects exist so long as their thread does

显然，在教材的上下文中，`int n = 10`不一定是一个自动变量。

在1.1.5部分，提到`int`占`4`字节。但这不一定正确，它取决于平台和编译器。比如在某些系统上，`int`可能占2字节；更可靠的做法是使用`sizeof(int)`来获取`int`类型的实际大小。

> X86 32系统上int占4字节，64位系统上int仍然占4字节；最低是2字节。

> short int总是2字节。

## 1.2

在教材PPT关于程序计时，这里提供计时代码，参考[C 语言中的 time 函数总结](https://www.runoob.com/w3cnote/c-time-func-summary.html)

```c
#include <stdio.h>
#include <time.h>

int a[60][10000];

void
foo ()
{
  for (int i = 0; i < 60; i++)
    for (int j = 0; j < 10000; j++)
      a[i][j] = 0;
}

void
bar ()
{
  for (int j = 0; j < 10000; j++)
    for (int i = 0; i < 60; i++)
      a[i][j] = 0;
}

int
main ()
{
  clock_t start, end;

  start = clock ();
  foo ();
  end = clock ();
  printf ("foo takes %f\n", (double)(end - start) / CLOCKS_PER_SEC);

  start = clock ();
  bar ();
  end = clock ();
  printf ("bar takes %f\n", (double)(end - start) / CLOCKS_PER_SEC);
}
```

## 1.3

在1.3.2部分，教材提到“一个问题只能用指数时间复杂度的算法求解，称为NP问题”。这是一个明显误解。

- P类问题：可以在多项式时间内求解的问题。例如排序、最短路径等。
- NP类问题：可以在多项式时间内验证答案的问题。例如给定一个解，能快速验证它是否正确。

换言之，$P \subseteq NP$。参考[P versus NP Problem](https://en.wikipedia.org/wiki/P_versus_NP_problem)。
