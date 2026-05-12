#include <stdio.h>
#include <stdlib.h>

#define LIST_INIT_CAPACITY 16

typedef struct {
  int vertex;
  double weight;
} WeightedEdge;

typedef struct {
  WeightedEdge *data;
  int size;
  int capacity;
} AdjList;

typedef struct {
  int V;
  int E;
  AdjList *adj;
} Graph;

void adjlist_init(AdjList *list) {
  list->data = malloc(sizeof(WeightedEdge) * LIST_INIT_CAPACITY);
  list->size = 0;
  list->capacity = LIST_INIT_CAPACITY;
}

void adjlist_add(AdjList *list, int vertex, double weight) {
  if (list->size == list->capacity) {
    int new_capacity = list->capacity * 2;

    WeightedEdge *new_data = realloc(list->data, sizeof(WeightedEdge) * new_capacity);
    list->data = new_data;
    list->capacity = new_capacity;
  }

  list->data[list->size].vertex = vertex;
  list->data[list->size].weight = weight;
  list->size++;
}

void adjlist_destroy(AdjList *list) {
  free(list->data);
  list->data = NULL;
  list->size = 0;
  list->capacity = 0;
}

Graph *graph_create(int V) {
  Graph *g = malloc(sizeof(Graph));
  if (g == NULL) {
    return NULL;
  }

  g->V = V;
  g->E = 0;
  g->adj = malloc(sizeof(AdjList) * V);
  if (g->adj == NULL) {
    free(g);
    return NULL;
  }

  for (int v = 0; v < V; v++) {
    adjlist_init(&g->adj[v]);
  }

  return g;
}

void graph_add_edge(Graph *g, int v, int w, double weight) {
  adjlist_add(&g->adj[v], w, weight);
  adjlist_add(&g->adj[w], v, weight);
  g->E++;
}

void graph_print_adj(const Graph *g, int v) {
  printf("%d:", v);

  for (int i = 0; i < g->adj[v].size; i++) {
    printf(" %d(%.2f)", g->adj[v].data[i].vertex, g->adj[v].data[i].weight);
  }

  printf("\n");
}

void graph_destroy(Graph *g) {
  if (g == NULL) {
    return;
  }

  for (int v = 0; v < g->V; v++) {
    adjlist_destroy(&g->adj[v]);
  }

  free(g->adj);
  free(g);
}

int main(void) {
  Graph *g = graph_create(4);

  graph_add_edge(g, 0, 1, 2.5);
  graph_add_edge(g, 0, 2, 1.8);
  graph_add_edge(g, 1, 2, 3.2);
  graph_add_edge(g, 2, 3, 4.1);

  for (int v = 0; v < g->V; v++) {
    graph_print_adj(g, v);
  }

  graph_destroy(g);
  return 0;
}
