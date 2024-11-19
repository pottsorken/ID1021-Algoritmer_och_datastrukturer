#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

#include "../timekeeping.h"

void swap(int *array, int first_index, int second_index)
{
    int tmp = array[first_index];
    array[first_index] = array[second_index];
    array[second_index] = tmp;
}

int *create_array(int length)
{
    int *array = (int *)malloc(length * sizeof(int));
    int next = 0;
    for (int i = 0; i < length; i++)
    {
        next += rand() % 10 + 1;
        array[i] = next;
    }
    return array;
}

void scramble(int *input, int length, int times)
{
    for (int i = 0; i < times; i++)
    {
        int first = rand() % length;
        int second = rand() % length;
        swap(input, first, second);
    }
}

int check_sort(int *array, int length)
{
    for (int i = 0; i < length - 1; i++)
    {
        if (array[i] > array[i + 1])
        {
            printf("\n\nSort failed\n\n");
            return 0;
        }
    }
    printf("\n\nSort successful!\n\n");
    return 1;
}

// SELECTION SORT
void selection_sort(int *array, int length)
{
    for (int i = 0; i < length - 1; i++)
    {
        int cand = i;
        for (int j = i; j < length; j++)
        {
            if (array[j] < array[cand])
            {
                cand = j;
            }
        }
        swap(array, i, cand);
    }
}

// INSERTION SORT
void insertion_sort(int *array, int length)
{
    for (int i = 1; i < length; i++)
    {
        for (int j = i; j > 0 && array[j] < array[j - 1]; j--)
        {
            swap(array, j, j - 1);
        }
    }
}

// MERGE SORT
void merge(int *array, int *temp_array, int low, int mid, int high)
{
    for (int i = low; i < high + 1; i++)
    {
        temp_array[i] = array[i];
    }

    int index1 = low;
    int index2 = mid + 1;

    for (int k = low; k <= high; k++)
    {
        if (index1 == mid + 1)
        {
            array[k] = temp_array[index2];
            index2++;
        }
        else if (index2 == high + 1)
        {
            array[k] = temp_array[index1];
            index1++;
        }
        else if (temp_array[index1] < temp_array[index2])
        {
            array[k] = temp_array[index1];
            index1++;
        }
        else if (temp_array[index1] > temp_array[index2])
        {
            array[k] = temp_array[index2];
            index2++;
        }
    }
}

void merge_sort(int *array, int *temp_array, int low, int high)
{
    if (low != high)
    {
        int mid = (low + high) / 2;
        merge_sort(array, temp_array, low, mid);
        merge_sort(array, temp_array, mid + 1, high);
        merge(array, temp_array, low, mid, high);
    }
}

void main()
{
    srand(time(NULL));
    int length = 1000;
    int *array = create_array(length);
    struct timespec t_start, t_stop;
    long long time;

    scramble(array, length, length);

    for (int j = 0; j < 6; j++)
    {
        for (int i = 0; i < 500; i++)
        {
            scramble(array, length, length);
            clock_gettime(CLOCK_MONOTONIC, &t_start);
            // selection_sort(array, length);
            // insertion_sort(array, length);
            int *temp_array = (int *)malloc(length * sizeof(int));
            merge_sort(array, temp_array, 0, length - 1);
            clock_gettime(CLOCK_MONOTONIC, &t_stop);

            long long temp = nano_seconds(&t_start, &t_stop);
            if (i == 0 || temp < time)
            {
                time = temp;
            }
        }

        printf("%d  |  %d\n", length, time);
        // check_sort(array, length);
        length = length * 2;
        array = create_array(length);
        scramble(array, length, length);
    }
}