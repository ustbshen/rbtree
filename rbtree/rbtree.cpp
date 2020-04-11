#include "rbtree.h"
#include "defines.h"

#define _RBTREE_NODE_PARENT_PTR_MASK                  (0xFFFFFFFE)
#define _RBTREE_NODE_COLOR_MASK                     (~_RBTREE_NODE_PARENT_PTR_MASK)

#define _RBTREE_NODE_COLOR_BLACK                    (0)
#define _RBTREE_NODE_COLOR_RED                      (1)

//基本操作

static uint32_t _rbtree_node_get_color(rbtree_node_t* node)
{
    if (node == NULL)
        return _RBTREE_NODE_COLOR_BLACK;
    else
        return ((uint32_t)node->_parent_and_color) & _RBTREE_NODE_COLOR_MASK;
}

static void _rbtree_node_set_color(rbtree_node_t* node, uint32_t color)
{
    if (node == NULL)
        return;

    uint32_t p_c = (uint32_t)node->_parent_and_color;
    p_c &= _RBTREE_NODE_PARENT_PTR_MASK;
    p_c |= color & _RBTREE_NODE_COLOR_MASK;
    node->_parent_and_color = (rbtree_node_t*)p_c;
}

static rbtree_node_t* _rbtree_node_get_parent(rbtree_node_t* node)
{
    if (node == NULL)
        return NULL;

    return (rbtree_node_t*)((uint32_t)node->_parent_and_color & _RBTREE_NODE_PARENT_PTR_MASK);
}

static void _rbtree_node_set_parent(rbtree_node_t* node, rbtree_node_t* parent)
{
    if (node == NULL)
        return;

    node->_parent_and_color = (rbtree_node_t*)((uint32_t)parent | _rbtree_node_get_color(node));
}

//红色节点判断
static int _rbtree_node_is_red(rbtree_node_t* node)
{
    if (node == NULL)
        return 0;
    else
        return _rbtree_node_get_color(node) == _RBTREE_NODE_COLOR_RED;
}
//黑色节点判断
static int _rbtree_node_is_black(rbtree_node_t* node)
{
    if (node == NULL)
        return 1;
    else
        return _rbtree_node_get_color(node) == _RBTREE_NODE_COLOR_BLACK;
}

//节点染红
static void _rbtree_node_set_red(rbtree_node_t* node)
{
    _rbtree_node_set_color(node, _RBTREE_NODE_COLOR_RED);
}

//节点染黑
static void _rbtree_node_set_black(rbtree_node_t* node)
{
    _rbtree_node_set_color(node, _RBTREE_NODE_COLOR_BLACK);
}

//左旋
static void _rbtree_left_rotate(rbtree_node_t** root, rbtree_node_t* x)
{
    //左旋的当前节点必须有右子树
    rbtree_node_t* y = x->right;                                //  y = x.right
    ASSERT(y);
    x->right = y->left;                                         //  x.right = y.left
    if (y->left != NULL)                                        //  if y.left != nil
        _rbtree_node_set_parent(y->left, x);                    //      y.left.p = x
    _rbtree_node_set_parent(y, _rbtree_node_get_parent(x));     //  y.p = x.p
    if (_rbtree_node_get_parent(x) == NULL)                     //  if x.p == nil
        *root = y;                                              //      root = y
    else if (x == _rbtree_node_get_parent(x)->left)             //  elseif x == x.p.left
        _rbtree_node_get_parent(x)->left = y;                   //      x.p.left = y
    else                                                        //  else
        _rbtree_node_get_parent(x)->right = y;                  //      x.p.right = y
    y->left = x;                                                //  y.left = x
    _rbtree_node_set_parent(x, y);                              //  x.p = y
}

//右旋
static void _rbtree_right_rotate(rbtree_node_t** root, rbtree_node_t* y)
{
    //右旋节点必须有左子树
    rbtree_node_t* x = y->left;                                 //  x = y.left
    ASSERT(x);
    y->left = x->right;                                         //  y.left = x.right
    if (x->right != NULL)                                       //  if x.right != nil
        _rbtree_node_set_parent(x->right, y);                   //      x.right.p = y
    _rbtree_node_set_parent(x, _rbtree_node_get_parent(y));     //  x.p = y.p
    if (_rbtree_node_get_parent(y) == NULL)                     //  if y.p == nil
        *root = x;                                              //      root = x
    else if (y == _rbtree_node_get_parent(y)->right)            //  elseif y == y.p.right
        _rbtree_node_get_parent(y)->right = x;                  //      y.p.right = x
    else                                                        //  else
        _rbtree_node_get_parent(y)->left = x;                   //      y.p.left = x
    x->right = y;                                                //  x.right = y
    _rbtree_node_set_parent(y, x);                              //  y.p = x
}

