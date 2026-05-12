# 第7章：图

> [!TIP]
> 无论是中文的“图”，还是英文的“graph”，这个词都是多义的。在数据结构中，我们通常指的是**图论中的图**，而不是函数图像、统计图表等其他类型的图。

> [!CAUTION]
> 从难度角度出发，图是最复杂的数据结构，没有之一。参考经典的《算法导论》，其第三部分是“Data Structures”，第无部分是“Advanced Data Structures”，而整体第六部分都是“Graph Algorithms”。因此，图的算法和数据结构是非常复杂的，需要花费大量时间来理解和掌握。

## 8.1

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

## 8.2

图的表示方法很重要，主要有两种：**邻接矩阵**（adjacency matrix）和**邻接表**（adjacency list，有时直接称为邻接列表）。前者适用于稠密图，后者适用于稀疏图。而现实中大多数图都是稀疏的，因此邻接表更常用。

![representation](graph-representation.png)

对于无向图，所有邻接表的长度之和等于$2|E|$；对于有向图，所有邻接表的长度之和等于$|E|$。因此，邻接表的空间复杂度是$O(|V|+|E|)$，而邻接矩阵的空间复杂度是$O(|V|^2)$。

不难发现，邻接表的实现很简单，即每个节点对应一个列表，列表中存储该节点的邻居节点。这个列表的具体实现有多种方式，可以是链表、数组，甚至是哈希表、红黑树等，具体选择取决于应用场景和性能需求。当然，很多时候我们都假定邻接表的“表”就是*linked list*。下面通过链表的方式举例：

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

如果需要记录权重，可以在 `Edge` 结构体中添加一个权重字段，其他代码几乎不变。

```c
typedef struct Edge {
  int vertex;
  double weight;
  struct Edge *next;
} Edge;
```


| 数据结构 | 空间复杂度 | 添加边的时间复杂度 | 检查 w 是否与 v 相邻 |
|---|---|---|---|
| 邻接矩阵 | $O(\lvert V\rvert^2)$ | $O(1)$ | $O(1)$ |
| 邻接表 | $O(\lvert V\rvert + \lvert E\rvert)$ | $O(1)$ | $O(\deg(v))$ |

## 8.3
图的搜索和遍历是图算法的基础，是本章的重点。对图搜索的常见应用包括：**路径搜索**（寻找图中两点之间的路径）。

