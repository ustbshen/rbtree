#include <iostream>
#include <queue>
#include <iomanip>
#include "rbtree.h"

using namespace std;

typedef struct node
{
    rbtree_node_t   rbt_node;
    int             value;
} node_t;

int32_t node_cmp(rbtree_node_t* node, void* value)
{
    node_t* n = (node_t*)node;

    return n->value - *(int*)value;
}

void print_blank(int n)
{
    while (n--)
    {
        cout << "    ";
    }
}

void print_value(node_t* n)
{
    if ((uint32_t)n->rbt_node._parent_and_color & 0x01)
    {
        //red
        cout << '*' << setw(3) << n->value;
    }
    else
    {
        cout << setw(4) << n->value;
    }
}

void print_tree(rbtree_t* tree)
{
    cout << "Tree :" << endl;
    if (tree->root == NULL)
    {
        cout << "empty tree" << endl;
        return;
    }

    vector<rbtree_node_t*> layer[6];
    layer[0].push_back(tree->root);
    bool to_leaf = false;
    int curr_layer = 0;
    while (!to_leaf && curr_layer < 4)
    {
        to_leaf = true;
        for (int i = 0; i < layer[curr_layer].size(); i++)
        {
            if (layer[curr_layer][i] != NULL)
            {
                layer[curr_layer + 1].push_back(layer[curr_layer][i]->left);
                layer[curr_layer + 1].push_back(layer[curr_layer][i]->right);
                if (layer[curr_layer][i]->left || layer[curr_layer][i]->right)
                    to_leaf = false;
            }
            else
            {
                layer[curr_layer + 1].push_back(NULL);
                layer[curr_layer + 1].push_back(NULL);
            }
        }
        curr_layer++;
    }
    int total_line = curr_layer;

    //Êä³ö
    for (int line = 0; line <= total_line; line++)
    {
        int begin_blank = pow(2, total_line - line) - 1;
        int middle_blank = 2 * begin_blank + 1;
        print_blank(begin_blank);
        for (int i = 0; i < layer[line].size(); i++)
        {
            node_t* n = (node_t*)layer[line][i];
            if (n)
                print_value(n);
            else
                print_blank(1);
            print_blank(middle_blank);
        }
        cout << endl;
    }

    cout << endl;
}

int main()
{
    rbtree_t tree;
    //node_t s, e, a, r, c, h, x, m, p, l;

    rbtree_init(&tree, node_cmp);

    while (1)
    {
        char act;
        int value;

        cout << "input action: i/r value:";
        cin >> act >> value;
        if (act == 'i')
        {
            if (value < 0 || value > 999)
            {
                cout << "invalid value" << endl;
                continue;
            }
            node_t* ins = new node();
            ins->value = value;
            rbtree_insert(&tree, &ins->rbt_node, &ins->value);
            print_tree(&tree);
            cout << endl;
        }
        else if (act == 'r')
        {
            if (value < 0 || value > 999)
            {
                cout << "invalid value" << endl;
                continue;
            }

            rbtree_node_t* d = rbtree_search(&tree, &value, RBTREE_SEARCH_NOT_SMALL);
            if (d)
            {
                node_t* n = (node_t*)d;
                cout << "found: " << n->value << endl;
                rbtree_remove(&tree, d);
                delete n;
                print_tree(&tree);
            }
            else
            {
                cout << "not found" << endl;
            }
        }
    }
    /*
    s.value = 80;
    e.value = 20;
    a.value = 10;
    r.value = 70;
    c.value = 15;
    h.value = 30;
    x.value = 90;
    m.value = 50;
    p.value = 60;
    l.value = 40;

    
    rbtree_insert(&tree, &s.rbt_node, &s.value);
    print_tree(&tree);
    rbtree_insert(&tree, &e.rbt_node, &e.value);
    print_tree(&tree);
    rbtree_insert(&tree, &a.rbt_node, &a.value);
    print_tree(&tree);
    rbtree_insert(&tree, &r.rbt_node, &r.value);
    print_tree(&tree);
    rbtree_insert(&tree, &c.rbt_node, &c.value);
    print_tree(&tree);
    rbtree_insert(&tree, &h.rbt_node, &h.value);
    print_tree(&tree);
    rbtree_insert(&tree, &x.rbt_node, &x.value);
    print_tree(&tree);
    rbtree_insert(&tree, &m.rbt_node, &m.value);
    print_tree(&tree);
    rbtree_insert(&tree, &p.rbt_node, &p.value);
    print_tree(&tree);
    rbtree_insert(&tree, &l.rbt_node, &l.value);
    print_tree(&tree);
    */
}