#ifndef CODE_LIST_H
#define CODE_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>

typedef struct node_ {
    struct node_ *prev;
    struct node_ *next;
    void *val;
} node;

#define forward(n) (n->next)
#define backward(n) (n->prev)

typedef struct list_ {
    node *front;
    node *back;
    size_t size;
} list;

list *make_list();

void *list_pop(list *l);

void list_insert(list *l, void *val);

void free_node(node *n);

void free_list(list *l);

void free_list_contents(list *l);

void **list_to_array(list *l);

void free_array(void **arr, size_t n);

#ifdef __cplusplus
}
#endif

#endif //CODE_LIST_H
