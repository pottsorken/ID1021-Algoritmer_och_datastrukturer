#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>

#include "../timekeeping.h"

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

int *scramble(int *input, int length, int times)
{
    for (int i = 0; i < times; i++)
    {
        int first = rand() % length;
        int second = rand() % length;

        int temp = input[first];
        input[first] = input[second];
        input[second] = temp;
    }
}

bool unsorted_search(int *array, int length, int key)
{
    for (int index = 0; index < length; index++)
    {
        if (array[index] == key)
        {
            return true;
        }
    }
    return false;
}

bool binary_search(int *array, int length, int key)
{
    int first = 0;
    int last = length - 1;
    while (true)
    {
        int index = (last + first) / 2;
        if (array[index] == key)
        {
            return true;
        }
        if (array[index] < key && index < last)
        {
            first = index + 1;
            continue;
        }
        if (array[index] > key && index > first)
        {
            last = index - 1;
            continue;
        }
        if (index == first || index == last)
        {
            return false;
        }
    }
}

bool recursive(int *array, int length, int key, int first, int last)
{
    while (true)
    {
        // jump to the middle
        int index = (last + first) / 2;
        if (array[index] == key)
        {
            return true;
        }
        if (array[index] < key && index < last)
        {
            // call recursive but narrow the search
            recursive(array, length / 2, key, index + 1, last);
        }
        if (array[index] > key && index > first)
        {
            // call recursive but narrow the search
            recursive(array, length / 2, key, first, index - 1);
        }
        // Why do we land here? What should we do?
        if (index == first || index == last)
        {
            return false;
        }
    }
}

int main()
{
    srand(time(NULL));
    int length = 320000;
    int *array = create_array(length);
    int key = rand() % length * 5;
    struct timespec t_start, t_stop;

    // ---- LINEAR SEARCH ----
    printf("Length: %d  Key: %d\n", length, key);
    printf("--------------------\n");
    int result1;
    int result2;
    long time1 = 0;
    long time2 = 0;

    for (int i = 0; i < 1000; i++)
    {
        clock_gettime(CLOCK_MONOTONIC, &t_start);
        result1 = unsorted_search(array, length, key);
        clock_gettime(CLOCK_MONOTONIC, &t_stop);
        long temp1 = nano_seconds(&t_start, &t_stop);
        if (i == 0 || temp1 < time1)
        {
            time1 = temp1;
        }
    }

    printf("SORTED\nResult: %d\nTime: %d ns\n\n", result1, time1);

    int *scrambled_array = scramble(array, length, length);

    for (int j = 0; j < 1000; j++)
    {
        clock_gettime(CLOCK_MONOTONIC, &t_start);
        result2 = unsorted_search(array, length, key);
        clock_gettime(CLOCK_MONOTONIC, &t_stop);
        long temp2 = nano_seconds(&t_start, &t_stop);
        if (j == 0 || temp2 < time2)
        {

            time2 = temp2;
        }
    }
    printf("SCRAMBLED\nResult: %d\nTime: %d ns\n\n", result2, time2);

    // ---- BINARY SEARCH ----
    int result3;
    long time3 = 0;

    for (int i = 0; i < 100; i++)
    {
        clock_gettime(CLOCK_MONOTONIC, &t_start);

        for (int j = 0; j < 1000; j++)
        {
            result3 = binary_search(array, length, key);
        }

        clock_gettime(CLOCK_MONOTONIC, &t_stop);
        long temp3 = nano_seconds(&t_start, &t_stop);
        if (i == 0 || temp3 < time3)
        {
            time3 = temp3;
        }
    }
    printf("BINARY\nResult: %d\nTime: %d ns\n\n", result3, time3);

    // ---- RECURSIVE BINARY SEARCH ----
    int result4;
    long time4 = 0;

    for (int i = 0; i < 100; i++)
    {
        clock_gettime(CLOCK_MONOTONIC, &t_start);

        for (int j = 0; j < 1000; j++)
        {
            result4 = binary_search(array, length, key);
        }

        clock_gettime(CLOCK_MONOTONIC, &t_stop);
        long temp4 = nano_seconds(&t_start, &t_stop);
        if (i == 0 || temp4 < time4)
        {
            time4 = temp4;
        }
    }
    printf("RECURSIVE BINARY\nResult: %d\nTime: %d ns\n\n", result4, time4);

    free(array);
}