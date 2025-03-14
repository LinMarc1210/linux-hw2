#include <stddef.h>
typedef struct list_item {
  int value;
  struct list_item *next;
} list_item_t;

typedef struct {
  struct list_item *head;
} list_t;

size_t list_size(const list_t *l);
void list_insert_before(list_t *l, list_item_t *before, list_item_t *item);
void list_insert_before_naive(list_t *l, list_item_t *before,
                              list_item_t *item);