static void _rbtree_insert_fixup(rbtree_node_t** root, rbtree_node_t* z)
{
    rbtree_node_t* p;
    rbtree_node_t* pp;

    while (_rbtree_node_is_red(_rbtree_node_get_parent(z)))
    {
        p = _rbtree_node_get_parent(z);
        pp = _rbtree_node_get_parent(p);
        if (p == pp->left)
        {
            rbtree_node_t* y = pp->right;
            if (_rbtree_node_is_red(y))
            {
                _rbtree_node_set_black(y);
                _rbtree_node_set_black(p);
                _rbtree_node_set_red(pp);
                z = pp;
            }
            else
            {
                if (z == p->right)
                {
                    z = p;
                    _rbtree_left_rotate(root, z);
                    p = _rbtree_node_get_parent(z);
                }
                _rbtree_node_set_black(p);
                _rbtree_node_set_red(pp);
                _rbtree_right_rotate(root, pp);
            }
        }
        else
        {
            rbtree_node_t* y = pp->left;
            if (_rbtree_node_is_red(y))
            {
                _rbtree_node_set_black(y);
                _rbtree_node_set_black(p);
                _rbtree_node_set_red(pp);
                z = pp;
            }
            else
            {
                if (z == p->left)
                {
                    z = p;
                    _rbtree_right_rotate(root, z);
                    p = _rbtree_node_get_parent(z);
                }
                _rbtree_node_set_black(p);
                _rbtree_node_set_red(pp);
                _rbtree_left_rotate(root, pp);
            }
        }
    }

    _rbtree_node_set_black(*root);
}

static void _rbtree_transplant(rbtree_node_t** root, rbtree_node_t* u, rbtree_node_t* v)
{
    if (_rbtree_node_get_parent(u) == NULL)
        *root = v;
    else if (u == _rbtree_node_get_parent(u)->left)
        _rbtree_node_get_parent(u)->left = v;
    else
        _rbtree_node_get_parent(u)->right = v;
    _rbtree_node_set_parent(v, _rbtree_node_get_parent(u));
}

static rbtree_node_t* _rbtree_minimum(rbtree_node_t* root)
{
    rbtree_node_t* min = root;
    if (!min)
        return NULL;
    while (min->left)
        min = min->left;

    return min;
}

static rbtree_node_t* _rbtree_maximum(rbtree_node_t* root)
{
    rbtree_node_t* max = root;
    if (!max)
        return NULL;
    while (max->left)
        max = max->left;

    return max;
}

static void _rbtree_delete_fixup(rbtree_node_t** root, rbtree_node_t* x, rbtree_node_t* x_p)
{
    while ((x != *root) && _rbtree_node_is_black(x))
    {
        if (x_p->left == x)
        {
            rbtree_node_t* w = x_p->right;
            if (_rbtree_node_is_red(w))
            {
                _rbtree_node_set_black(w);
                _rbtree_node_set_red(x_p);
                _rbtree_left_rotate(root, x_p);
                w = x_p->right;
            }
            if (_rbtree_node_is_black(w->left) && _rbtree_node_is_black(w->right))
            {
                _rbtree_node_set_red(w);
                x = x_p;
                x_p = _rbtree_node_get_parent(x);
            }
            else
            {
                if (_rbtree_node_is_black(w->right))
                {
                    _rbtree_node_set_black(w->left);
                    _rbtree_node_set_red(w);
                    _rbtree_right_rotate(root, w);
                    w = x_p->right;
                }
                _rbtree_node_set_color(w, _rbtree_node_get_color(x_p));
                _rbtree_node_set_black(x_p);
                _rbtree_node_set_black(w->right);
                _rbtree_left_rotate(root, x_p);
                x = *root;
                break;
            }
        }
        else
        {
            rbtree_node_t* w = x_p->left;
            if (_rbtree_node_is_red(w))
            {
                _rbtree_node_set_black(w);
                _rbtree_node_set_red(x_p);
                _rbtree_right_rotate(root, x_p);
                w = x_p->left;
            }
            if (_rbtree_node_is_black(w->left) && _rbtree_node_is_black(w->right))
            {
                _rbtree_node_set_red(w);
                x = x_p;
                x_p = _rbtree_node_get_parent(x);
            }
            else
            {
                if (_rbtree_node_is_black(w->left))
                {
                    _rbtree_node_set_black(w->right);
                    _rbtree_node_set_red(w);
                    _rbtree_left_rotate(root, w);
                    w = x_p->left;
                }
                _rbtree_node_set_color(w, _rbtree_node_get_color(x_p));
                _rbtree_node_set_black(x_p);
                _rbtree_node_set_black(w->left);
                _rbtree_right_rotate(root, x_p);
                x = *root;
                break;
            }
        }
    }
    _rbtree_node_set_black(x);
}

