# 第7章：树和二叉树

> [!CAUTION]
> 在国内主流教材中（如《Algorithms》、《Introduction to Algorithms》），都是直接讨论二叉搜索树和平衡树（AVL树、红黑树），即教材第9章的内容；不像国内教材那样花大量篇幅介绍树的基本概念和性质。客观来说，教材中的内容存在的价值只是因为“考试”（如考研408）会考，对实际编程并没有太大帮助。

> [!TIP]
> 从编程能力的角度出发，本章仅二叉树的遍历、哈夫曼树有用。但从考试角度，每部分都重要。

## 7.1

树的最核心特征是**hierarchical**（层次的）。

![animal](animal.webp)

树可以使用递归定义：树T要么是空树，要么是由一个根节点（root）和若干子树（subtrees）组成的。

> A tree `T` is either empty or consists of a node `r`, called the **root** of `T`, and a (possibly empty) set of subtrees whose roots are the children of `r`.

对照链表的递归定义：

> A linked list is a recursive data structure that is either empty (null) or a reference to a node having an item and a reference to a linked list.

![tree](tree.png)

> A path is a sequence of nodes with the property that each node in the sequence is adjacent to the node linked to it. A path that does not repeat nodes is called a **simple path**.

按教材的说法，树中结点的最大层次被称为树的**高度**（height）或树的**深度**（depth），两者是同义词。然而，在经典数据结构的书籍（如《算法导论》）中，高度的定义有所差异： **高度**（height）是从叶子节点到根节点的最长路径的边数（空树高度是-1）；以上面的树为例，教材中的说法是高度为5，而经典定义的高度为4。

>[!TIP]
> 为了考试角度的一致性，后续我们将使用教材中的定义，即树的高度等于树中结点的最大层次。


关于树的性质，可以进一步总结出：**n个结点，一定有n-1条边。**

关于“孩子兄弟链式存储结构”，在经典教材中被称为“left-child, right-sibling representation”。

## 7.2

教材中对二叉树（binary tree）的定义非常不清晰。更简单的定义是：

> A binary tree is a tree in which each node has at most two children, which are referred to as the left child and the right child.

关于二叉树与树、森林的转换是考试的重点内容。但从工程出发，它们本质还是理解树的*left-child, right-sibling representation*。

## 7.3

关于二叉树的顺序存储（即底层使用数组），最常见的做法是让root结点存在index为1的位置，左子树和右子树分别存在2i和2i+1的位置；对于i位置的结点，其父节点在 $\lfloor i/2 \rfloor$ 位置。

如果非要使用0-based indexing，那么root结点存在index为0的位置，左子树和右子树分别存在2i+1和2i+2的位置；对于i位置的结点，其父节点在 $\lfloor (i-1)/2 \rfloor$ 位置。

教材中把二叉树结点取名为`BTNode`，保存在头文件`btree.h`中。由于BTree本身是另外一种数据结构，为了避免混淆，建议把二叉树结点的结构体命名为`BinaryTreeNode`，并保存在头文件`binary_tree.h`中。

## 7.4

本小节的代码比较简单，主要是通过递归的方式进行二叉树的基本操作。唯一复杂的代码（`CreateBTree`）仅用于示例，实际编程中不太可能用到。

另一方面，该代码也能通过递归实现，从而不直接使用栈：

```c
// 辅助函数：递归解析，index 记录当前解析位置
Node* createTreeHelper(char *str, int *index) {
    if (str == NULL || str[*index] == '\0') return NULL;

    Node *node = createNode(str[*index]);
    (*index)++;

    if (str[*index] == '(') {
        (*index)++;  // 跳过 '('

        // 左子树：若当前是 ','，说明左子树为空
        if (str[*index] != ',') {
            node->left = createTreeHelper(str, index);
        }

        // 右子树：若有 ','，跳过后继续判断
        if (str[*index] == ',') {
            (*index)++;  // 跳过 ','
            if (str[*index] != ')') {
                node->right = createTreeHelper(str, index);
            }
        }

        (*index)++;  // 跳过 ')'
    }

    return node;
}

// 对外接口保持不变
Node* createTree(char *str) {
    int index = 0;
    return createTreeHelper(str, &index);
}
```

```
createTreeHelper("A(B,C)", &0)
  ├─ 创建 A，index=1
  ├─ 遇到 '('，index=2
  ├─ 递归左子树：
  │    createTreeHelper(..., &2)
  │      ├─ 创建 B，index=3
  │      └─ 遇到 ','，不是 '('，直接返回 B
  ├─ 遇到 ','，index=4
  ├─ 递归右子树：
  │    createTreeHelper(..., &4)
  │      ├─ 创建 C，index=5
  │      └─ 遇到 ')'，不是 '('，直接返回 C
  ├─ 遇到 ')'，index=6
  └─ 返回 A（A->left=B, A->right=C）
```

## 证明上面算法的正确性

（递归算法的正确性证明通常使用**数学归纳法**）

下面对树的高度进行归纳。

