#include <stdio.h>
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

void graph_print_adj(const Graph *g, int v) {
  printf("%d:", v);

  Edge *curr = g->adj[v].head;
  while (curr != NULL) {
    printf(" %d", curr->vertex);
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
