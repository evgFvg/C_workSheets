/********************************************
*	Developer:	Evgenii F.
*	Reviewer:       Harel S.					
*	Date:		16/04/2023												
********************************************/
#include <assert.h>
#include <stdlib.h> /* malloc*/
#include <math.h>   /*sqrt*/

#include "hash.h"
#include "dll.h"

#define STANDARD_DEV_POW (2)

struct hash_table
{
    dll_t **hash_table_array;
    size_t capacity;
    size_t (*hash_func)(const void *value);
    int (*is_match_func)(const void *lhd, const void *rhd);
};

static size_t GetDllIndex(const hash_table_t *table, const void *data);

/******************************************User Functions**********************************/


hash_table_t *HashTableCreate(size_t capacity, size_t (*hash_func)(const void *value), int (*is_match_func)(const void *lhd, const void *rhd))
{
    hash_table_t *h_table = NULL;

    assert(NULL != hash_func);
    assert(NULL != is_match_func);

    h_table = (hash_table_t *)malloc(sizeof(hash_table_t));

    if (NULL == h_table)
    {
        return NULL;
    }

    h_table->capacity = capacity;
    h_table->hash_func = hash_func;
    h_table->is_match_func = is_match_func;
    h_table->hash_table_array = (dll_t **)calloc(capacity, sizeof(dll_t *));

    if (NULL == h_table->hash_table_array)
    {
        free(h_table);
        return NULL;
    }

    return h_table;
}


void HashTableDestroy(hash_table_t *table)
{
    size_t i = 0;

    assert(NULL != table);

    for (i = 0; i < table->capacity; ++i)
    {
        if (NULL == table->hash_table_array[i])
        {
            continue;
        }
        else
        {
            DLLDestroy(table->hash_table_array[i]);
        }
    }

    free(table->hash_table_array);
    free(table);
}

int HashTableInsert(hash_table_t *table, void *data)
{
    size_t index = 0;
    iterator_t dll_iter = {0};
    dll_t *curr_dll = NULL;

    assert(NULL != table);

    index = GetDllIndex(table, data);

    if (NULL == table->hash_table_array[index])
    {
        table->hash_table_array[index] = DLLCreate();
    }

    curr_dll = table->hash_table_array[index];

    dll_iter = DLLPushFront(curr_dll, data);

    return 1 == DLLIsSameIter(dll_iter, DLLEndIter(curr_dll));
}

void *HashTableFind(const hash_table_t *table, const void *data)
{
    iterator_t match = {0};
    dll_t *curr_dll = NULL;
    size_t index = 0;
    void *res = NULL;

    assert(NULL != table);

    index = GetDllIndex(table, data);

    if (NULL == table->hash_table_array[index])
    {
        return NULL;
    }

    curr_dll = table->hash_table_array[index];
    match = DLLFind(DLLBeginIter(curr_dll), DLLEndIter(curr_dll), table->is_match_func, (void *)data);

    if(1 != DLLIsSameIter(match, DLLEndIter(curr_dll)))
    {
        DLLRemove(match);
        match = DLLPushFront(curr_dll,(void *)data);
        res = DLLGetData(match);
    }

    return res;
}

void HashTableRemove(hash_table_t *table, const void *data)
{
    iterator_t match = {0};
    dll_t *curr_dll = NULL;
    size_t index = 0;

    assert(NULL != table);

    index = GetDllIndex(table, data);

    if (NULL == table->hash_table_array[index])
    {
        return;
    }

    curr_dll = table->hash_table_array[index];
    match = DLLFind(DLLBeginIter(curr_dll), DLLEndIter(curr_dll), table->is_match_func, (void *)data);

    if (1 != DLLIsSameIter(match, DLLEndIter(curr_dll)))
    {
        DLLRemove(match);
    }
}


size_t HashTableSize(const hash_table_t *table)
{
    size_t size = 0;
    size_t i = 0;

    assert(NULL != table);

    for (i = 0; i < table->capacity; ++i)
    {
        if (NULL == table->hash_table_array[i])
        {
            continue;
        }
        else
        {
            size += DLLCount(table->hash_table_array[i]);
        }
    }

    return size;
}

int HashTableIsEmpty(const hash_table_t *table)
{
    size_t i = 0;
    int is_dll_empty = 1;

    assert(NULL != table);

    for (i = 0; i < table->capacity && 1 == is_dll_empty; ++i)
    {
        is_dll_empty = ( NULL == table->hash_table_array[i]);
    }

    return i == table->capacity;
}

double HashLoad(const hash_table_t *table)
{
    assert(NULL != table);

    return (double)(HashTableSize(table) / table->capacity);
}

double HashSTDev(const hash_table_t *table)
{
    size_t i = 0;
    size_t dll_size = 0;
    double st_dev = 0;
    double load_factor = 0;

    assert(NULL != table);

    load_factor = HashLoad(table);

    for (i = 0; i < table->capacity; ++i)
    {
        if (NULL == table->hash_table_array[i])
        {
            continue;
        }
        else
        {
            dll_size = DLLCount(table->hash_table_array[i]);
            st_dev += pow(dll_size - load_factor, STANDARD_DEV_POW);
        }
    }

    return sqrt(st_dev / table->capacity);
}



int HashTableForEach(const hash_table_t *table, int (*action_func)(void *data, void *param), void *param)
{
    size_t i = 0;
    int res = 0;
    dll_t *curr_dll = NULL;

    assert(NULL != table);
    assert(NULL != action_func);

    for (i = 0; i < table->capacity && 0 == res; ++i)
    {
        curr_dll = table->hash_table_array[i];

        if (NULL == curr_dll)
        {
            continue;
        }

        res = DLLForEach(DLLBeginIter(curr_dll), DLLEndIter(curr_dll), action_func, param);
    }

    return res;
}

/*********************************************Static functions****************************/

static size_t GetDllIndex(const hash_table_t *table, const void *data)
{
    assert(NULL != table);

    return table->hash_func(data) % table->capacity;
}

