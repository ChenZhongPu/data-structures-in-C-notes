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

1.1.5提到“自动变量（automatic variable）”的概念，作为对*duration*补充，读者需要知道：

- Automatic: exists only within blocks
- Static: exist for the entire duration of the program
- Thread: objects exist so long as their thread does

显然，在教材的上下文中，`int n = 10`不一定是一个自动变量。

在1.1.5部分，提到`int`占`4`字节。但这不一定正确，它取决于平台和编译器。比如在某些系统上，`int`可能占2字节；更可靠的做法是使用`sizeof(int)`来获取`int`类型的实际大小。

> X86 32系统上int占4字节，64位系统上int仍然占4字节；最低是2字节。

> short int总是2字节。

## 1.2

在教材PPT关于程序计时,传统的计时代码`clock()`(参考[C 语言中的 time 函数总结](https://www.runoob.com/w3cnote/c-time-func-summary.html))由于来自C语言标准库导致其高度依赖操作系统,其中`CLOCKS_PER_SEC`为实现跨平台而牺牲统一的转换系数,本身精度不足导致`Windows`上大量出现`error(0.000000)`错误,统一的`clock()`也带来跨平台上对时间测量的潜在差异(`Windows`:`wall-clock time`,`MacOS`和`Linux`:`CPU time`),这点尤其体现在面对频繁I/O流、用户输入等操作中。

因此,普遍情况下,应调用基于不同平台的高精度API(`Windows`:`QueryPerformanceCounter()`,`Linux`:`clock_gettime(CLOCK_MONOTONIC)``MacOS`:`mach_absolute_time()`)以统一记录`Wall-clock time`并提前`Warm-up`抛开初始化或首次执行时的异常开销,以实现最佳实践。

目前给出`Windows`和`MacOS`版本:
```c
#include <stdio.h>
#ifdef _WIN32
    #include <windows.h>
    typedef LARGE_INTEGER timer;
    void timer_start(timer *t){
        QueryPerformanceCounter(t);}
    double timer_elapsed(timer *t){
        LARGE_INTEGER end, freq;
        QueryPerformanceCounter(&end);
        QueryPerformanceFrequency(&freq);
        return (double)(end.QuadPart-t->QuadPart)/freq.QuadPart;
    }
#elif defined(__APPLE__)
    #include <mach/mach_time.h>
    typedef struct { uint64_t start; } timer;
    void timer_start(timer *t){
        t->start = mach_absolute_time();}
    double timer_elapsed(timer *t) {
        uint64_t end = mach_absolute_time();
        mach_timebase_info_data_t info;
        mach_timebase_info(&info);
        uint64_t nanos=(end-t->start)*info.numer/info.denom;
        return (double)nanos/1e9;
    }
#endif

int a[60][10000];

void foo() {
    for (int i = 0; i < 60; i++)
        for (int j = 0; j < 10000; j++)
            a[i][j] = 0;
}

void bar() {
    for (int j = 0; j < 10000; j++)
        for (int i = 0; i < 60; i++)
            a[i][j] = 0;
}

int main() {
    timer t;
    double tf, tb;

    foo(); 
    bar();

    
    timer_start(&t); 
    foo(); 
    tf = timer_elapsed(&t);
        
    timer_start(&t); 
    bar(); 
    tb = timer_elapsed(&t);

        printf("foo takes:%.6f\tbar takes:%.6f\tratio:%.6f\n", tf, tb, tf/tb);
    
    return 0;
}

```

此外,值得注意的是,高精度`API`尤其受CPU线程切换、频率波动、缓存的后精准命中等因素影响导致计时偶尔大幅偏离"预期",因此在衡量"快""慢"问题上常常以观察倍数的统计分布作为参考依据,事实上,绝大多数情况下,我们更多关心于"快""慢"本身。

## 1.3

在1.3.2部分，教材提到“一个问题只能用指数时间复杂度的算法求解，称为NP问题”。这是一个明显误解。

- P类问题：可以在多项式时间内求解的问题。例如排序、最短路径等。
- NP类问题：可以在多项式时间内验证答案的问题。例如给定一个解，能快速验证它是否正确。

换言之，\[P \subset NP\]。参考[P versus NP Problem](https://en.wikipedia.org/wiki/P_versus_NP_problem)。
