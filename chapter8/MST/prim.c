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

// 时间复杂度是O(|V||E|)
void graph_prim(const Graph *g, int start) {
  if (g == NULL || start < 0 || start >= g->V) {
    return;
  }

  bool *visited = calloc(g->V, sizeof(bool));
  double total_weight = 0.0;
  int edge_count = 0;

  visited[start] = true;

  printf("Prim MST from %d:\n", start);

  while (edge_count < g->V - 1) {
    int best_from = -1;
    int best_to = -1;
    double best_weight = DBL_MAX;

    for (int v = 0; v < g->V; v++) {
      if (!visited[v]) {
        continue;
      }

      Edge *curr = g->adj[v].head;
      while (curr != NULL) {
        int w = curr->vertex;
        if (!visited[w] && curr->weight < best_weight) {
          best_from = v;
          best_to = w;
          best_weight = curr->weight;
        }
        curr = curr->next;
      }
    }

    printf("%d - %d: %.2f\n", best_from, best_to, best_weight);
    visited[best_to] = true;
    total_weight += best_weight;
    edge_count++;
  }

  if (edge_count == g->V - 1) {
    printf("total weight: %.2f\n", total_weight);
  }

  free(visited);
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

  graph_prim(g, 0);

  graph_destroy(g);
  return 0;
}
