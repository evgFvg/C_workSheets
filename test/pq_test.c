#include <stdio.h>
#include <assert.h>
#include "pq.h"

static int CmpNumbers(const void *lhs, const void *rhs);
static int IsSameInt(const void *lhs, const void *rhs);
static void TestCreateDestroy(void);
static void TestEnqDeq(void);
static void TestPeek(void);
static void TestClear(void);
static void TestErase(void);
static void TestMerge(void);

int main()
{
    TestCreateDestroy();
    TestEnqDeq();
    TestPeek();
    TestClear();
    TestErase();
    TestMerge();
    printf("All tests have been passed successfully\n");
    
    return 0;
}


static void TestCreateDestroy()
{
    pq_t *pq = PQCreate(CmpNumbers);
    int res = PQIsEmpty(pq);

    assert(res == 1);

    res = PQSize(pq);

    assert(res == 0);

    PQDestroy(pq);
}

static void TestEnqDeq()
{
    int a = 8, b = 4, c = 2, d = 10;
    pq_t *pq = PQCreate(CmpNumbers);

    assert(0 == PQEnqueue(pq, &a));

    PQEnqueue(pq, &b);
    PQEnqueue(pq, &c);
    PQEnqueue(pq, &d);

    assert(PQSize(pq) == 4);
    assert(PQIsEmpty(pq) == 0);

    PQDequeue(pq);

    assert(PQSize(pq) == 3);

    PQDestroy(pq);
}

static void TestPeek()
{
    int a = 8, b = 4, c = 2, d = 10; 
    pq_t *pq = PQCreate(CmpNumbers); /*2 -> 4 -> 8 -> 10*/

    PQEnqueue(pq, &a);

    assert(a == *(int *)PQPeek(pq));

    PQEnqueue(pq, &b);
    PQEnqueue(pq, &c);
    PQEnqueue(pq, &d);

    assert(c == *(int *)PQPeek(pq));

    PQDequeue(pq);

    assert(b == *(int *)PQPeek(pq));

    PQDestroy(pq);
}

static void TestClear()
{
    int a = 8, b = 4, c = 2, d = 10; /*2 -> 4 -> 8 -> 10*/
    pq_t *pq = PQCreate(CmpNumbers);

    PQEnqueue(pq, &a);
    PQEnqueue(pq, &b);
    PQEnqueue(pq, &c);
    PQEnqueue(pq, &d);

    PQClear(pq);
    assert(PQSize(pq) == 0);
    assert(PQIsEmpty(pq) == 1);

    PQDestroy(pq);
}

static void TestErase()
{
    int a = 8, b = 4, c = 2, d = 10; /*2 -> 4 -> 8 -> 10*/
    int not_exist = 99;
    void *res = NULL;
    
    pq_t *pq = PQCreate(CmpNumbers);
    PQEnqueue(pq, &a);
    PQEnqueue(pq, &b);
    PQEnqueue(pq, &c);
    PQEnqueue(pq, &d);

    res = PQErase(pq, IsSameInt, &not_exist);

    assert(4 == PQSize(pq));
    assert(NULL == res);

    res = PQErase(pq, IsSameInt, &a);

    assert(3 == PQSize(pq));
    assert(*(int *)res == a);

    PQDestroy(pq);
}

static void TestMerge(void)
{
    int a = 8, b = 4, c = 2, d = 10; /*2 -> 4 -> 8 -> 10*/
    int x = 3, y = 4, z = 12, w = 20;
    int res = 0;
    pq_t *pq_dest = PQCreate(CmpNumbers);
    pq_t *pq_src  = PQCreate(CmpNumbers);

    PQEnqueue(pq_dest, &a);
    PQEnqueue(pq_dest, &b);
    PQEnqueue(pq_dest, &c);
    PQEnqueue(pq_dest, &d);

    PQEnqueue(pq_src, &x);
    PQEnqueue(pq_src, &y);
    PQEnqueue(pq_src, &z);
    PQEnqueue(pq_src, &w);

    pq_dest = PQMerge(pq_dest, pq_src);
    res = PQSize(pq_dest);
    assert(res == 8);
    res = PQSize(pq_src);
    assert(res == 0);

    assert(1 == PQIsEmpty(pq_src));
    res = *(int*)PQPeek(pq_dest);
    assert(res == 2);

    res = *(int*)PQErase(pq_dest,IsSameInt,&z);
    assert(12 == res);

    PQDestroy(pq_dest);
    PQDestroy(pq_src);
}

/****************************Static func PQ**************************/
static int CmpNumbers(const void *lhs, const void *rhs)
{
    int a = *(int *)lhs;
    int b = *(int *)rhs;
    return (a - b);
}

static int IsSameInt(const void *lhs, const void *rhs)
{
    int a = *(int *)lhs;
    int b = *(int *)rhs;
    return (a == b);
}
