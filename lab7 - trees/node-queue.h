#include <stdlib.h>
#include <stdio.h>

// #include "tree-struct.h"

typedef struct node
{
    int value;
    struct node *right;
    struct node *left;
} node;

typedef struct tree
{
    node *root;
} tree;

typedef struct q_node
{
    node *value;
    struct q_node *next;
} q_node;

typedef struct queue
{
    q_node *first;
    q_node *last;
} queue;

queue *create_queue()
{
    queue *q = (queue *)malloc(sizeof(queue));
    q->first = NULL;
    q->last = NULL;
    return q;
}

int empty(queue *q)
{
    if (q->first) {
        return 1;
    } 
    return 0;
}

void print_queue(queue *q)
{
    q_node *nd = q->first;
    while (nd != NULL)
    {
        printf("%d, ", nd->value);
        nd = nd->next;
    }
}

// void enqueue(queue *q, node *v)
// {
//     q_node *nd = (q_node*)malloc(sizeof(q_node));
//     nd->value = v; 
//     nd->next = NULL;

//     q_node *nxt = q->first;

//     if (q->first == NULL) {
//         q->first = nd;
//         return;
//     } 

//     while (nxt->next != NULL)
//     {
//         nxt = nxt->next;
//     }

//     nxt->next = nd;
// }

node* dequeue(queue *q)
{
    node *res = 0;
    if (q->first != NULL)
    {
        res = q->first->value;
        q_node *tmp = q->first;
        if (q->first == q->last) {
            q->last = NULL;
        }
        q->first = q->first->next;
        free(tmp);
    }
    return res;
}

void enqueue_improved(queue *q, node *v)
{
    q_node *nd = (q_node*)malloc(sizeof(q_node));
    nd->value = v; 
    nd->next = NULL;

    if (q->last == NULL) {
        q->first = nd;
    } else {
        q->last->next = nd;
    }
    q->last = nd;
}