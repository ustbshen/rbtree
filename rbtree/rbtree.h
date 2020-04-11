#pragma once

#include <stdint.h>

typedef struct rbtree_node
{
    struct rbtree_node* _parent_and_color;
    struct rbtree_node* left;
    struct rbtree_node* right;
} rbtree_node_t;

typedef int32_t (*rbtree_node_cmp_func_t)(rbtree_node_t* node, void* value);

typedef struct rbtree
{
    rbtree_node_t* root;
    rbtree_node_cmp_func_t cmp_func;
} rbtree_t;

typedef enum rbtree_search_type
{
    RBTREE_SEARCH_EQUAL,
    RBTREE_SEARCH_NOT_SMALL,
    RBTREE_SEARCH_NOT_LARGE,
} rbtree_search_type_t;

//api
void rbtree_init(rbtree_t* tree, rbtree_node_cmp_func_t cmp_func);
void rbtree_insert(rbtree_t* tree, rbtree_node_t* node, void* node_val);
void rbtree_remove(rbtree_t* tree, rbtree_node_t* node);
rbtree_node_t* rbtree_first(rbtree_t* tree);
rbtree_node_t* rbtree_last(rbtree_t* tree);
rbtree_node_t* rbtree_next(rbtree_node_t* node);
rbtree_node_t* rbtree_prev(rbtree_node_t* node);
rbtree_node_t* rbtree_search(rbtree_t* tree, void* value, rbtree_search_type_t type);
