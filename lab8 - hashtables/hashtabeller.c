#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../timekeeping.h"

#define AREAS 10000
#define AREASX 100000
#define BUFFER 200

typedef struct area
{
    char *zip;
    int zipInt;
    char *name;
    int pop;
} area;

typedef struct codes
{
    area *areas;
    int n;
} codes;

typedef struct bucket
{
    area **areas;
    int size;
} bucket;

typedef struct hashmap
{
    bucket **buckets;
    int size;
} hashmap;

codes *read_postcodes(char *file)
{
    codes *postnr = (codes *)malloc(sizeof(codes));
    area *areas = (area *)malloc(sizeof(area) * AREAS);
    // Open a file in read mode
    FILE *fptr = fopen(file, "r");

    if (fptr == NULL)
    {
        printf("\nError opening file\n");
        return NULL;
    }

    int k = 0;
    char *lineptr = malloc(sizeof(char) * BUFFER);
    size_t n = BUFFER;

    while ((k < AREAS) && (fgets(lineptr, n, fptr) > 0))
    {
        char *copy = (char *)malloc(sizeof(char) * n);
        strcpy(copy, lineptr);
        if (n != BUFFER)
        {
            // the line was longer than BUFFER
            free(lineptr);
            n = BUFFER;
        }
        // divide the copy into three strings
        area a;
        a.zip = strtok(copy, ",");
        a.zipInt = atoi(a.zip) * 100 + atoi(a.zip + 3);
        a.name = strtok(NULL, ",");
        a.pop = atoi(strtok(NULL, ","));
        // update the array of areas
        areas[k++] = a;
    }
    fclose(fptr);
    postnr->areas = areas;
    postnr->n = k;
    return postnr;
}

// ########## Lookup using string compare ##########
int linear_lookup(codes *c, char *val)
{
    for (int i = 0; i < c->n; i++)
    {
        if (strcmp(val, c->areas[i].zip) == 0)
            return 1;
    }
    return 0;
}

int binary_lookup(codes *c, char *val, int first, int last)
{
    int index = (first + last) / 2;
    int comp = strcmp(val, c->areas[index].zip);

    if (comp == 0)
        return 1;
    if (comp > 0 && index < last)
        return binary_lookup(c, val, index + 1, last);
    if (comp < 0 && index > first)
        return binary_lookup(c, val, first, index - 1);
    if (index == first || index == last)
        return 0;
}
// ########## Lookup using integers ##########
int linear_lookupInt(codes *c, int val)
{
    for (int i = 0; i < c->n; i++)
    {
        if (c->areas[i].zipInt == val)
            return 1;
    }
    return 0;
}

int binary_lookupInt(codes *c, int val, int first, int last)
{
    int index = (first + last) / 2;
    int comp = c->areas[index].zipInt - val;

    if (first == last)
        return 0;
    else if (comp > 0)
        return binary_lookupInt(c, val, first, index);
    else if (comp < 0)
        return binary_lookupInt(c, val, index, last);

    return 1;
}

//     ##############################################
//     ############### Index and Keys ###############
//     ##############################################

codes *read_postcodesX(char *file)
{
    codes *postnr = (codes *)malloc(sizeof(codes));
    area *areas = (area *)malloc(sizeof(area) * AREASX);
    // Open a file in read mode
    FILE *fptr = fopen(file, "r");

    if (fptr == NULL)
    {
        printf("\nError opening file\n");
        return NULL;
    }

    int k = 0;
    char *lineptr = malloc(sizeof(char) * BUFFER);
    size_t n = BUFFER;

    while ((k < AREASX) && (fgets(lineptr, n, fptr) > 0))
    {
        char *copy = (char *)malloc(sizeof(char) * n);
        strcpy(copy, lineptr);
        if (n != BUFFER)
        {
            // the line was longer than BUFFER
            free(lineptr);
            n = BUFFER;
        }
        // divide the copy into three strings

        area a;
        a.zip = strtok(copy, ",");
        a.zipInt = atoi(a.zip) * 100 + atoi(a.zip + 3);
        a.name = strtok(NULL, ",");
        a.pop = atoi(strtok(NULL, ","));
        // update the array of areas
        areas[a.zipInt] = a;
        k++;
    }
    fclose(fptr);
    postnr->areas = areas;
    postnr->n = k;
    return postnr;
}

// ########## Collisions ##########

void collisions(codes *postnr, int mod)
{
    int mx = 20;
    int data[mod];
    int cols[mx + 1];
    for (int i = 0; i < mod; i++)
    {
        data[i] = 0;
    }
    for (int i = 0; i <= mx; i++)
    {
        cols[i] = 0;
    }
    for (int i = 0; i < postnr->n; i++)
    {
        int index = postnr->areas[i].zipInt % mod;
        data[index]++;
    }
    int sum = 0;
    for (int i = 0; i < mod; i++)
    {
        sum += data[i];
        if (data[i] <= mx)
            cols[data[i]]++;
        else cols[mx]++;
    }
    printf("%d (%d) : ", mod, sum);
    for (int i = 0; i <= mx; i++)
    {
        printf("\n| %d: %d |", i, cols[i]);
    }
    printf("\n");
}


// ########## Buckets and Hashmaps ##########

void print_bucket(bucket *b) {
    printf("\n\n----------");
    if (b == NULL) return;
    int size = b->size;

    for (int i = 0; i < size; i++) {
        if (b->areas[i] == NULL) 
            printf("\nNULL");
        else 
            printf("\n%d | %s - population: %d", b->areas[i]->zipInt, b->areas[i]->name, b->areas[i]->pop);
    }
}

