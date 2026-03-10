# 第2章：线性表

## 2.1

关于`linear list`的概念在很多国外经典教材中，一般并不强调，而是直接用`list`表示。为了区分它的表示方式，可以说`array-based list`或`linked list`。《算法导论》为例，它直接甚至没有直接单独提出`list`，而是先`array-based data structures`，然后`linked lists`。

关于接口命名，不同于教材习惯的风格，这里采用前缀区分的方式。比如：

```c
bool list_is_empty(const List L);

size_t list_length(const List L);

void list_display(const List L);

ElemType list_get(const List L, size_t i);
```

> C23 引入了 `constexpr`，可以取代 `#define` 定义常量。

> C23 引入了 `nullptr`，可以取代 `NULL` 定义空指针。

## 2.2

类似的，`sequential list`在很多国外经典教材中也很少提及。此外，JDK有个[AbstractSequentialList](https://docs.oracle.com/javase/8/docs/api/java/util/AbstractSequentialList.html)，其内涵也和教材中不同。

本节的`SqList`被设计成定长的，但在实践中，设计成可变长的更为常见。比如Java中的`ArrayList`，C++中的`std::vector`。

在API的设计中，本书大量使用C++的引用用于返回值，但并不直观。比如下面的创建/初始化的API更合理：

```c
ArrayList* arraylist_new(unsigned int initial_capacity);
```

对于`GetElem`操作，它返回bool表示是否找到。但在实践中，常见的做法更简单：为了效率，不进行边界检查，直接返回元素值。如果调用者传入了非法下标，程序就会崩溃；并且，这个操作一般使用宏定义实现。

当然，对于`insert`和`delete`操作，仍然需要进行边界检查，那么到底如何表示失败呢？有多种方案，一种是类似教材的做法，返回bool表示成功与否（CPython实现的是0/-1表示）；另一种是统一的异常处理，比如glib的`g_return_val_if_fail`。

> C语言不提供内建的异常处理机制。

对于移动操作，在单线程下可以使用`memmove`函数实现，效率更高。

```c
void insertElement(List *list, size_t index, ElemType value) {
  // ignore the resize operation for simplicity
  memmove(&list->data[index + 1], &list->data[index],
          (list->size - index) * sizeof(ElemType));
  // Insert new element
  list->data[index] = value;
  list->size++;
}
```

## 2.3

2.3.1提到“指针域”的概念。这是国内技术翻译糟糕的一个缩影。英文原文是`pointer field`，指的是结构体中的一个成员变量，它的类型是指针。翻译成“指针域”非常不直观，容易让人误解为“指针的范围”或者“指针的作用域”。更合理的翻译应该是“指针成员”。总而言之，field的正确翻译应该是“成员”、“字段”，而不是“域”；结合上下文，有时理解成“属性”也可以。

教材严格区分`head pointer`和`first pointer`，这在国外教材更常见的说法是：**header**表示这个特殊的结点（dummy node 或 sentinel node），而**head/first**表示指向第一个结点的指针。为了保持和经典教材一致，这里也采用了这种区分的方式。

2.3.2提到如果引入*header*能简化代码。这是一个经典的设计技巧。作为比对，下面是没有引入header的时候的插入和删除操作：

```c
typedef int ElemType;

typedef struct Node
{
  ElemType data;
  struct Node *next;
} Node;

Node *
add_first (Node *list, ElemType e)
{
  Node *new_node = malloc (sizeof (Node));
  new_node->data = e;
  new_node->next = list;
  return new_node;
}

Node *
remove_first (Node *list)
{
  if (list == NULL)
    return NULL;
  Node *temp = list;
  list = list->next;
  free (temp);
  return list;
}
```

> **header**能简化编程的本质原因是：它永远在链表的最前面，这样就没有必要再边界情况，更不需要返回新的头指针了。

> 读者应该能独立完成不同风格的实现，比如：仅有head；仅有header;有head和tail等。
