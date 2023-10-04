
#include <assert.h>
#include <stdio.h>  /* printf*/


#include "heap.h"

static void TestCreate(void);
static void TestInsert(void);
static void TestPop(void);
static void TestRemove(void);
static void TestRemoveByKey(void);

static int IsSameNumber(const void *a, const void *b);
static int CmpNumber(const void *a, const void *b);

int main()
{
    TestCreate();
    TestInsert();
    TestPop();
    TestRemove();
    TestRemoveByKey();

    printf("All tests have been passed\n");

    return 0;
}

/********************************test Functions************************/

static void TestCreate()
{
    heap_t *heap = HeapCreate(CmpNumber);

    assert(NULL != heap);
    assert(1 == HeapIsEmpty(heap));
    assert(0 == HeapSize(heap));

    HeapDestroy(heap);
}

static void TestInsert()
{
    int arr[] = {5, 10, 7, 3, 8, 11, 0};
    size_t arr_size = sizeof(arr) / sizeof(arr[0]);
    size_t i = 0;
    int res = 0;
    heap_t *heap = HeapCreate(CmpNumber);

    assert(NULL != heap);

    for (i = 0; i < arr_size && 0 == res; ++i)
    {
        res = HeapPush(heap, &arr[i]);
    }

    assert(NULL != heap);
    assert(0 == HeapIsEmpty(heap));
    assert(arr_size == HeapSize(heap));

    HeapDestroy(heap);
}


static void TestPop()
{
    int arr[] = {5, 10, 7, 3, 8, 11, 0};
    size_t arr_size = sizeof(arr) / sizeof(arr[0]);
    size_t i = 0;
    heap_t *heap = HeapCreate(CmpNumber);

    assert(NULL != heap);

    for (i = 0; i < arr_size; ++i)
    {
        HeapPush(heap, &arr[i]);
    }

    for (i = 0; i < arr_size; ++i)
    {
        assert(arr_size - i == HeapSize(heap));
        HeapPop(heap);
    }

    assert(1 == HeapIsEmpty(heap));

    HeapDestroy(heap);
}

static void TestRemove()
{
    int arr[] = {5, 10, 7, 3, 8, 11, 0};
    size_t arr_size = sizeof(arr) / sizeof(arr[0]);
    heap_t *heap = HeapCreate(CmpNumber);
    int not_exist = 99;
    size_t i = 0;

    assert(NULL != heap);

    for (i = 0; i < arr_size; ++i)
    {
        HeapPush(heap, &arr[i]);
    }

    for (i = 0; i < arr_size; ++i)
    {
        assert(arr_size - i == HeapSize(heap));
        assert(arr[i] == *(int *)HeapRemove(heap, IsSameNumber, &arr[i]));
    }

    assert(NULL == HeapRemove(heap, IsSameNumber, &not_exist));

    HeapDestroy(heap);
}

static void TestRemoveByKey()
{
    int arr[] = {5, 10, 7, 3, 8, 11, 0};
    size_t arr_size = sizeof(arr) / sizeof(arr[0]);
    size_t i = 0;
    heap_t *heap = HeapCreate(CmpNumber);
    int not_exist = 99;

    assert(NULL != heap);

    for (i = 0; i < arr_size; ++i)
    {
        HeapPush(heap, &arr[i]);
    }

    for (i = 0; i < arr_size; ++i)
    {
         assert(arr_size - i == HeapSize(heap));
         assert(arr[i] == *(int *)HeapRemoveByKey(heap, &arr[i]));
    }

    assert(NULL == HeapRemoveByKey(heap, &not_exist));

    HeapDestroy(heap);
}

/***********************************User Test functions*********************/

static int CmpNumber(const void *a, const void *b)
{
    int lhs = *(int *)a;
    int rhs = *(int *)b;
    /*int res = rhs - lhs;*/ 
    
    return lhs - rhs;
}

static int IsSameNumber(const void *a, const void *b)
{
    int gg = *(int *)a;
    int hh = *(int *)b;

    return gg == hh;
}

