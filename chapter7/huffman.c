#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_USER_INPUT 1024
#define MAX_SYMBOLS (UCHAR_MAX + 1)

typedef struct node {
  unsigned char ch;
  int weight;
  struct node *left;
  struct node *right;
} Node;

Node *createLeaf(unsigned char ch, int weight) {
  Node *node = (Node *)malloc(sizeof(Node));
  if (node == NULL) {
    return NULL;
  }

  node->ch = ch;
  node->weight = weight;
  node->left = NULL;
  node->right = NULL;
  return node;
}

Node *createInternal(Node *left, Node *right) {
  Node *node = (Node *)malloc(sizeof(Node));
  if (node == NULL) {
    return NULL;
  }

  node->ch = '\0';
  node->weight = left->weight + right->weight;
  node->left = left;
  node->right = right;
  return node;
}

int isLeaf(const Node *node) {
  return node != NULL && node->left == NULL && node->right == NULL;
}

int compareNodePtr(const void *left, const void *right) {
  const Node *a = *(const Node *const *)left;
  const Node *b = *(const Node *const *)right;

  if (a->weight < b->weight) {
    return -1;
  }
  if (a->weight > b->weight) {
    return 1;
  }
  return 0;
}

void countFrequency(const char *text, int freq[]) {
  for (int i = 0; text[i] != '\0'; i++) {
    unsigned char ch = (unsigned char)text[i];
    if (ch == ' ' || ch == '\n' || ch == '\r') {
      continue;
    }
    freq[ch]++;
  }
}

Node *buildHuffmanTree(const int freq[]) {
  Node *nodes[MAX_SYMBOLS];
  int count = 0;

  for (int i = 0; i < MAX_SYMBOLS; i++) {
    if (freq[i] > 0) {
      Node *leaf = createLeaf((unsigned char)i, freq[i]);
      if (leaf == NULL) {
        return NULL;
      }
      nodes[count++] = leaf;
    }
  }

  if (count == 0) {
    return NULL;
  }

  while (count > 1) {
    // 这是为了方便写代码，真实应用中应该使用优先级队列实现（如min-heap）
    qsort(nodes, count, sizeof(nodes[0]), compareNodePtr);

    Node *left = nodes[0];
    Node *right = nodes[1];
    Node *parent = createInternal(left, right);
    if (parent == NULL) {
      return NULL;
    }
    // 通过左移动，相当于把两个最小权重的删去
    for (int i = 2; i < count; i++) {
      nodes[i - 2] = nodes[i];
    }
    count -= 2;
    nodes[count++] = parent;
  }

  return nodes[0];
}

void printChar(unsigned char ch) {
  if (ch == '\t') {
    printf("\\t");
  } else if (ch == '\n') {
    printf("\\n");
  } else if (ch == '\r') {
    printf("\\r");
  } else if (ch == '\'') {
    printf("\\'");
  } else if (ch == '\\') {
    printf("\\\\");
  } else if (isprint(ch)) {
    printf("%c", ch);
  } else {
    printf("\\x%02X", ch);
  }
}

void printNodeLabel(const Node *node) {
  if (isLeaf(node)) {
    printf("'");
    printChar(node->ch);
    printf("':%d", node->weight);
  } else {
    printf("%d", node->weight);
  }
}

/*
 * 树以顺时针旋转90°的方式呈现：
 *   - 右子树在上方
 *   - 根节点居中
 *   - 左子树在下方
 *
 * 内部结点显示权值；叶子结点显示 '字符':权值。
 */
static void print_tree_impl(const Node *node, const char *prefix, int is_left) {
  if (!node) {
    return;
  }

  {
    char new_prefix[256];
    snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix,
             is_left ? "│   " : "    ");
    print_tree_impl(node->right, new_prefix, 0);
  }

  printf("%s%s[", prefix, is_left ? "└── " : "┌── ");
  printNodeLabel(node);
  printf("]\n");

  {
    char new_prefix[256];
    snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix,
             is_left ? "    " : "│   ");
    print_tree_impl(node->left, new_prefix, 1);
  }
}

void print_tree(const Node *root) {
  if (!root) {
    printf("(empty tree)\n");
    return;
  }

  print_tree_impl(root->right, "    ", 0);
  printf("[");
  printNodeLabel(root);
  printf("]\n");
  print_tree_impl(root->left, "    ", 1);
}

void displayFrequency(const int freq[]) {
  printf("frequency:\n");
  for (int i = 0; i < MAX_SYMBOLS; i++) {
    if (freq[i] > 0) {
      printf("  '");
      printChar((unsigned char)i);
      printf("': %d\n", freq[i]);
    }
  }
}

void destroyTree(Node *root) {
  if (root != NULL) {
    destroyTree(root->left);
    destroyTree(root->right);
    free(root);
  }
}

int main(void) {
  char text[MAX_USER_INPUT];
  int freq[MAX_SYMBOLS] = {0};

  printf("input text: ");
  if (fgets(text, sizeof(text), stdin) == NULL) {
    printf("read input failed\n");
    return 1;
  }

  countFrequency(text, freq);

  Node *root = buildHuffmanTree(freq);
  if (root == NULL) {
    printf("empty input after ignoring spaces\n");
    return 1;
  }

  displayFrequency(freq);
  printf("huffman tree:\n");
  print_tree(root);

  destroyTree(root);
  return 0;
}
