#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int from;
  int to;
  double weight;
} DirectedEdge;

typedef struct AdjNode {
  DirectedEdge edge;
  struct AdjNode *next;
} AdjNode;

typedef struct {
  AdjNode *head;
  int size;
} List;

typedef struct {
  int V;
  int E;
  List *adj;
} EdgeWeightedDigraph;

DirectedEdge directed_edge_create(int from, int to, double weight) {
  DirectedEdge e = {from, to, weight};
  return e;
}

AdjNode *adj_node_create(DirectedEdge edge) {
  AdjNode *node = malloc(sizeof(AdjNode));
  node->edge = edge;
  node->next = NULL;
  return node;
}

void list_init(List *list) {
  list->head = NULL;
  list->size = 0;
}

void list_add(List *list, DirectedEdge edge) {
  AdjNode *node = adj_node_create(edge);
  node->next = list->head;
  list->head = node;
  list->size++;
}

void list_destroy(List *list) {
  AdjNode *curr = list->head;
  while (curr != NULL) {
    AdjNode *next = curr->next;
    free(curr);
    curr = next;
  }

  list->head = NULL;
  list->size = 0;
}

EdgeWeightedDigraph *digraph_create(int V) {
  EdgeWeightedDigraph *g = malloc(sizeof(EdgeWeightedDigraph));
  g->V = V;
  g->E = 0;
  g->adj = malloc(sizeof(List) * V);

  for (int v = 0; v < V; v++) {
    list_init(&g->adj[v]);
  }

  return g;
}

void digraph_add_edge(EdgeWeightedDigraph *g, DirectedEdge e) {
  list_add(&g->adj[e.from], e);
  g->E++;
}

void digraph_print_adj(const EdgeWeightedDigraph *g, int v) {
  printf("%d:", v);

  AdjNode *curr = g->adj[v].head;
  while (curr != NULL) {
    printf(" %d->%d(%.2f)", curr->edge.from, curr->edge.to, curr->edge.weight);
    curr = curr->next;
  }

  printf("\n");
}

void digraph_destroy(EdgeWeightedDigraph *g) {
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
  EdgeWeightedDigraph *g = digraph_create(8);

  digraph_add_edge(g, directed_edge_create(4, 5, 0.35));
  digraph_add_edge(g, directed_edge_create(5, 4, 0.35));
  digraph_add_edge(g, directed_edge_create(4, 7, 0.37));
  digraph_add_edge(g, directed_edge_create(5, 7, 0.28));
  digraph_add_edge(g, directed_edge_create(7, 5, 0.28));
  digraph_add_edge(g, directed_edge_create(5, 1, 0.32));
  digraph_add_edge(g, directed_edge_create(0, 4, 0.38));
  digraph_add_edge(g, directed_edge_create(0, 2, 0.26));
  digraph_add_edge(g, directed_edge_create(7, 3, 0.39));
  digraph_add_edge(g, directed_edge_create(1, 3, 0.29));
  digraph_add_edge(g, directed_edge_create(2, 7, 0.34));
  digraph_add_edge(g, directed_edge_create(6, 2, 0.40));
  digraph_add_edge(g, directed_edge_create(3, 6, 0.52));
  digraph_add_edge(g, directed_edge_create(6, 0, 0.58));
  digraph_add_edge(g, directed_edge_create(6, 4, 0.93));

  for (int v = 0; v < g->V; v++) {
    digraph_print_adj(g, v);
  }

  digraph_destroy(g);
  return 0;
}
