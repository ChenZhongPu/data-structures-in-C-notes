#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  int data;
  struct Node *next;
} Node;

typedef struct Queue {
  Node *head;
  Node *tail;
} Queue;

Queue *queue_init(void) {
  Queue *q = malloc(sizeof(Queue));
  q->head = NULL;
  q->tail = NULL;
  return q;
}

void queue_destroy(Queue *q) {
  Node *current = q->head;
  while (current) {
    Node *tmp = current;
    current = current->next;
    free(tmp);
  }
  free(q);
}

void queue_enq(Queue *q, int val) {
  Node *new_node = malloc(sizeof(Node));
  new_node->data = val;
  new_node->next = NULL;

  if (q->head == NULL) {
    q->head = new_node;
    q->tail = new_node;
  } else {
    q->tail->next = new_node;
    q->tail = new_node;
  }
}

bool queue_deq(Queue *q, int *val) {
  if (q->head == NULL) {
    return false;
  }

  Node *node = q->head;
  *val = node->data;

  if (q->head == q->tail) {
    q->head = NULL;
    q->tail = NULL;
  } else {
    q->head = q->head->next;
  }

  free(node);
  return true;
}

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

  printf("\n");

  queue_destroy(q);
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

  graph_bfs(g, 0);

  graph_destroy(g);
  return 0;
}
