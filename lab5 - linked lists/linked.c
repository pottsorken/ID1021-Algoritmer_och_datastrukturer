#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

#include "../timekeeping.h"

typedef struct cell
{
    int value;
    struct cell *tail;
} cell;

typedef struct linked
{
    cell *first;
} linked;

linked *create_linked()
{
    linked *new = (linked *)malloc(sizeof(linked));
    new->first = NULL;
    return new;
}

void free_linked(linked *lnk)
{
    cell *nxt = lnk->first;
    while (nxt != NULL)
    {
        cell *tmp = nxt->tail;
        free(nxt);
        nxt = tmp;
    }
    free(lnk);
}

void print_list(linked *lnk)
{
    cell *nxt = lnk->first;
    while (nxt != NULL)
    {
        printf("%d, ", nxt->value);
        nxt = nxt->tail;
    }
}

void add(linked *lnk, int item)
{
    cell *new = (cell *)malloc(sizeof(cell));
    new->value = item;
    new->tail = lnk->first;
    lnk->first = new;
}

bool find(linked *lnk, int item)
{
    cell *nxt = lnk->first;
    while (nxt != NULL)
    {
        if (nxt->value == item)
        {
            return true;
        }
        nxt = nxt->tail;
    }
    return false;
}

int length(linked *lnk)
{
    int count = 0;
    cell *nxt = lnk->first;
    while (nxt != NULL)
    {
        count++;
        nxt = nxt->tail;
    }
    return count;
}

void remove_item(linked *lnk, int item)
{
    cell *prv = lnk->first;
    // item in first position
    if (prv->value == item)
    {
        if (prv->tail == NULL)
        {
            lnk->first == NULL;
            free(prv);
        }
        else
        {
            lnk->first = prv->tail;
        }
    }

    cell *nxt = prv->tail;
    for (int i = 0; i < length(lnk); i++)
    {
        if (nxt->value == item)
        {
            prv->tail = nxt->tail;
            free(nxt);
        }
        if (nxt->value == item && nxt->tail == NULL)
        {
            prv->tail = NULL;
            free(nxt);
        }
        prv = nxt;
        nxt = nxt->tail;
    }
}

void append(linked *a, linked *b)
{
    cell *nxt = a->first;
    while (nxt->tail != NULL)
    {
        nxt = nxt->tail;
    }
    nxt->tail = b->first;
}

void main()
{
    int lnk1_len = 4000;
    int lnk2_len = 1000; // constant length
    struct timespec t_start, t_stop;

    linked *lnk1 = create_linked();
    linked *lnk2 = create_linked();

    for (int i = 0; i < lnk1_len; i++)
    {
        add(lnk1, i);
    }

    for (int i = 0; i < lnk2_len; i++)
    {
        add(lnk2, i + lnk2_len + 10);
    }

    // printf("LIST 1: ");
    // print_list(lnk1);
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    append(lnk1, lnk2);
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop);

    // printf("\nLIST NEW: ");
    // print_list(lnk1);

    printf("\nIt took %d ns to append a list of size %d to a list of size %d.", wall, lnk2_len, lnk1_len);

    // print_list(lnk);
    // printf("\nFind: %d", find(lnk, 7));
    // printf("\nFind: %d", find(lnk, 4));
    // printf("\nLength: %d", length(lnk));
    // remove_item(lnk, 3);
    // printf("\n-----------\n");
    // print_list(lnk);
    // printf("\nFind: %d", find(lnk, 4));
    // printf("\nLength: %d", length(lnk));
}