void print_hashmap(hashmap *hm) {
    for (int i = 0; i < hm->size; i++) {
        print_bucket(hm->buckets[i]);
    }
}

void free_bucket(bucket *b) {
    if (b != NULL) {
        if(b->areas != NULL) free(b->areas);
        free(b);
    }
}

void free_hashmap(hashmap *hm) {
    for(int i = 0; i < hm->size; i++) 
        free_bucket(hm->buckets[i]);
    
    free(hm);
}

bucket *create_bucket(int size) {
    bucket *new_b = (bucket *)malloc(sizeof(bucket));
    new_b->areas = (area **)malloc(sizeof(area*)*size);
    for (int i = 0; i < size; i++) 
        new_b->areas[i] = NULL;
    new_b->size = size;
    return new_b;
}

bucket *add_to_bucket(area *content, bucket *b) {
    int size = b->size;
    bucket *new_b = create_bucket(size + 1);

    for (int i = 0; i < size; i++) {
        new_b->areas[i] = b->areas[i];
    }
    new_b->areas[size] = content;

    free_bucket(b);
    return new_b;
}

hashmap *create_hashmap(int length) {
    hashmap *new_map = (hashmap *)malloc(sizeof(hashmap));
    new_map->buckets = (bucket **)malloc(sizeof(bucket *) * length);
    for (int i = 0; i < length; i++) {
        new_map->buckets[i] = create_bucket(0);
    }
    new_map->size = length;
    return new_map;
}

hashmap *fill_hashmap(codes *postnr, int mod) {
    hashmap *hm = create_hashmap(mod);
    for (int i = 0; i < postnr->n; i++) {
        int index = postnr->areas[i].zipInt % mod;
        hm->buckets[index] = add_to_bucket(&(postnr->areas[i]), hm->buckets[index]);
    }
    return hm;
}

area *bucket_lookup(hashmap *hm, int lookfor) {
    int mod = hm->size;
    int b_index = lookfor % mod;
    bucket *b_result = hm->buckets[b_index];
    int i = 0;
    while (i < b_result->size) {
        area *cmp_area = b_result->areas[i];
        if (cmp_area->zipInt == lookfor) {
            printf("\nPostcode found");
            return cmp_area;
        }
        i++;
    }
    printf("\nNo postcode found\n");
    return NULL;
}

bucket *create_bucket_array(int size, codes *postnr) {
    bucket *array = (bucket *)malloc(sizeof(bucket)*size);

    for (int i = 0; i < size; i++) {
        array[i].areas = (area **)malloc(sizeof(area *) * size);
        array->areas[i] = NULL;
    }

    array->size = size;

    for (int j = 0; j < postnr->n; j++) {
        int array_index = postnr->areas[j].zipInt % size;

        while (array->areas[array_index] != NULL) {
            array_index = (array_index + 1) % size;
        }

        array->areas[array_index] = &(postnr->areas[j]);

    }
    return array;
}

area *bucket_array_lookup(bucket *b, int lookfor) {
    int mod = b->size;
    int a_index = lookfor % mod;

    int i = 0;
    while (i < mod) {
        if (a_index >= mod)
            a_index = 0;
        area *cmp_area = b->areas[a_index];
        if (cmp_area == NULL) {
            printf("\nNo postcode found\n");
            return NULL;
        }
        if (cmp_area->zipInt == lookfor) {
            printf("\nPostcode found");
            return cmp_area;
        }
        a_index++;
        cmp_area = b->areas[a_index];
        i++;
    }
}

void main()
{
    int mod = 1000;
    // codes* pc = read_postcodes("postnummer.csv");
    codes *pc = read_postcodesX("postnummer.csv");
    struct timespec t_start, t_stop;

        // ”111 15” and ”984 99” 

    clock_gettime(CLOCK_MONOTONIC, &t_start);

    // // ##### LOOKUP STRING #####
    // printf("Was found? %d\n", linear_lookup(pc, "111 15"));
    // printf("Was found? %d\n", binary_lookup(pc, "984 99", 0, 9675));


    // // ##### LOOKUP INTEGER #####
    // printf("Was found? %d\n", linear_lookupInt(pc, 98499));
    // printf("Was found? %d\n", binary_lookupInt(pc, 11115, 0, 9675));


    // // ##### COLLISIONS #####
    collisions(pc, 12345); // Run with read_postcodes (not X)


    // // ##### BUCKET TEST #####
    // bucket *bucket_array = create_bucket(0);
    // for (int i = 0; i < 5;i++)
    //     bucket_array = add_to_bucket(&(pc->areas[i]), bucket_array);
        

    // // ##### HASHMAP TEST #####
    // hashmap *hm = fill_hashmap(pc, mod);
    // // print_hashmap(hm);
    // area *result = bucket_lookup(hm, 11051);
    // printf("\n%d - %s - population: %d", result->zipInt, result->name, result->pop);

    // bucket *bucket_array = create_bucket_array(11000, pc);
    // // print_bucket(bucket_array);
    // area *result = bucket_array_lookup(bucket_array, 11232);
    // if (result != NULL)
    //     printf("\n%d - %s - population: %d", result->zipInt, result->name, result->pop);


    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long time = nano_seconds(&t_start, &t_stop);

    printf("\nTime: %d ns", time);
}