> [“凯文·贝肯六度法则”](https://en.wikipedia.org/wiki/Six_Degrees_of_Kevin_Bacon)或称“贝肯定律”，是一种社交游戏：玩家轮流选择两位演员，通过他们共同出演过的电影来建立这两位演员之间的联系，不断重复这个过程，试图找到连接美国著名演员凯文·贝肯的最短路径。这个游戏的假设是——任何参与好莱坞电影行业的人，都可以通过他们在电影中的角色，在六步之内与凯文·贝肯建立联系。游戏名称借鉴了“六度分隔理论”，这一概念认为地球上任意两个人之间，最多只需通过六层人际关系就能建立联系。

在引入具体的算法之前，我们可以讨论**通用的图检索/遍历算法**。

问题定义：

>[!NOTE]
> 输入：无向图或有向图 $G=(V,E)$，起点 $s \in V$。
>
> 目标：$V$ 中所有与 $s$ 可达的顶点。

算法描述：

（最终的**完成状态**是：当且仅当一个顶点被标记为”已探索“/”已访问“，它才是从 $s$ 可达的顶点。）


```
将 s 标记为已访问，所有其他顶点标记为未访问
while 存在一条边 (u, w) ∈ E，且u已访问，w未访问：
    选择这样的边 (u, w) // 不够明确
    将 w 标记为已访问
```

![maze](maze-graph.png)

显然，在while循环的一次迭代中可能有多条边(u, w)可供选择，而广度优先搜索（BFS）和深度优先搜索（DFS）就是两种不同的具体选择策略。

### 深度优先搜索（DFS）

深度优先搜索（DFS）总是从最近发现的顶点开始向前搜索，直到无法继续（“走投无路”）为止，然后回退并继续搜索其他路径。DFS可以使用递归或显式的栈来实现。

![maze-adj](maze-adj.png)

递归版本的DFS实现（初始调用为`DFS(G, s)`）：

```
将 s 标记为已访问，所有其他顶点标记为未访问
while s邻接表的边(s, v)：
    if v未访问：
        递归调用DFS(G, v)
```

对应的C语言代码也很简单：

```c
void graph_dfs_recursive(const Graph *g, int v, bool *visited) {
  visited[v] = true;
  printf(" %d", v);

  int size;
  int *neighbors = adj(g, v, &size);
  for (int i = 0; i < size; i++) {
    int w = neighbors[i];
    if (!visited[w]) {
      graph_dfs_recursive(g, w, visited);
    }
  }

  free(neighbors);
}

void graph_dfs(const Graph *g, int start) {
  if (g == NULL || start < 0 || start >= g->V) {
    return;
  }
  // 申请 g->V 个 bool 类型的空间，并且全部初始化为 false： https://cppreference.com/c/memory/calloc
  bool *visited = calloc(g->V, sizeof(bool));

  printf("DFS from %d:", start);
  graph_dfs_recursive(g, start, visited);
  printf("\n");

  free(visited);
}
```

上述引入adj()是为了简化代码。为了提升程序效率，也可以类似教材中一样维护当前访问的邻接表的位置，从而避免每次都从头遍历邻接表（参考`graph_dfs2.c`）。

```c
void graph_dfs_recursive(const Graph *g, int v, bool *visited) {
  visited[v] = true;
  printf(" %d", v);

  Edge *curr = g->adj[v].head;
  while (curr != NULL) {
    int w = curr->vertex;
    if (!visited[w]) {
      graph_dfs_recursive(g, w, visited);
    }
    curr = curr->next;
  }
}
```

DFS的时间复杂度是$O(|V| + |E|)$，因为每个顶点和每条边都被访问一次。

DFS可以回答以下问题：

- **连通性**：DFS可以用来检查图是否连通，或者找到图的所有连通分量。
- **路径存在性**：DFS可以用来检查从一个顶点到另一个顶点是否存在路径。

### 广度优先搜索（BFS）
BFS按照与起点的距离（即边的数量）逐层访问顶点。BFS使用一个队列来实现。

![layer](layer.png)

BFS的实现：

```
将 s 标记为已访问，所有其他顶点标记为未访问
创建一个空队列 Q
将 s 入队 Q（即Q.enqueue(s)）
while Q 不为空：
    u = Q.dequeue()
    for 每条边 (u, w) ∈ E：
        if w未访问：
            将 w 标记为已访问
            将 w 入队 Q（即Q.enqueue(w)）
```

BFS的时间复杂度也是$O(|V| + |E|)$，因为每个顶点和每条边都被访问一次。

```c
void graph_bfs(const Graph *g, int start) {
  if (g == NULL || start < 0 || start >= g->V) {
    return;
  }

  bool *visited = calloc(g->V, sizeof(bool));
  Queue *q = queue_init();

  visited[start] = true;
  queue_enq(q, start);

  printf("BFS from %d:", start);

  int v;
  while (queue_deq(q, &v)) {
    printf(" %d", v);

    int size;
    int *neighbors = adj(g, v, &size);
    for (int i = 0; i < size; i++) {
      int w = neighbors[i];
      if (!visited[w]) {
        visited[w] = true;
        queue_enq(q, w);
      }
    }
    free(neighbors);
  }

  printf("\n");

  queue_destroy(q);
  free(visited);
}
```

类似的，引入`adj()`是为了简化代码。如果邻接表的实现是链表，可以直接遍历链表来访问邻居节点，从而避免每次都创建一个新的邻居数组（参考`graph_bfs2.c`）。

```c
  int v;
  while (queue_deq(q, &v)) {
    printf(" %d", v);

    Edge *curr = g->adj[v].head;
    while (curr != NULL) {
      int w = curr->vertex;
      if (!visited[w]) {
        visited[w] = true;
        queue_enq(q, w);
      }
      curr = curr->next;
    }
  }
```

BFS的独到之处在于，只需要添加额外几行代码，就能计算出**最短路径的长度**。


```
将 s 标记为已访问，所有其他顶点标记为未访问
l(s) = 0, 对其余v != s, l(v) = ∞
创建一个空队列 Q
将 s 入队 Q（即Q.enqueue(s)）
while Q 不为空：
    u = Q.dequeue()
    for 每条边 (u, w) ∈ E：
        if w未访问：
            将 w 标记为已访问
            l(w) = l(u) + 1
            将 w 入队 Q（即Q.enqueue(w)）
```