# 第7章：图

> [!TIP]
> 无论是中文的“图”，还是英文的“graph”，这个词都是多义的。在数据结构中，我们通常指的是**图论中的图**，而不是函数图像、统计图表等其他类型的图。

> [!CAUTION]
> 从难度角度出发，图是最复杂的数据结构，没有之一。参考经典的《算法导论》，其第三部分是“Data Structures”，第无部分是“Advanced Data Structures”，而整体第六部分都是“Graph Algorithms”。因此，图的算法和数据结构是非常复杂的，需要花费大量时间来理解和掌握。


从8.1节可以看出，图和树比较类似；或者可以说，树是图的一种特殊情况。但图由于其更一般性，其术语也更复杂。从应用角度，教材未充分讨论图的应用场景，这里补充一些常见的图应用（https://ml-digest.com/knowledge-graphs/）：

- **社交网络**：用户之间的关系可以表示为图，节点代表用户，边代表好友关系。
- **交通网络**：城市之间的道路可以表示为图，节点代表城市，
- **函数调用关系**：在程序中，函数之间的调用关系可以表示为图，节点代表函数，边代表调用关系。
- **网页链接**：网页之间的链接关系可以表示为图，节点代表网页，边代表链接关系。

关于边（edge）的种类，实际上有两种常见的异常（anomaly）：

- **自环**（self-loop）：边连接同一个节点，例如A->A。
- **平行边**（parallel）：两个节点之间存在多条边，例如A->B和A->B。

![anomaly](anomalies.png)

结合下面无向图的示例，理解图的常用术语：

![anatomy](anatomy1.png)

一般情况下，我们都是考虑simple path、simple cycle等，从而在描述的时候经常省略“simple”这个词。

教材中有个概念很模糊，就是**稠密图**（dense graph）和**稀疏图**（sparse graph）。真实情况就是，它们本来就没有严格定义：if $|E|$ is much less than $|V|^2$, then it is sparse；书中的经验范围 $O(n\log{n})$ 也是正确的。

还有一个术语经常出现，就是**acyclic**，即无环的。特别地，如果是有向图无环，我们称之为**DAG**（Directed Acyclic Graph）。DAG在很多应用中非常重要，例如任务调度、版本控制等。

因此，**A tree is an acyclic connected graph**，即树是一个无环连通图。比如说，当下面任意条件成立的时候，图就是树：

- 图是无环的，并且有n-1条边。
- 图是连通的，并且有n-1条边。

连通分量（connected component）是一个难点。它有多种定义方式（建议参考[词典解释](https://dictionary.cambridge.org/zht/%E8%A9%9E%E5%85%B8/%E8%8B%B1%E8%AA%9E-%E6%BC%A2%E8%AA%9E-%E7%B9%81%E9%AB%94/component)）：

> A component of an undirected graph is a connected subgraph that is not part of any larger connected subgraph. 

> A graph is connected if there is a path from every vertex to every other
vertex in the graph. A graph that is not connected consists of a set of connected components, which are maximal connected subgraphs.

图的表示方法很重要，主要有两种：**邻接矩阵**（adjacency matrix）和**邻接表**（adjacency list）。前者适用于稠密图，后者适用于稀疏图。而现实中大多数图都是稀疏的，因此邻接表更常用。

![representation](graph-representation.png)

对于无向图，所有邻接表的长度之和等于$2|E|$；对于有向图，所有邻接表的长度之和等于$|E|$。因此，邻接表的空间复杂度是$O(|V|+|E|)$，而邻接矩阵的空间复杂度是$O(|V|^2)$。

不难发现，邻接表的实现很简单，即每个节点对应一个列表，列表中存储该节点的邻居节点。这个列表的具体实现有多种方式，可以是链表、数组，甚至是哈希表、红黑树等，具体选择取决于应用场景和性能需求。下面通过链表的方式举例：

```c
// 邻接表中每个元素表示一个边，包含一个邻居节点ID和指向下一个边的指针
typedef struct Edge {
  int vertex;
  struct Edge *next;
} Edge;

// 邻接表本身通过首节点表示，size可选
typedef struct {
  Edge *head;
  int size;
} List;

typedef struct {
  int V;
  int E;
  List *adj; // 邻接表数组，adj[i]表示节点i的邻接表
} Graph;
```

这样添加边的时候就是在对应链表上执行头插法，比教材中晦涩的代码更好理解。