#include <assert.h> 
#include <stdio.h>  /* printf */

#include "bst.h"

static int CmpNumbers(const void *a, const void *b);
static int PrintValue(void *a, void *b);

static void TestCreate(void);
static void TestInsertDestroy(void);
static void TestNextPrev(void);
static void TestFind(void);
static void TestForEach(void);
static void TestRemove(void);
static void TestHeight();

/*******************************************************/

int main()
{
    TestCreate();
    TestInsertDestroy();
    TestNextPrev();
    TestFind();
    TestForEach();
    TestRemove();
    TestHeight();
    
    printf("All tests have been passed\n");
    
    return 0;
}


/********************************Static functions*********************/

static void TestCreate()
{
    bst_t *bst = BSTCreate(CmpNumbers);
    BSTDestroy(bst);
}

static void TestInsertDestroy()
{
    int ar[] = {8, 3, 6, 4, 7, 1, 10, 14, 13};
    size_t i = 0;
    size_t ar_size = sizeof(ar) / sizeof(int);
    bst_t *bst = BSTCreate(CmpNumbers);

    for (i = 0; i < ar_size; ++i)
    {
        BSTInsert(bst, (void *)(&ar[i]));
    }

    assert(ar_size == BSTSize(bst));

    BSTDestroy(bst);
}

static void TestNextPrev()
{
    int ar[] = {8, 3, 6, 4, 7, 1, 10, 14, 13};
    int ar_sorted[] = {1, 3, 4, 6, 7, 8, 10, 13, 14};
    size_t i = 0;
    size_t ar_size = sizeof(ar) / sizeof(int);
    bst_t *bst = BSTCreate(CmpNumbers);
    bst_iter_t runner = {0};

    for (i = 0; i < ar_size; ++i)
    {
        BSTInsert(bst, (void *)(&ar[i]));
    }

    runner = BSTBeginIter(bst);

    for (i = 0; i < ar_size; ++i)
    {
        assert(ar_sorted[i] == *(int *)BSTGetData(runner));
        runner = BSTNextIter(runner);
    }

    assert(1 == BSTIsSameIter(runner, BSTEndIter(bst)));

    runner = BSTPrevIter(runner);

    for (i = ar_size - 1; i > 0; --i)
    {
        assert(ar_sorted[i] == *(int *)BSTGetData(runner));
        runner = BSTPrevIter(runner);
    }

    assert(1 == BSTIsSameIter(runner, BSTBeginIter(bst)));

    BSTDestroy(bst);
}

static void TestFind(void)
{
    int ar[] = {8, 3, 6, 4, 7, 1, 10, 14, 13};
    size_t i = 0;
    int no_exist = 90;
    size_t ar_size = sizeof(ar) / sizeof(int);
    bst_t *bst = BSTCreate(CmpNumbers);
    bst_iter_t match = {0};

    for (i = 0; i < ar_size; ++i)
    {
        BSTInsert(bst, (void *)(&ar[i]));
    }

    match = BSTFind(bst, &ar[2]); /* search 6*/
    assert(0 == BSTIsSameIter(match, BSTEndIter(bst)));
    assert(ar[2] == *(int *)BSTGetData(match));

    match = BSTFind(bst, &no_exist);
    assert(1 == BSTIsSameIter(match, BSTEndIter(bst)));

    BSTDestroy(bst);
}

static void TestForEach(void)
{
    int ar[] = {8, 3, 6, 4, 7, 1, 10, 14, 13};
    size_t i = 0;
    size_t ar_size = sizeof(ar) / sizeof(int);
    bst_t *bst = BSTCreate(CmpNumbers);

    for (i = 0; i < ar_size; ++i)
    {
        BSTInsert(bst, (void *)(&ar[i]));
    }

    assert(0 == BSTForEach(BSTBeginIter(bst), BSTEndIter(bst), PrintValue, NULL));

    BSTDestroy(bst);
}

static void TestRemove(void)
{
    int ar[] = {8, 3, 6, 4, 7, 1, 10, 14, 13, 9};
    size_t i = 0;
    size_t ar_size = sizeof(ar) / sizeof(int);
    bst_t *bst = BSTCreate(CmpNumbers);
    bst_iter_t match = {0};

    for (i = 0; i < ar_size; ++i)
    {
        BSTInsert(bst, (void *)(&ar[i]));
    }

    match = BSTBeginIter(bst);
    BSTRemove(match);
    assert(ar_size - 1 == BSTSize(bst));

    match = BSTBeginIter(bst);
    assert(ar[1] == *(int *)BSTGetData(match));
    BSTRemove(match);
    assert(ar_size - 2 == BSTSize(bst));

    match = BSTFind(bst, &ar[6]); /* search 10, two kids*/
    BSTRemove(match);
    assert(ar_size - 3 == BSTSize(bst));

    BSTDestroy(bst);
}

static void TestHeight()
{
    int ar[] = {8, 3, 6, 4, 7, 1, 10, 14, 13};
    size_t i = 0;
    size_t ar_size = sizeof(ar) / sizeof(int);
    bst_t *bst = BSTCreate(CmpNumbers);

    assert(0 == BSTHeight(bst));

    BSTInsert(bst, &ar[0]);

    assert(1 == BSTHeight(bst));

    for (i = 1; i < ar_size; ++i)
    {
        BSTInsert(bst, (void *)(&ar[i]));
    }

    assert(4 == BSTHeight(bst));

    BSTDestroy(bst);
}

static int CmpNumbers(const void *a, const void *b)
{
    int x = *(int *)a;
    int v = *(int *)b;
    int res = x - v;
    return res;
}

static int PrintValue(void *a, void *b)
{
    (void)b;
    printf("Node's value is %d\n", *(int *)a);
    return 0;
}
