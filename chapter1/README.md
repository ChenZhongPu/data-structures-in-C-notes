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

如果使用UTF-8编码，一个汉字一般需要三个字节，加上最后一个`\0`，那么`sex`的大小就需要至少为4。合理的做法：

- 使用枚举
- 更大空间的数组
- 使用`wchar_t`

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

1.1.5提到“自动变量（automatic variable）”的概念，作为对*duration*（）补充，读者需要知道：

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
  foo ();
  end = clock ();
  printf ("bar takes %f\n", (double)(end - start) / CLOCKS_PER_SEC);
}
```

## 1.3

在1.3.2部分，教材提到“一个问题只能用指数时间复杂度的算法求解，称为NP问题”。这是一个明显误解。

- P类问题：可以在多项式时间内求解的问题。例如排序、最短路径等。
- NP类问题：可以在多项式时间内验证答案的问题。例如给定一个解，能快速验证它是否正确。

换言之，$P \subset NP$。参考[P versus NP Problem](https://en.wikipedia.org/wiki/P_versus_NP_problem)。

##2.3.1.2 随机存储特性的补充说明：

随机存储也叫直接访问，是指存取数据的时间与数据所在位置无关，不会因为位置"偏远"而变慢。前文已经提及了直接映射，那为何不选用直接访问这个更严谨，更直观的教学术语

更详细的理由：
1，含义准确：“直接访问”强调“直接定位到目标位置”的能力，突出逻辑上的可跳过性，完美对应数据结构（如数组）通过下标 O(1) 访问任意元素的特性。

2，避免混淆：“随机”容易误导初学者，以为是指“随机地”（randomly）访问某个位置，而实际意思是“任意”（arbitrary）访问，即“想访问哪里就访问哪里，不分顺序”。

如果是与计算机组成原理等硬件相关，“随机存取存储器”（RAM，即运行内存）是标准译法，强调存取时间与物理位置无关的硬件特性。可教材是讲解数据结构，所以我觉得这里的随机存储特性使用的不是特别好。

总结：在数据结构与算法领域，推荐用“直接访问”；若需提及硬件原理，可用“随机存取”，但最好补充说明这里的“随机”是“任意”的意思。
