#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct Edge {
  int vertex;
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

Edge *edge_create(int vertex) {
  Edge *edge = malloc(sizeof(Edge));
  edge->vertex = vertex;
  edge->next = NULL;
  return edge;
}

void list_init(List *list) {
  list->head = NULL;
  list->size = 0;
}

void list_add(List *list, int vertex) {
  Edge *edge = edge_create(vertex);
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

void graph_add_edge(Graph *g, int v, int w) {
  list_add(&g->adj[v], w);
  list_add(&g->adj[w], v);
  g->E++;
}

int *adj(const Graph *g, int v, int *size) {
  *size = g->adj[v].size;
  if (*size == 0) {
    return NULL;
  }

  int *neighbors = malloc(sizeof(int) * *size);
  Edge *curr = g->adj[v].head;
  for (int i = 0; i < *size; i++) {
    neighbors[i] = curr->vertex;
    curr = curr->next;
  }

  return neighbors;
}

void graph_print_adj(const Graph *g, int v) {
  printf("%d:", v);

  int size;
  int *neighbors = adj(g, v, &size);
  for (int i = 0; i < size; i++) {
    printf(" %d", neighbors[i]);
  }

  free(neighbors);
  printf("\n");
}

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

  bool *visited = calloc(g->V, sizeof(bool));

  printf("DFS from %d:", start);
  graph_dfs_recursive(g, start, visited);
  printf("\n");

  free(visited);
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

Graph *build_image_graph(void) {
  Graph *g = graph_create(6);

  /*
   * list_add uses head insertion, so these edge calls are ordered to produce:
   * 0: 2 1 5
   * 1: 0 2
   * 2: 0 1 3 4
   * 3: 5 4 2
   * 4: 3 2
   * 5: 3 0
   */
  graph_add_edge(g, 2, 4);
  graph_add_edge(g, 2, 3);
  graph_add_edge(g, 1, 2);
  graph_add_edge(g, 0, 5);
  graph_add_edge(g, 3, 4);
  graph_add_edge(g, 0, 1);
  graph_add_edge(g, 3, 5);
  graph_add_edge(g, 0, 2);

  return g;
}

int main(void) {
  Graph *g = build_image_graph();

  for (int v = 0; v < g->V; v++) {
    graph_print_adj(g, v);
  }

  graph_dfs(g, 0);

  graph_destroy(g);
  return 0;
}
