#include "list.h"

size_t list_size(const list_t *l) {
  size_t size = 0;
  list_item_t *cur = l->head;
  while (cur) {
    size++;
    cur = cur->next;
  }
  return size;
}

void list_insert_before(list_t *l, list_item_t *before, list_item_t *item) {
  list_item_t **p;
  for (p = &(l)->head; *p != before; p = &(*p)->next)
    ;
  *p = item;
  item->next = before;
}

void list_insert_before_naive(list_t *l, list_item_t *before,
                              list_item_t *item) {
  list_item_t *pos = l->head;
  if (before == l->head) {
    item->next = before;
    l->head = item;
    return;
  }

  while (pos && pos->next != before) {
    pos = pos->next;
  }

  item->next = before;
  pos->next = item;
}