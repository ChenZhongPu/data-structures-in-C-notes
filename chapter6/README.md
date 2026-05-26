# 第6章：数组与广义表

>[!NOTE]
> 这里仅讨论普通数组。

参考维基百科的定义，

> In computer science, an array is a data structure consisting of a collection of elements (values or variables), of the same memory size.

> An array is stored such that the position (memory address) of each element can be computed from its index tuple by a mathematical formula.

它的重点是每个元素占用相同的内存空间（代码中要求是相同数据类型），并且可以通过索引计算出元素的位置。换言之，它们在内存是连续的（**contiguous**）。因此，数组的访问效率很高，可以在常数时间内访问任意元素。

```c
int arr[5] = {10, 20, 30, 40, 50};
for (int i = 0; i < 4; i++) {
    char *addr1 = (char *)&arr[i];
    char *addr2 = (char *)&arr[i + 1];
    long byte_diff = addr2 - addr1;
    printf("地址差值: &arr[%d] - &arr[%d] = %p - %p = %ld 字节 (sizeof(int) = %zu)\n", i + 1, i, (void*)addr2, (void*)addr1, byte_diff, sizeof(int));
}
```

再比如，考虑一个数组A，如果A[0]的地址是42，每个元素占用2个字节，那么A[10]的地址就是42 + 10 * 2 = 62。

然而，如果是高维数组（下面以二维为例），事情变得复杂。首先要明确，从计算机的视角，二维数组也是连续的内存空间。比如，定义一个二维数组：

```c
int arr[3][4] = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12}
};
```

- 如果按行优先（row-major order）存储，那么内存中的元素顺序是：1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12。
- 如果按列优先（column-major order）存储，那么内存中的元素顺序是：1, 5, 9, 2, 6, 10, 3, 7, 11, 4, 8, 12。

两种方式都是合理的，但是在目前主流的编程语言中，基本都是采用行优先的方式存储。下面是一次运行结果：

```
元素位置 数值 内存地址       相对于首地址的偏移
--------------------------------------------------
arr[0][0]     1      0x7ffdea30eec0   +0 字节
arr[0][1]     2      0x7ffdea30eec4   +4 字节
arr[0][2]     3      0x7ffdea30eec8   +8 字节
arr[0][3]     4      0x7ffdea30eecc   +12 字节
--- 行 arr[0] 结束 ---
arr[1][0]     5      0x7ffdea30eed0   +16 字节
arr[1][1]     6      0x7ffdea30eed4   +20 字节
arr[1][2]     7      0x7ffdea30eed8   +24 字节
arr[1][3]     8      0x7ffdea30eedc   +28 字节
--- 行 arr[1] 结束 ---
arr[2][0]     9      0x7ffdea30eee0   +32 字节
arr[2][1]     10     0x7ffdea30eee4   +36 字节
arr[2][2]     11     0x7ffdea30eee8   +40 字节
arr[2][3]     12     0x7ffdea30eeec   +44 字节
--- 行 arr[2] 结束 ---
```

### 练习题1

考虑二维数组A[3][4]，采用列优先存储方式。
如果A[0][0]的地址是1000，每个元素占用1个存储单元，那么A[2][1]的地址是________。

### 练习题2（2021年考研）

已知二维数组 A 按行优先方式存储，每个元素占用 1 个存储单元。若元素 A[0][0] 的存储地址是 100, A[3][3] 的存储地址是 220 , 则元素 A[5][5] 的存储地址是（ ）。