证明：对于任意合法的括号表示字符串`s`，`createTreeHelper(s, index)` 能正确构建对应的二叉树，且调用结束后 `*index` 恰好指向该子树字符串的末尾后一位。

- 当高度为0时，字符串为空，算法正确。
- 当高度为1时，字符串仅包含一个字符（根节点），算法正确。
- 假设对于高度不超过h的树，算法正确。那么对于高度为h的树，形如

$$
s = \underbrace{N}_{\text{根}} \underbrace{(}_{\text{ }} \underbrace{L}_{\text{左子树}} \underbrace{,}_{\text{ }} \underbrace{R}_{\text{右子树}} \underbrace{)}_{\text{ }}
$$

读者也可以证明其正确性。

## 7.5

二叉搜索树可视化：https://bst.zhongpu.info/

理解递归算法的一种技巧是：理解其“递归不变式”（递归不变式是指在递归过程中始终保持为真的条件）。

考虑计算结点深度的算法（教材例7.13）:

```c
int levelOfNode(Node *root, char x, int level) {
  if (root == NULL) {
    return 0;
  }
  if (root->data == x) {
    return level;
  }

  int leftLevel = levelOfNode(root->left, x, level + 1);
  if (leftLevel != 0) {
    return leftLevel;
  }

  return levelOfNode(root->right, x, level + 1);
}
```

上面函数的核心是：  
- 如果以 root 为根的子树中存在值为 x 的结点，就返回它在整棵树中的层次
- 如果不存在，就返回 0

在这个断言成立的前提下，初始调用`levelOfNode(root, x, 1)`可以正确地计算出结点 x 在树中的深度。进一步，我们可以通过数学归纳法证明其正确性。

很多时候，更简单的递归算法依赖对树的结构和性质的理解。考虑教材例7.14，求二叉树中第k层的结点个数。一个更简单的算法是：

```c
int countLevelK(Node *root, int k) {
  if (root == NULL || k < 1) {
    return 0;
  }
  if (k == 1) {
    return 1;
  }
  return countLevelK(root->left, k - 1) + countLevelK(root->right, k - 1);
}
```

7.5.4节提到，“因此层次遍历算法采用一个环形队列qu来实现”。这可能对读者造成误解：*难道不能用普通队列？* 实际上，树的层次遍历就是一个广度优先搜索（**BFS**）过程，重点是FIFO，使用任意队列都可以；教材中的环形队列只是一个具体的实现。比如仓库代码`queue.h`是个通用的队列实现：

```c
void level_order(TreeNode *root) {
  if (root == NULL) {
    return;
  }

  Queue *q = queue_init();
  queue_enqueue(q, root);

  void *value = NULL;
  while (queue_deq(q, &value)) {
    TreeNode *node = (TreeNode *)value;
    printf("%c ", node->data);

    if (node->left != NULL) {
      queue_enqueue(q, node->left);
    }
    if (node->right != NULL) {
      queue_enqueue(q, node->right);
    }
  }

  queue_destroy(q);
}
```

特别地，之前的数据结构中，由于C语言的限制，我们一般必须限制其元素的类型。其他编程语言中（如Rust/C++/Java）提供泛型机制，可以更灵活地处理不同类型的元素，而在C语言中可以使用`void *`实现类似的功能。因此，这里使用的队列（底层用链表实现）能够存储任意类型的元素，包括树结点的指针。

```c
struct QueueNode {
  void *data;
  struct QueueNode *next;
};
```

7.7节的线索二叉树的目的是为了在不使用栈或递归的情况下实现二叉树的遍历。这种方法在某些特定场景下可以提高遍历效率，尤其是在内存受限的环境中。具体的方法不需要刻意记忆，理解三种遍历（前序、中序、后序）的访问顺序，并结合线索二叉树的结构，就能推导出相应的遍历算法。

7.8节的哈夫曼树是考试的重点。算法本身比较简单，核心是每次选择权重最小的两个结点合并。这里就有一个问题：**如何高效地选择权重最小的两个结点？** 书中是直接比较，每次选择的算法复杂度是O(N)，更好的方法是使用一个优先级队列（最小堆）来存储当前的结点，这样每次选择权重最小的两个结点的时间复杂度可以降到O(log N)。仓库给出的示例代码为了简单，是直接排序得到。

教材中提到的是**带权路径长度**（WPL），也称加权路径长度。

$$
WPL = \sum_{i=1}^{n} w_i \cdot l_i
$$
其中， $w_i$ 是第i个叶子结点的权重， $l_i$ 是从根节点到第i个叶子结点的路径长度（边数）。

类似的，还有**加权平均长度**：

$$
\frac{WPL}{\sum_{i=1}^{n} w_i}
$$

在编码场景，频次或频率就是权重，假设频率 $p_i = \frac{w_i}{\sum_{j=1}^{n} w_j}$ ，则加权平均长度可以表示为：

$$
\sum_{i=1}^{n} p_i \cdot l_i
$$