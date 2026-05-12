#include <stdio.h>
#include <stdlib.h>

typedef struct AdjNode {
  int v;
  struct AdjNode *next;
} AdjNode;

typedef struct Graph {
  int V;         // 顶点的数量
  int E;         // 边的数量
  AdjNode **adj; // adj[i]是顶点i对应的adj list
} Graph;

Graph *graph_create(int V) {
  Graph *g = malloc(sizeof(Graph));
  g->V = V;
  g->E = 0;
  g->adj = calloc(V, sizeof(AdjNode *));
  return g;
}

static void add_to_list(AdjNode **head, int v) {
  AdjNode *node = malloc(sizeof(AdjNode));
  node->v = v;
  node->next = *head;
  *head = node;
}

// 无向图
void graph_add_edge(Graph *g, int v1, int v2) {
  add_to_list(&g->adj[v1], v2);
  add_to_list(&g->adj[v2], v1);
  g->E++;
}

void graph_print_adj(const Graph *g, int v) {
  printf("%d:", v);
  for (AdjNode *p = g->adj[v]; p != NULL; p = p->next) {
    printf(" %d", p->v);
  }
  printf("\n");
}

void graph_destroy(Graph *g) {
  if (g == NULL) {
    return;
  }

  for (int v = 0; v < g->V; v++) {
    AdjNode *p = g->adj[v];
    while (p != NULL) {
      AdjNode *next = p->next;
      free(p);
      p = next;
    }
  }
  free(g->adj);
  free(g);
}

int main(void) {
  Graph *g = graph_create(4);
  graph_add_edge(g, 0, 1);
  graph_add_edge(g, 0, 2);
  graph_add_edge(g, 1, 2);
  graph_add_edge(g, 2, 3);

  for (int v = 0; v < g->V; v++) {
    graph_print_adj(g, v);
  }

  graph_destroy(g);
  return 0;
}
