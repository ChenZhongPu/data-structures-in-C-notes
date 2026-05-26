#include <stdio.h>

int main() {
    // 1. 定义一个整型数组
    int arr[5] = {10, 20, 30, 40, 50};
    
    printf("===== 数组内存地址连续性演示 =====\n\n");
    
    // 2. 打印每个元素的索引、值和对应的内存地址
    printf("1. 打印数组中每个元素的地址：\n");
    printf("%-8s %-8s %-18s\n", "元素", "数值", "内存地址");
    printf("---------------------------------------------\n");
    for (int i = 0; i < 5; i++) {
        // &arr[i] 获取第 i 个元素的地址
        // %p 是打印指针的格式化字符
        printf("arr[%d]   %-8d %p\n", i, arr[i], (void*)&arr[i]);
    }
    printf("---------------------------------------------\n\n");
    
    // 3. 计算并展示相邻元素地址之间的字节差
    printf("2. 计算相邻元素地址之间的差值（以字节为单位）：\n");
    for (int i = 0; i < 4; i++) {
        // 指针相减在 C 语言中默认返回的是元素个数差，而不是字节差。
        // 为了获取实际的字节差，我们可以将地址转换为字符指针 (char*)，因为 char 占 1 字节。
        char *addr1 = (char *)&arr[i];
        char *addr2 = (char *)&arr[i + 1];
        long byte_diff = addr2 - addr1;
        
        printf("地址差值: &arr[%d] - &arr[%d] = %p - %p = %ld 字节 (sizeof(int) = %zu)\n",
               i + 1, i, (void*)addr2, (void*)addr1, byte_diff, sizeof(int));
    }
    
    printf("\n结论：\n");
    printf("每个 int 类型的元素占用 %zu 个字节。\n", sizeof(int));
    printf("相邻元素的内存地址差值正好是 %zu 字节，说明数组在内存中是【连续存储】的。\n", sizeof(int));
    
    return 0;
}
