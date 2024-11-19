#include <stdlib.h>
#include <stdio.h>

#include "../timekeeping.h"
#include "node-queue.h"

tree *construct_tree()
{
    tree *tr = (tree *)malloc(sizeof(tree));
    tr->root = NULL;
    return tr;
}

node *construct_node(int val)
{
    node *nd = (node *)malloc(sizeof(node));
    nd->value = val;
    nd->left = NULL;
    nd->right = NULL;
    return nd;
}

void free_node(node *nd)
{
    if (nd != NULL)
    {
        free_node(nd->left);
        free_node(nd->right);
        free(nd);
    }
}

void free_tree(tree *tr)
{
    free(tr->root);
}

void add(tree *tr, int value)
{
    node *current_node = tr->root;
    node *prev_node = NULL;

    if (current_node == NULL)
    {
        tr->root = construct_node(value);
        return;
    }

    while (current_node != NULL)
    {
        if (current_node->value > value)
        {
            prev_node = current_node;
            current_node = current_node->left;
            continue;
        }
        if (current_node->value < value)
        {
            prev_node = current_node;
            current_node = current_node->right;
            continue;
        }
        if (current_node->value == value)
        {
            return;
        }
    }

    if (prev_node->value > value)
    {
        prev_node->left = construct_node(value);
        return;
    }
    if (prev_node->value < value)
    {
        prev_node->right = construct_node(value);
        return;
    }
}

int lookup(node *nd, int value)
{
    if (nd == NULL)
    {
        return 0;
    }
    if (nd->value == value)
    {
        return 1;
    }
    if (nd->value > value)
    {
        return lookup(nd->left, value);
    }
    if (nd->value < value)
    {
        return lookup(nd->right, value);
    }
}

void add_recursive(tree *tr, int value, node *current_node)
{
    if (current_node == NULL)
    {
        tr->root = construct_node(value);
        return;
    }

    if (current_node->value < value)
    {
        if (current_node->right != NULL)
            add_recursive(tr, value, current_node->right);
        else
        {
            current_node->right = construct_node(value);
            return;
        }
    }

    else if (current_node->value > value)
    {
        if (current_node->left != NULL)
            add_recursive(tr, value, current_node->left);
        else
        {
            current_node->left = construct_node(value);
            return;
        }
    }
    else
    {
        return;
    }
}

static void print_node(node *nd)
{
    if (nd != NULL)
    {
        print_node(nd->left);
        printf("%d ", nd->value);
        print_node(nd->right);
    }
}

void print_tree(tree *tr)
{
    if (tr->root != NULL)
        print_node(tr->root);
    printf("\n");
}

int breadth_first_print(tree *tr)
{
    queue *q = create_queue();

    if (tr->root != NULL)
    {
        node *nd = tr->root;
        enqueue_improved(q, nd);

        while (q->first != NULL)
        {
            nd = dequeue(q);
            printf("%d, ", nd->value);
            if (nd->left != NULL)
                enqueue_improved(q, nd->left);
            if (nd->right != NULL)
                enqueue_improved(q, nd->right);
        }
    }
}

// ########## LAZY SEQUENCE ##########

typedef struct sequence
{
    queue *queue;
} sequence;

sequence *create_sequence(tree *tr)
{
    if (tr->root == NULL)
        return NULL;

    sequence *seq = (sequence *)malloc(sizeof(sequence));
    queue *new = create_queue();
    seq->queue = new;
    enqueue_improved(new, tr->root);
    return seq;
}

int next(sequence *seq)
{
    node *nd = dequeue(seq->queue);
    if (nd->left != NULL)
        enqueue_improved(seq->queue, nd->left);
    if (nd->right != NULL)
        enqueue_improved(seq->queue, nd->right);
    return nd->value;
}

// ########## LAZY SEQUENCE END ##########

void main()
{
    int data_size = 1000;
    tree *tr = construct_tree();
    tr->root = construct_node(data_size * 3);
    struct timespec t_start, t_stop;
    long totaltime = 0;

    for (int i = 0; i < data_size; i++)
    {
        int val = rand() % data_size * 6;
        clock_gettime(CLOCK_MONOTONIC, &t_start);
        add(tr, val);
        // add_recursive(tr, val, tr->root);
        clock_gettime(CLOCK_MONOTONIC, &t_stop);
        totaltime += nano_seconds(&t_start, &t_stop);
    }

    // printf("\nTime to add (%d) values: %d ns\n\n", data_size, totaltime);
    // printf("\nTime to add_recursive (%d) values: %d ns\n\n", data_size, totaltime);

    // clock_gettime(CLOCK_MONOTONIC, &t_start);
    // printf("%d, ", lookup(tr->root, 500));
    // clock_gettime(CLOCK_MONOTONIC, &t_stop);

    // printf("\nTime to lookup (%d) values: %d ns\n\n", data_size, totaltime);

    // breadth_first_print(tr);

    // sequence *seq = create_sequence(tr);
    // printf("%d, ", next(seq));
}