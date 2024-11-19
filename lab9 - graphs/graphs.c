#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../timekeeping.h"

#define BUFFER 200
#define MOD 80

typedef struct connection
{
    int time;
    struct city *dst;
    struct connection *next;
} connection;

typedef struct city
{
    char *city;
    connection *connections;
} city;

typedef struct hashmap
{
    city **city_array;
    int size;
} hashmap;

void print_connections(city *cit)
{
    if (cit == NULL)
        printf("\n(NULL)");
    else
    {
        printf("\nCITY: %s, CONNECTING TO:", cit->city);
        connection *t = cit->connections;
        if (t != NULL)
        {
            while (t->next != NULL)
            {
                printf("\n\t %s (%d)", t->dst->city, t->time);
                t = t->next;
            }
            printf("\n\t %s (%d)", t->dst->city, t->time);
        }
    }
}

void print_hashmap(hashmap *hm)
{
    if (hm == NULL)
        return;

    for (int i = 0; i < hm->size; i++)
        print_connections(hm->city_array[i]);
}

void print_path(city **path) {
    int i = 0;
    while (path[i] != NULL) {
        printf("%s -> ", path[i]->city);
        i++;
    }
    return;
}

int hash(char *name, int mod)
{
    int h = 0;
    int i = 0;
    unsigned char c = 0;
    while ((c = name[i]) != 0)
    {
        h = (h * 31 + c) % mod;
        i++;
    }
    return h;
}

city *create_city(char *name)
{
    city *new_city = (city *)malloc(sizeof(city));
    new_city->city = name;
    new_city->connections = NULL;
    return new_city;
}

connection *create_connection(city *src, city *dst, int time)
{
    connection *new_con = (connection *)malloc(sizeof(connection));
    new_con->dst = dst;
    new_con->time = time;
    new_con->next = src->connections;
    src->connections = new_con;
    return src->connections;
}

city **create_path()
{
    city **path = (city **)malloc(sizeof(city *) * MOD);
    for (int i = 0; i < MOD; i++) {
        path[i] = NULL;
    }
    return path;
}

void bidirectional_connect(city *city1, city *city2, int time)
{
    create_connection(city1, city2, time);
    create_connection(city2, city1, time);
}

city *lookup(hashmap *cities, char *lookfor)
{
    int mod = cities->size;
    int index = hash(lookfor, mod);

    while (cities->city_array[index] != NULL)
    {
        if (strcmp(cities->city_array[index]->city, lookfor) == 0)
        {
            return cities->city_array[index];
        }
        else
        {
            index = (index + 1) % mod;
        }
    }
    city *new_city = create_city(lookfor);
    cities->city_array[index] = new_city;

    return new_city;
}

hashmap *graph(char *file, int mod)
{
    city **cities = (city **)malloc(sizeof(city) * mod);
    hashmap *trains = (hashmap *)malloc(sizeof(hashmap));
    trains->size = mod;

    trains->city_array = cities;
    for (int i = 0; i < mod; i++)
    {
        trains->city_array[i] = NULL;
    }

    // Open the file in read mode
    FILE *fptr = fopen(file, "r");
    if (fptr == NULL)
    {
        return NULL;
    }

    char *lineptr = malloc(sizeof(char) * BUFFER);
    size_t n = BUFFER;

    while (fgets(lineptr, n, fptr) > 0)
    {
        char *copy = (char *)malloc(sizeof(char) * (strlen(lineptr) + 1));
        strcpy(copy, lineptr);
        city *src = lookup(trains, strtok(copy, ","));
        city *dst = lookup(trains, strtok(NULL, ","));
        int dist = atoi(strtok(NULL, ","));

        bidirectional_connect(src, dst, dist);
    }
    fclose(fptr);
    return trains;
}

int shortest(city *from, city *to, int left)
{
    if (from == to)
    {
        return 0;
    }
    int sofar = -1;
    connection *nxt = from->connections;
    while (nxt != NULL)
    {
        if (nxt->time <= left)
        {
            int d = shortest(nxt->dst, to, left - nxt->time);
            if (d >= 0 && ((sofar == -1) || (d + nxt->time) < sofar))
                sofar = (d + nxt->time);
        }
        nxt = nxt->next;
    }
    return sofar;
}

int loop(city **path, int k, city *dst)
{
    int i = 0;
    while (path[i] != NULL)
    {
        int cmp = strcmp(path[i]->city, dst->city);
        if (cmp == 0)
            return 1;
        i++;
    }
    path[i] = dst;
    return 0;
}

int shortest_path(city *from, city *to, city **path, int k)
{
    if (from == to)
    {
        return 0;
    }

    if (path == NULL)
    {
        path = create_path();
    }

    int sofar = -1;
    connection *nxt = from->connections;
    while (nxt != NULL)
    {
        if (!loop(path, k, nxt->dst))
        {
            // Copy path
            city **new_path = create_path();
            for (int i = 0; path[i] != NULL; i++)
            {
                new_path[i] = path[i];
            }

            int d = shortest_path(nxt->dst, to, new_path, k+1);

            if (d >= 0 && ((sofar == -1) || (d + nxt->time) < sofar))
            {
                sofar = (d + nxt->time);
            }
        }
        nxt = nxt->next;
    }
    return sofar;
}


// int main()
// {
//     // ---------- TEST CITY & CONNECTION ----------
//     // city *city1 = create_city("Stockholm");
//     // city *city2 = create_city("Södertälje");
//     // city *city3 = create_city("Katrineholm");

//     // printf("\nName: %s", city2->city);

//     // bidirectional_connect(city1, city2, 21);

//     // printf("\nConnections: %s", city2->connections->dst->city);

//     // ---------- TEST READING FILE ----------
//     int mod = 80;
//     hashmap *trains = graph("trains.csv", mod);
//     print_hashmap(trains);
// }

// ---------- TEST NAIVE MAIN ----------

int main()
{
    char *src = "Köpenhamn";
    char *dst = "Malmö";
    int limit = 1000;
    hashmap *trains = graph("europe.csv", MOD);
    city *from = lookup(trains, src);
    city *to = lookup(trains, dst);
    // print_hashmap(trains);

    // int limit = atoi(max_time);
    struct timespec t_start, t_stop;
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    int s = shortest(from, to, limit);
    // int s = shortest_path(from, to, NULL, 0);
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop);
    if (s >= 0)
        printf("\nShortest path %d found in %.2fms\n", s, ((double)wall) / 1000000);
    else
        printf("no path found\n");
}