//API
//初始化树
void rbtree_init(rbtree_t* tree, rbtree_node_cmp_func_t cmp_func)
{
    tree->root = NULL;
    tree->cmp_func = cmp_func;
}

//插入
void rbtree_insert(rbtree_t* tree, rbtree_node_t* node, void* val)
{
    rbtree_node_t* y = NULL;
    rbtree_node_t* x = tree->root;
    while (x != NULL)
    {
        y = x;
        if (tree->cmp_func(x, val) > 0)
            x = x->left;
        else
            x = x->right;
    }
    _rbtree_node_set_parent(node, y);
    if (y == NULL)
        tree->root = node;
    else if (tree->cmp_func(y, val) > 0)
        y->left = node;
    else
        y->right = node;
    
    node->left = node->right = NULL;
    _rbtree_node_set_red(node);

    _rbtree_insert_fixup(&tree->root, node);
}

//删除：删除节点node(z)，真正删除的节点y，删除后替换的节点x，以及x的父节点x_p
void rbtree_remove(rbtree_t* tree, rbtree_node_t* node)
{
    rbtree_node_t* y, * x, * x_p;
    y = node;
    uint32_t ycolor = _rbtree_node_get_color(y);

    if (node->left == NULL)
    {
        x = node->right;
        x_p = _rbtree_node_get_parent(node);
        _rbtree_transplant(&tree->root, node, node->right);
    }
    else if (node->right == NULL)
    {
        x = node->left;
        x_p = _rbtree_node_get_parent(node);
        _rbtree_transplant(&tree->root, node, node->left);
    }
    else
    {
        y = _rbtree_minimum(node->right);
        ycolor = _rbtree_node_get_color(y);
        x = y->right;
        x_p = _rbtree_node_get_parent(y);

        if (x_p == node)
        {
            node->right = x;
            x_p = y;
        }
        else
        {
            _rbtree_transplant(&tree->root, y, y->right);
            y->right = node->right;
            _rbtree_node_set_parent(y->right, y);
        }
        _rbtree_transplant(&tree->root, node, y);
        y->left = node->left;
        _rbtree_node_set_parent(y->left, y);
        _rbtree_node_set_color(y, _rbtree_node_get_color(node));
    }

    if (ycolor == _RBTREE_NODE_COLOR_BLACK)
        _rbtree_delete_fixup(&tree->root, x, x_p);
}

rbtree_node_t* rbtree_first(rbtree_t* tree)
{
    return _rbtree_minimum(tree->root);
}
rbtree_node_t* rbtree_last(rbtree_t* tree)
{
    return _rbtree_maximum(tree->root);
}
rbtree_node_t* rbtree_next(rbtree_node_t* node)
{
    if (node->right)
        return _rbtree_minimum(node->right);

    rbtree_node_t* parent = _rbtree_node_get_parent(node);
    while (parent && (node == parent->right))
    {
        node = parent;
        parent = _rbtree_node_get_parent(node);
    }

    return parent;
}
rbtree_node_t* rbtree_prev(rbtree_node_t* node)
{
    if (node->left)
        return _rbtree_maximum(node->left);

    rbtree_node_t* parent = _rbtree_node_get_parent(node);
    while (parent && (node == parent->left))
    {
        node = parent;
        parent = _rbtree_node_get_parent(node);
    }

    return parent;
}

rbtree_node_t* rbtree_search(rbtree_t* tree, void* value, rbtree_search_type_t type)
{
    rbtree_node_t* match = NULL;
    rbtree_node_t* temp = tree->root;
    int gap = 0;

    while (temp != NULL)
    {
        int this_gap = tree->cmp_func(temp, value);
        if (this_gap > 0)
        {
            if (type == RBTREE_SEARCH_NOT_SMALL)
            {
                if ((gap == 0) || (this_gap < gap))
                {
                    gap = this_gap;
                    match = temp;
                }
            }
            temp = temp->left;
        }
        else if (this_gap < 0)
        {
            if (type == RBTREE_SEARCH_NOT_LARGE)
            {
                if ((gap == 0) || (this_gap > gap))
                {
                    gap = this_gap;
                    match = temp;
                }
            }
            temp = temp->right;
        }
        else
        {
            match = temp;
            break;
        }
    }

    return match;
}