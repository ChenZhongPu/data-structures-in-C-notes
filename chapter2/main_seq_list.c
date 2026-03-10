#include "seq_list.h"
#include <stdio.h>

static void print_bool(const char *label, bool value) {
  printf("%s: %s\n", label, value ? "true" : "false");
}

int main(void) {
  List L = list_init();
  if (L == NULL) {
    printf("list_init failed\n");
    return 1;
  }

  print_bool("empty at start", list_is_empty(L));
  printf("length at start: %zu\n", list_length(L));

  print_bool("insert 10 at 0", list_insert(L, 0, 10));
  print_bool("insert 20 at 1", list_insert(L, 1, 20));
  print_bool("insert 15 at 1", list_insert(L, 1, 15));

  printf("after inserts: ");
  list_display(L);

  printf("get index 0: %d\n", list_get(L, 0));
  printf("get index 1: %d\n", list_get(L, 1));
  printf("get index 2: %d\n", list_get(L, 2));

  printf("locate 15: %d\n", list_locate(L, 15));
  printf("locate 99: %d\n", list_locate(L, 99));

  print_bool("delete index 1", list_delete(L, 1));
  printf("after delete: ");
  list_display(L);

  print_bool("insert 20 at 1", list_insert(L, 1, 20));
  print_bool("insert 30 at 2", list_insert(L, 2, 30));
  print_bool("insert 20 at 3", list_insert(L, 3, 20));

  printf("before delete elem 20: ");
  list_display(L);
  list_delete_elem(L, 20);
  printf("after delete elem 20: ");
  list_display(L);

  list_delete_elem(L, 99);
  printf("after delete elem 99: ");
  list_display(L);

  list_delete_elem(L, 10);
  list_delete_elem(L, 30);
  printf("after delete elem 10 and 30: ");
  list_display(L);

  printf("length at end: %zu\n", list_length(L));
  print_bool("empty at end", list_is_empty(L));

  list_destroy(L);
  return 0;
}
