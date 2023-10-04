#include <assert.h> 
#include <stdio.h>  /* printf */
#include <string.h> /* memcmp */

#include "avl.h"

/*************************Test funcs******************************/

static int CmpNumbers(const void *a, const void *b);
static int FillBuffer(void *a, void *b);

static void TestCreate(void);
static void TestInsertDestroy(void);
static void TestFind(void);
static void TestRemove(void);
static void TestDestroy(void);
static void TestForEach(void);

/*******************************************************/

int main()
{
    TestCreate();
    TestInsertDestroy();
    TestFind();
    TestRemove();
    TestDestroy();
    TestForEach();

    printf("All test have been passed\n");
    
    return 0;
}

/*****************************Test functions*********************/

static void TestCreate()
{
    avl_t *avl = AVLCreate(CmpNumbers);

    AVLDestroy(avl);
}

static void TestInsertDestroy()
{
    int ar[] = {8, 3, 6, 4, 7, 1, 10, 14, 13};
    size_t i = 0;
    size_t ar_size = sizeof(ar) / sizeof(int);
    avl_t *avl = AVLCreate(CmpNumbers);

    for (i = 0; i < ar_size; ++i)
    {
        AVLInsert(avl, (void *)(&ar[i]));
    }

    assert(i == ar_size);
    assert(ar_size == AVLCount(avl));

    AVLDestroy(avl);
}

static void TestFind(void)
{
    int ar[] = {8, 3, 6, 4, 7, 1, 10, 14, 13};
    size_t i = 0;
    int no_exist = 90;
    void *match = NULL;
    size_t ar_size = sizeof(ar) / sizeof(int);
    avl_t *avl = AVLCreate(CmpNumbers);

    for (i = 0; i < ar_size; ++i)
    {
        AVLInsert(avl, (void *)(&ar[i]));
    }

    match = AVLFind(avl, &ar[2]); /* search 6*/

    assert(NULL != match);
    assert(ar[2] == *(int *)match);

    match = AVLFind(avl, &no_exist);

    assert(NULL == match);

    AVLDestroy(avl);
}

static void TestRemove(void)
{
    int ar[] = {8, 3, 6, 4, 7, 1, 10, 14, 13};
    size_t i = 0;
    int no_exist = 90;
    size_t ar_size = sizeof(ar) / sizeof(int);
    avl_t *avl = AVLCreate(CmpNumbers);

    for (i = 0; i < ar_size; ++i)
    {
        AVLInsert(avl, (void *)(&ar[i]));
    }

    assert(4 == AVLHeight(avl));

    AVLRemove(avl, &ar[2]); /*2 child node, root,6*/
    assert(ar_size - 1 == AVLCount(avl));
    assert(4 == AVLHeight(avl));

    AVLRemove(avl, &ar[0]); /*1 child node 8*/
    assert(ar_size - 2 == AVLCount(avl));
    assert(3 == AVLHeight(avl));

    AVLRemove(avl, &no_exist);
    assert(ar_size - 2 == AVLCount(avl));
    assert(3 == AVLHeight(avl));

    AVLDestroy(avl);
}

static void TestDestroy()
{
    int ar[] = {8, 3, 6, 4, 7, 1, 10, 14, 13};
    size_t i = 0;
    size_t ar_size = sizeof(ar) / sizeof(int);
    int ins_status = 0;
    avl_t *avl = AVLCreate(CmpNumbers);

    for (i = 0; i < ar_size && 0 == ins_status; ++i)
    {
        ins_status = AVLInsert(avl, (void *)(&ar[i]));
    }

    AVLDestroy(avl);
}

static void TestForEach()
{
    int ar[] = {25, 50, 15, 22, 35, 70, 66, 90, 31, 44, 24, 18, 10, 12, 4};

    int pre_expected[15] = {25, 15, 10, 4, 12, 22, 18, 24, 50, 35, 31, 44, 70, 66, 90};
    int in_expected[15] = {4, 10, 12, 15, 18, 22, 24, 25, 31, 35, 44, 50, 66, 70, 90};
    int post_expected[15] = {4, 12, 10, 18, 24, 22, 15, 31, 44, 35, 66, 90, 70, 50, 25};
    int buffer[15] = {0};

    size_t i = 0;
    size_t ar_size = sizeof(ar) / sizeof(int);
    avl_t *avl = AVLCreate(CmpNumbers);

    for (i = 0; i < 15; ++i)
    {
        AVLInsert(avl, &ar[i]);
    }

    AVLForEach(avl, FillBuffer, buffer, PRE_ORDER);

    assert(0 == memcmp(pre_expected, buffer, ar_size * sizeof(int)));

    AVLForEach(avl, FillBuffer, buffer, IN_ORDER);

    assert(0 == memcmp(in_expected, buffer, ar_size * sizeof(int)));

    AVLForEach(avl, FillBuffer, buffer, POST_ORDER);

    assert(0 == memcmp(post_expected, buffer, ar_size * sizeof(int)));

    AVLDestroy(avl);
}

/*********************Static functions**********************/

static int CmpNumbers(const void *a, const void *b)
{
    return *(int *)a - *(int *)b;
}

static int FillBuffer(void *a, void *ar)
{
    static int index = 0; /*only for testing*/
    int *arr = (int *)ar;
    *(arr + index) = *(int *)a;
    ++index;

    if (index == 15)
    {
        index = 0;
    }

    return 0;
}

