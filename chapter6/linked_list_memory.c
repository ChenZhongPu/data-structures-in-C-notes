#include <stdio.h>
#include <stdlib.h>

// 定义链表节点结构
typedef struct Node {
    int data;
    struct Node *next;
} Node;

int main() {
    // 1. 动态创建 5 个节点（模拟链表的构建过程）
    // 注意：每个节点都是独立 malloc 的，地址由操作系统决定
    Node *head = NULL;

    printf("===== 链表内存非连续性演示 =====\n\n");

    // 依次创建节点并链接
    // 技巧：在每次分配节点之间，分配一块较大的"干扰"内存并保持不释放，
    // 迫使后续 malloc 从不同区域取内存，从而使节点地址分散
    int values[] = {10, 20, 30, 40, 50};
    Node *nodes[5];
    void *junk[4];  // 用于存放干扰分配

    for (int i = 0; i < 5; i++) {
        if (i > 0) {
            // 分配一块较大的干扰内存（不释放），迫使下一个节点分配到不同位置
            junk[i - 1] = malloc(128 + i * 64);
        }
        nodes[i] = (Node *)malloc(sizeof(Node));
        nodes[i]->data = values[i];
        nodes[i]->next = NULL;
    }
    // 释放干扰内存
    for (int i = 0; i < 4; i++) {
        free(junk[i]);
    }

    // 将节点串联成链表: nodes[0] -> nodes[1] -> ... -> nodes[4] -> NULL
    for (int i = 0; i < 4; i++) {
        nodes[i]->next = nodes[i + 1];
    }
    head = nodes[0];

    // 2. 打印每个节点的地址、数据和 next 指针
    printf("1. 打印链表中每个节点的信息：\n");
    printf("%-8s %-18s %-8s %-18s\n", "节点", "节点地址", "数据", "next 指针");
    printf("---------------------------------------------------------------\n");

    Node *cur = head;
    int idx = 0;
    while (cur != NULL) {
        printf("node[%d]  %-18p %-8d %-18p\n",
               idx, (void *)cur, cur->data, (void *)cur->next);
        cur = cur->next;
        idx++;
    }
    printf("---------------------------------------------------------------\n\n");

    // 3. 计算相邻节点地址之间的字节差
    printf("2. 计算相邻节点地址之间的差值（以字节为单位）：\n");
    for (int i = 0; i < 4; i++) {
        long addr_diff = (char *)nodes[i + 1] - (char *)nodes[i];
        // 取绝对值，因为 malloc 不保证分配顺序
        long abs_diff = addr_diff > 0 ? addr_diff : -addr_diff;

        printf("地址差值: node[%d] - node[%d] = %p - %p = %ld 字节",
               i + 1, i, (void *)nodes[i + 1], (void *)nodes[i], addr_diff);
        // 判断是否连续（相差恰好一个节点大小）
        if (abs_diff == sizeof(Node)) {
            printf("  [连续]\n");
        } else {
            printf("  [非连续]\n");
        }
    }

    printf("\n结论：\n");
    printf("每个 Node 占用 %zu 字节 (data: %zu, next: %zu, 可能有填充字节)。\n",
           sizeof(Node), sizeof(int), sizeof(Node *));
    printf("malloc 分配的地址由操作系统决定，相邻节点的地址差通常不等于 sizeof(Node)。\n");
    printf("因此链表在内存中是【非连续存储】的。\n\n");

    // 4. 解释 next 指针的作用
    printf("3. next 指针的含义：\n");
    printf("---------------------------------------------------------------\n");
    cur = head;
    idx = 0;
    while (cur->next != NULL) {
        printf("node[%d] (地址 %p, 数据 %d) 的 next 指向 node[%d] (地址 %p, 数据 %d)\n",
               idx, (void *)cur, cur->data,
               idx + 1, (void *)cur->next, cur->next->data);
        printf("  => 通过 next 指针，从当前节点可以找到下一个节点，无论它在内存的哪个位置\n\n");
        cur = cur->next;
        idx++;
    }
    printf("node[%d] (地址 %p, 数据 %d) 的 next = NULL (链表结束)\n",
           idx, (void *)cur, cur->data);

    printf("\n---------------------------------------------------------------\n");
    printf("总结：\n");
    printf("- 数组：元素在内存中连续排列，通过下标直接计算地址（O(1) 随机访问）\n");
    printf("- 链表：节点在内存中分散存储，通过 next 指针串联（只能顺序访问）\n");
    printf("- next 指针就是「桥梁」，让内存中不相邻的节点在逻辑上形成一条链\n");

    // 释放内存
    for (int i = 0; i < 5; i++) {
        free(nodes[i]);
    }

    return 0;
}
