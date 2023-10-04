#include <assert.h>
#include <stdio.h> /*printf*/
#include "dll.h"

static void TestCreate(void);
static void TestGetSetPushInsert(void);
static void TestPopRemove(void);
static void TestFind(void);
static void TestMultiFind(void);
static void TestSplice(void);

static int isSameNumber(void *node_data, const void *number);
static int isNumberBiggerThanN(void *node_data, const void *number);

int main()
{
    TestCreate();
    TestGetSetPushInsert();
    TestPopRemove();
    TestFind();
    TestMultiFind();
    TestSplice();

    printf(" All tests have been passed\n");

    return 0;
}


static void TestCreate()
{
    dll_t *dll = DLLCreate();
    iterator_t head = DLLBeginIter(dll);
    iterator_t tail = DLLEndIter(dll);

    assert(1 == DLLIsEmpty(dll));
    assert(0 == DLLCount(dll));
    assert(head == tail);
    DLLDestroy(dll);
}

static void TestGetSetPushInsert(void)
{

    int a = 5, b = 6, c = 7, d = 8;
    dll_t *dll = DLLCreate();
    iterator_t prev_iter = NULL;
    iterator_t test_iter = NULL;

    test_iter = DLLInsert(DLLBeginIter(dll), (void *)&a);
    assert(a == *(int *)DLLGetData(test_iter));
    assert(1 == DLLIsSameIter(test_iter, DLLBeginIter(dll)));

    test_iter = DLLInsert(DLLNextIter(test_iter), (void *)&b);
    test_iter = DLLInsert(DLLNextIter(test_iter), (void *)&c);
    test_iter = DLLInsert(DLLNextIter(test_iter), (void *)&d);

    assert(4 == DLLCount(dll));
    assert(*(int *)DLLGetData(test_iter) == d);
    DLLSetData(test_iter, &a);
    assert(*(int *)DLLGetData(test_iter) == a);

    test_iter = DLLPushFront(dll, &b);
    assert(1 == DLLIsSameIter(test_iter, DLLBeginIter(dll)));
    assert(*(int *)DLLGetData(test_iter) == b);

    test_iter = DLLPushBack(dll, &c);
    prev_iter = DLLPrevIter(DLLEndIter(dll));
    assert(1 == DLLIsSameIter(test_iter, prev_iter));
    assert(*(int *)DLLGetData(test_iter) == c);

    DLLDestroy(dll);
}

static void TestPopRemove()
{
    int a = 5, b = 6, c = 7, d = 8;
    int deleted = 0;
    dll_t *dll = DLLCreate();
    iterator_t test_iter = NULL;

    DLLPushFront(dll, &a);
    DLLPushFront(dll, &b);
    DLLPushFront(dll, &c);
    DLLPushFront(dll, &d);

    assert(4 == DLLCount(dll));

    deleted = *(int *)DLLPopBack(dll);
    assert(3 == DLLCount(dll));
    assert(deleted == a);

    deleted = *(int *)DLLPopFront(dll);
    assert(2 == DLLCount(dll));
    assert(deleted == d);

    test_iter = DLLBeginIter(dll);
    deleted = *(int *)DLLGetData(test_iter);
    test_iter = DLLRemove(DLLBeginIter(dll)); /*returns pointer to the next*/
    assert(1 == DLLCount(dll));
    assert(1 == DLLIsSameIter(test_iter, DLLBeginIter(dll)));

    DLLDestroy(dll);
}

static void TestFind(void)
{
    int a = 5, b = 6, c = 7, d = 8;
    int non_exist = 57;
    dll_t *dll = DLLCreate();
    iterator_t found_elm_iter = NULL;

    DLLPushFront(dll, &a);
    DLLPushFront(dll, &b);
    DLLPushFront(dll, &c);
    DLLPushFront(dll, &d);

    assert(4 == DLLCount(dll));

    found_elm_iter = DLLFind(DLLBeginIter(dll), DLLEndIter(dll), isSameNumber, (void *)&non_exist); /*no 57 in the list, iterator to to is returned*/
    assert(1 == DLLIsSameIter(found_elm_iter, DLLEndIter(dll)));

    found_elm_iter = DLLFind(DLLBeginIter(dll), DLLEndIter(dll), isSameNumber, (void *)&c); /*there is c in the list*/
    assert(c == *(int *)DLLGetData(found_elm_iter));

    DLLRemove(found_elm_iter);
    found_elm_iter = DLLFind(DLLBeginIter(dll), DLLEndIter(dll), isSameNumber, (void *)&c); /*there is no c in the list after Remove*/
    assert(1 == DLLIsSameIter(found_elm_iter, DLLEndIter(dll)));

    DLLDestroy(dll);
}

static void TestMultiFind()
{
    int x = 20, y = 30, z = 40, w = 50;
    dll_t *dll_dest = DLLCreate();
    dll_t *dll_src = DLLCreate();
    int param = 30;

    DLLPushFront(dll_src, &x);
    DLLPushFront(dll_src, &y);
    DLLPushFront(dll_src, &z);
    DLLPushFront(dll_src, &w);

    DLLMultiFind(dll_dest, DLLBeginIter(dll_src), DLLEndIter(dll_src), isNumberBiggerThanN, &param);

    assert(2 == DLLCount(dll_dest));
    assert(40 == *(int *)DLLGetData(DLLBeginIter(dll_dest)));

    DLLDestroy(dll_dest);
    DLLDestroy(dll_src);
}

static void TestSplice()
{
    int a = 5, b = 6, c = 7, d = 8;
    int x = 20, y = 30, z = 40, w = 50;
    dll_t *dll = DLLCreate();
    dll_t *dll_src = DLLCreate();
    iterator_t dest_iter = NULL;
    iterator_t src_iter_from = NULL;
    iterator_t src_iter_to = NULL;

    DLLPushFront(dll, &a);
    DLLPushFront(dll, &b);
    DLLPushFront(dll, &c);
    DLLPushFront(dll, &d);

    DLLPushFront(dll_src, &x);
    DLLPushFront(dll_src, &y);
    DLLPushFront(dll_src, &z);
    DLLPushFront(dll_src, &w);

    dest_iter = DLLNextIter(DLLBeginIter(dll));
    src_iter_from = DLLNextIter(DLLBeginIter(dll_src));
    src_iter_to = DLLPrevIter(DLLEndIter(dll_src));

    assert(7 == *(int *)DLLGetData(dest_iter));
    assert(40 == *(int *)DLLGetData(src_iter_from));
    assert(20 == *(int *)DLLGetData(src_iter_to));

    DLLSplice(dest_iter, src_iter_from, src_iter_to);

    assert(6 == DLLCount(dll));
    assert(2 == DLLCount(dll_src));

    dest_iter = DLLNextIter(DLLBeginIter(dll));
    src_iter_from = DLLNextIter(DLLBeginIter(dll_src));
    src_iter_to = DLLPrevIter(DLLEndIter(dll_src));

    assert(40 == *(int *)DLLGetData(dest_iter));
    assert(20 == *(int *)DLLGetData(src_iter_from));
    assert(20 == *(int *)DLLGetData(src_iter_to));

    DLLDestroy(dll);
    DLLDestroy(dll_src);
}



static int isSameNumber(void *node_data, const void *number)
{
    return *(int *)node_data == *(int *)number;
}
/*searching for every number that is bigger than 'number' variable*/
static int isNumberBiggerThanN(void *node_data, const void *number)
{
    int data = *(int *)node_data;
    return data > *(int *)number ? 0 : 1;
}


