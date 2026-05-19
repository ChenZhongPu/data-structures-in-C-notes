#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Edge {
  int vertex;
  double weight;
  struct Edge *next;
} Edge;

typedef struct {
  Edge *head;
  int size;
} List;

typedef struct {
  int V;
  int E;
  List *adj;
} Graph;

Edge *edge_create(int vertex, double weight) {
  Edge *edge = malloc(sizeof(Edge));
  edge->vertex = vertex;
  edge->weight = weight;
  edge->next = NULL;
  return edge;
}

void list_init(List *list) {
  list->head = NULL;
  list->size = 0;
}

void list_add(List *list, int vertex, double weight) {
  Edge *edge = edge_create(vertex, weight);
  edge->next = list->head;
  list->head = edge;
  list->size++;
}

void list_destroy(List *list) {
  Edge *curr = list->head;
  while (curr != NULL) {
    Edge *next = curr->next;
    free(curr);
    curr = next;
  }

  list->head = NULL;
  list->size = 0;
}

Graph *graph_create(int V) {
  Graph *g = malloc(sizeof(Graph));
  g->V = V;
  g->E = 0;
  g->adj = malloc(sizeof(List) * V);

  for (int v = 0; v < V; v++) {
    list_init(&g->adj[v]);
  }

  return g;
}

void graph_add_edge(Graph *g, int v, int w, double weight) {
  list_add(&g->adj[v], w, weight);
  list_add(&g->adj[w], v, weight);
  g->E++;
}

void graph_print_adj(const Graph *g, int v) {
  printf("%d:", v);

  Edge *curr = g->adj[v].head;
  while (curr != NULL) {
    printf(" %d(%.2f)", curr->vertex, curr->weight);
    curr = curr->next;
  }

  printf("\n");
}

void graph_destroy(Graph *g) {
  if (g == NULL) {
    return;
  }

  for (int v = 0; v < g->V; v++) {
    list_destroy(&g->adj[v]);
  }

  free(g->adj);
  free(g);
}

int min_key_vertex(const Graph *g, const double *key, const bool *visited) {
  int min_vertex = -1;
  double min_key = DBL_MAX;

  for (int v = 0; v < g->V; v++) {
    if (!visited[v] && key[v] < min_key) {
      min_vertex = v;
      min_key = key[v];
    }
  }

  return min_vertex;
}

// 不使用堆。每轮线性扫描 key 选点，整体复杂度为 O(V^2 + E)，即 O(V^2)。
void graph_prim2(const Graph *g, int start) {
  if (g == NULL || start < 0 || start >= g->V) {
    return;
  }

  bool *visited = calloc(g->V, sizeof(bool));
  double *key = malloc(sizeof(double) * g->V);
  int *from = malloc(sizeof(int) * g->V);
  double total_weight = 0.0;

  for (int v = 0; v < g->V; v++) {
    key[v] = DBL_MAX;
    from[v] = -1;
  }
  key[start] = 0.0;

  printf("Prim MST from %d:\n", start);

  for (int i = 0; i < g->V; i++) {
    int v = min_key_vertex(g, key, visited);
    visited[v] = true;

    if (v != start) {
      printf("%d - %d: %.2f\n", from[v], v, key[v]);
      total_weight += key[v];
    }

    Edge *curr = g->adj[v].head;
    while (curr != NULL) {
      int w = curr->vertex;
      if (!visited[w] && curr->weight < key[w]) {
        key[w] = curr->weight;
        from[w] = v;
      }
      curr = curr->next;
    }
  }

  printf("total weight: %.2f\n", total_weight);

  free(visited);
  free(key);
  free(from);
}

Graph *build_sample_graph(void) {
  Graph *g = graph_create(6);

  graph_add_edge(g, 0, 1, 4.0);
  graph_add_edge(g, 0, 2, 2.0);
  graph_add_edge(g, 1, 2, 1.0);
  graph_add_edge(g, 1, 3, 5.0);
  graph_add_edge(g, 2, 3, 8.0);
  graph_add_edge(g, 2, 4, 10.0);
  graph_add_edge(g, 3, 4, 2.0);
  graph_add_edge(g, 3, 5, 6.0);
  graph_add_edge(g, 4, 5, 3.0);

  return g;
}

int main(void) {
  Graph *g = build_sample_graph();

  for (int v = 0; v < g->V; v++) {
    graph_print_adj(g, v);
  }

  graph_prim2(g, 0);

  graph_destroy(g);
  return 0;
}
