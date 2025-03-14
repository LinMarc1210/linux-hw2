#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define my_assert(test, message)                                               \
  do {                                                                         \
    if (!(test))                                                               \
      return message;                                                          \
  } while (0)
#define my_run_test(test)                                                      \
  do {                                                                         \
    char *message = test();                                                    \
    tests_run++;                                                               \
    if (message)                                                               \
      return message;                                                          \
  } while (0)

#define N 5

typedef struct block {
  size_t size;
  struct block *l, *r;
} block_t;

static block_t nodes[N];
static block_t *root = NULL;

block_t **find_free_tree(block_t **root, block_t *target) {
  if (!root || !*root)
    return NULL;
  if (*root == target)
    return root;

  block_t **left_result = find_free_tree(&(*root)->l, target);
  if (left_result)
    return left_result;
  return find_free_tree(&(*root)->r, target);
}

block_t *find_predecessor_free_tree(block_t **root, block_t *node) {
  if (!root || !*root)
    return NULL;

  block_t *pred = NULL;
  block_t *pos = *root;

  if (node->l) {
    pos = node->l;
    while (pos->r)
      pos = pos->r;
    pred = pos;
  } else {
    while (pos) {
      if (pos->size < node->size) {
        pred = pos;
        pos = pos->r;
      } else {
        pos = pos->l;
      }
    }
    pred = pos;
  }

  return pred;
}

void remove_free_tree(block_t **root, block_t *target) {
  block_t **node_ptr = find_free_tree(root, target);

  if ((*node_ptr)->l && (*node_ptr)->r) {
    block_t **pred_ptr = &(*node_ptr)->l;
    while ((*pred_ptr)->r)
      pred_ptr = &(*pred_ptr)->r;

    block_t *expected_pred = find_predecessor_free_tree(root, *node_ptr);
    assert(expected_pred == *pred_ptr);

    if (*pred_ptr == (*node_ptr)->l) {
      block_t *old_right = (*node_ptr)->r;
      *node_ptr = *pred_ptr;
      (*node_ptr)->r = old_right;
      assert(*node_ptr != (*node_ptr)->l);
      assert(*node_ptr != (*node_ptr)->r);
    } else {
      block_t *old_left = (*node_ptr)->l;
      block_t *old_right = (*node_ptr)->r;
      block_t *pred_node = *pred_ptr;
      remove_free_tree(&old_left, *pred_ptr);
      *node_ptr = pred_node;
      (*node_ptr)->l = old_left;
      (*node_ptr)->r = old_right;
      assert(*node_ptr != (*node_ptr)->l);
      assert(*node_ptr != (*node_ptr)->r);
    }
  } else if ((*node_ptr)->l || (*node_ptr)->r) {
    block_t *child = ((*node_ptr)->l) ? (*node_ptr)->l : (*node_ptr)->r;
    *node_ptr = child;
  } else {
    *node_ptr = NULL;
  }

  target->l = NULL;
  target->r = NULL;
}

static block_t *create_node(size_t size) {
  block_t *node = &nodes[size];
  node->size = size;
  node->l = NULL;
  node->r = NULL;
  return node;
}

static void insert_node(block_t **root, block_t *node) {
  if (!*root) {
    *root = node;
    return;
  }
  if (node->size < (*root)->size)
    insert_node(&(*root)->l, node);
  else
    insert_node(&(*root)->r, node);
}

static void reset_tree(void) {
  for (size_t i = 0; i < N; i++) {
    nodes[i].size = i;
    nodes[i].l = NULL;
    nodes[i].r = NULL;
  }
  root = NULL;
}

static int tree_size(block_t *node) {
  if (!node)
    return 0;
  return 1 + tree_size(node->l) + tree_size(node->r);
}

static char *test_remove_node(void) {
  reset_tree();
  insert_node(&root, create_node(3));
  insert_node(&root, create_node(1));
  insert_node(&root, create_node(4));
  insert_node(&root, create_node(0));
  insert_node(&root, create_node(2));

  my_assert(tree_size(root) == 5, "Tree size should be 5 after insertion");

  remove_free_tree(&root, &nodes[1]);
  my_assert(tree_size(root) == 4, "Tree size should be 4 after removal");

  remove_free_tree(&root, &nodes[3]);
  my_assert(tree_size(root) == 3, "Tree size should be 3 after removal");

  remove_free_tree(&root, &nodes[0]);
  my_assert(tree_size(root) == 2, "Tree size should be 2 after removal");

  return NULL;
}

int tests_run = 0;

static char *test_suite(void) {
  my_run_test(test_remove_node);
  return NULL;
}

int main(void) {
  printf("---=[ Tree Removal Tests\n");
  char *result = test_suite();
  if (result)
    printf("ERROR: %s\n", result);
  else
    printf("ALL TESTS PASSED\n");
  printf("Tests run: %d\n", tests_run);
  return !!result;
}
