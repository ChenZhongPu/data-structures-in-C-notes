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

## 1.3
在1.1.3的部分，对于哈希存储结构的知识点有补充：

这里没有提到哈希存储的缺点：这是典型的空间换取时间的结构，为了查找较快，会预留较多的存储空间。

同时，哈希冲突----即不用的关键字有可能会算出相同的存储地址，导致存储混乱。解决方案如下：

1、开放地址法
2、链地址法

参考：<ins>[点击这里跳转到我的仓库](https://github.com/porkyummy/data-structures-in-C-notes)</ins>


## 1.5
在1.1.5中静态存储空间部分，概念有些混淆。

原文：

“一旦遇到该语句，系统就为 a 数组分配 10 个 int 整型空间。无论程序是否向 a 中放入元素，这一片空间都被占用。它也是用于自动变量，当超出其作用范围时系统自动释放其内存空间。”

1、前面在讲静态存储分配，举的例子是 int a[10];（局部数组），但局部数组实际上是自动存储期，在栈上分配，属于动态分配（运行时分配），而不是“静态存储空间分配”。

2、 真正的静态存储分配指的是：全局变量、static 关键字修饰的局部变量，它们在编译时确定地址，在整个程序运行期间都存在。

而原文把“静态存储空间分配”和“自动变量”混在一起说，逻辑矛盾。


