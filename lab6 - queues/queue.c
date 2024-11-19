#include <stdlib.h>
#include <stdio.h>

#include "../timekeeping.h"

typedef struct node
{
    int value;
    struct node *next;
} node;

typedef struct queue
{
    node *first;
    node *last;
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
    if (q->first)
    {
        return 1;
    }
    return 0;
}

void print_queue(queue *q)
{
    node *nd = q->first;
    while (nd != NULL)
    {
        printf("%d, ", nd->value);
        nd = nd->next;
    }
}

void enqueue(queue *q, int v)
{
    node *nd = (node *)malloc(sizeof(node));
    nd->value = v;
    nd->next = NULL;

    node *nxt = q->first;

    if (q->first == NULL)
    {
        q->first = nd;
        return;
    }

    while (nxt->next != NULL)
    {
        nxt = nxt->next;
    }

    nxt->next = nd;
}

int dequeue(queue *q)
{
    int res = 0;
    if (q->first != NULL)
    {
        res = q->first->value;
        node *tmp = q->first;
        q->first = q->first->next;
        free(tmp);
    }
    return res;
}

void enqueue_improved(queue *q, int v)
{
    node *nd = (node *)malloc(sizeof(node));
    nd->value = v;
    nd->next = NULL;

    if (q->last == NULL)
    {
        q->first = nd;
    }
    else
    {
        q->last->next = nd;
    }
    q->last = nd;
}

void main()
{
    int q_size = 250;
    int times = 10000;
    queue *q = create_queue();
    struct timespec t_start, t_stop;

    // ----- MEASURE TIME TO ENQUEUE -----
    for (int j = 0; j < 6; j++)
    {
        for (int k = 0; k < q_size; k++)
        {
            enqueue(q, k);
        }
        long wall = 0;
        for (int i = 0; i < times; i++)
        {
            clock_gettime(CLOCK_MONOTONIC, &t_start);
            enqueue(q, 55);
            clock_gettime(CLOCK_MONOTONIC, &t_stop);
            wall += nano_seconds(&t_start, &t_stop);
            int tmp = dequeue(q);
        }
        printf("\nSize of queue: %d, Average time to enqueue %d", q_size, wall / times);
        q_size *= 2;
    }

    // // ----- MEASURE TIME TO DEQUEUE -----
    // for (int j = 0; j < 6; j++) {
    //     for(int k = 0; k < q_size; k++) {
    //         enqueue(q, k);
    //     }
    // long wall = 0;
    //     for (int i = 0; i < times; i++) {
    //         enqueue(q, 55);
    //         clock_gettime(CLOCK_MONOTONIC, &t_start);
    //             int tmp = dequeue(q);
    //         clock_gettime(CLOCK_MONOTONIC, &t_stop);
    //         wall += nano_seconds(&t_start, &t_stop);
    //     }
    //     printf("\nSize of queue: %d, Average time to dequeue %d", q_size, wall/times);
    //     q_size *= 2;
    // }

    // // ----- MEASURE TIME TO ENQUEUE IMPROVED -----
    // for (int j = 0; j < 6; j++) {
    //     for(int k = 0; k < q_size; k++) {
    //         enqueue_improved(q, k);
    //     }
    //     long wall = 0;
    //     for (int i = 0; i < times; i++) {
    //         clock_gettime(CLOCK_MONOTONIC, &t_start);
    //             enqueue_improved(q, 55);
    //         clock_gettime(CLOCK_MONOTONIC, &t_stop);
    //         wall += nano_seconds(&t_start, &t_stop);
    //         int tmp = dequeue(q);
    //     }
    //     printf("\nSize of queue: %d, Average time to enqueue improved %d", q_size, wall/times);
    //     q_size *= 2;
    // }
}