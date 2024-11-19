#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

long nano_seconds(struct timespec *t_start, struct timespec *t_stop)
{
    return (t_stop->tv_nsec - t_start->tv_nsec) +
           (t_stop->tv_sec - t_start->tv_sec) * 1000000000;
}

// RANDOM ACCESS PART 1
int main()
{
    struct timespec t_start, t_stop;
    for (int i = 0; i < 10; i++)
    {
        clock_gettime(CLOCK_MONOTONIC, &t_start);
        clock_gettime(CLOCK_MONOTONIC, &t_stop);
        long wall = nano_seconds(&t_start, &t_stop);
        printf("%ld ns\n", wall);
    }
}

// RANDOM ACCESS PART 2
int main()
{
    struct timespec t_start, t_stop;
    int given[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    int sum = 0;
    for (int i = 0; i < 10; i++)
    {
        clock_gettime(CLOCK_MONOTONIC, &t_start);
        sum += given[i];
        clock_gettime(CLOCK_MONOTONIC, &t_stop);
        long wall = nano_seconds(&t_start, &t_stop);
        printf("one operation in %ld ns\n", wall);
    }
}

// RANDOM ACCESS PART 3
int main()
{
    struct timespec t_start, t_stop;
    int array[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int sum = 0;
    for (int i = 0; i < 10; i++)
    {
        clock_gettime(CLOCK_MONOTONIC, &t_start);
        sum += array[rand() % 10];
        clock_gettime(CLOCK_MONOTONIC, &t_stop);
        long wall = nano_seconds(&t_start, &t_stop);
        printf("%ld ns\n", wall);
    }
}

long bench(int n, int loop)
{
    struct timespec t_start, t_stop;

    int *array = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        array[i] = i;
    int *indx = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < loop; i++)
        indx[i] = rand() % n;

    int sum = 0;
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    for (int i = 0; i < loop; i++)
        sum += array[indx[i]];
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    if (sum == 0)
        return 0;
    long wall = nano_seconds(&t_start, &t_stop);
    return wall;

    // int sum = 0;
    // clock_gettime(CLOCK_MONOTONIC, &t_start);
    // for (int i = 0; i < loop; i++)
    //     sum += array[indx[i]];
    // clock_gettime(CLOCK_MONOTONIC, &t_stop);
}


// SEARCH
int main()
{
    int n = 10;
    int loop = 10;
    for (int i = 0; i < 10; i++)
    {
        long wall = bench(n, loop);
        printf("time : %ld ns\n", wall);
    }
}

long search(int n, int loop)
{
    struct timespec t_start, t_stop;

    int *array = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        array[i] = rand() % (n * 2);
    int *keys = (int *)malloc(loop * sizeof(int));
    for (int i = 0; i < loop; i++)
        keys[i] = rand() % (n * 2);
    int sum = 0;
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    for (int i = 0; i < loop; i++)
    {
        int key = keys[i];
        for (int j = 0; j < n; j++)
        {
            if (key == array[j])
            {
                sum++;
                break;
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop);
    return wall;
}


// DUPLICATES
long duplicates(int n)
{
    struct timespec t_start, t_stop;
    int loop = 10;
    int keys[1000];
    int *array_a = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        array_a[i] = rand() % (n * 2);
    int *array_b = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < loop; i++)
        keys[i] = rand() % (n * 2);
    int sum = 0;
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    for (int i = 0; i < loop; i++)
    {
        int key = array_a[i];
        for (int j = 0; j < n; j++)
        {
            if (key == array_b[j])
            {
                sum++;
                break;
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop);
    return wall;
}

int main()
{
    int n = 100000;
    for (int i = 0; i < 10; i++)
    {
        long wall = duplicates(n);
        printf("time : %ld ns\n", wall);
    }
}

// int main(int argc, char *argv[])
// {
//     int LONG_MAX = 2147483647;
//     int sizes[] = {1000, 2000, 4000, 8000, 16000, 32000};
//     int k = 10;
//     int loop = 1000;
//     for (int n = 1000; n <= 32000; n *= 2)
//     {
//         long min = LONG_MAX;
//         for (int i = 0; i < k; i++)
//         {
//             long wall = bench(n, loop);
//             if (wall < min)
//                 min = wall;
//         }
//         printf("%d %0.2f ns\n", n, (double)min / loop);
//     }
// }