#include <assert.h>
#include <stdio.h>
#include "linked_list.h"

static int isSameNumber(void *node_data, const void *number);
static void TestAppend(void);
static void TestAddRemove(void);

int main()
{
    
    TestAddRemove();
    TestAppend();
    printf("All tests have been passed successfully\n");
    return 0;
}

static void TestAddRemove(void)
{
    
    int a = 5, b = 6, c = 7, d = 8;
    int non_exist = 57;
    sll_t *list = SLLCreate();
    
    iterator_t test_iter = NULL;
    iterator_t found_elm_iter = NULL;
    assert(0 == SLLCount(list));
    assert(1 == SLLIsEmpty(list));

    test_iter = SLLInsertBefore(SLLBeginIter(list), (void *)&a);
    assert(a == *(int*)SLLGetData(test_iter));

    SLLInsertBefore(SLLBeginIter(list), (void *)&b);
    SLLInsertBefore(SLLBeginIter(list), (void *)&c);
    SLLInsertBefore(SLLBeginIter(list), (void *)&d);
    assert(4 == SLLCount(list));

    found_elm_iter = SLLFind(SLLBeginIter(list), SLLEndIter(list), isSameNumber, (void *)&non_exist); /*no 57 in the list, iterator to to is returned*/
    assert(1 == SLLIsSameIter(found_elm_iter, SLLEndIter(list)));

    found_elm_iter = SLLFind(SLLBeginIter(list), SLLEndIter(list), isSameNumber, (void *)&c); /*there is c in the list*/
    assert(c == *(int*)SLLGetData(found_elm_iter));

    SLLRemove(found_elm_iter);
    found_elm_iter = SLLFind(SLLBeginIter(list), SLLEndIter(list), isSameNumber, (void *)&c); /*there is no c in the list after Remove*/
    assert(1 == SLLIsSameIter(found_elm_iter, SLLEndIter(list)));

    SLLDestroy(list);
}

static void TestAppend(void)
{
    sll_t *dest = SLLCreate();
    sll_t *src = SLLCreate();
    int a = 5, b = 6, c = 7;

    SLLInsertBefore(SLLBeginIter(dest), (void *)&a);
    SLLInsertBefore(SLLBeginIter(dest), (void *)&b);
    SLLInsertBefore(SLLBeginIter(dest), (void *)&c);

    SLLInsertBefore(SLLBeginIter(src), (void *)&c);
    SLLInsertBefore(SLLBeginIter(src), (void *)&b);
    SLLInsertBefore(SLLBeginIter(src), (void *)&a);

    dest = SLLAppend(dest, src);
    
    assert( 6 == SLLCount(dest));
}

static int isSameNumber(void *node_data, const void *number)
{
    return *(int *)node_data == *(int *)number;
}

