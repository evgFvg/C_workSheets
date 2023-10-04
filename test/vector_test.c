#include <assert.h>
#include <stdio.h> /*printf*/
#include "vector.h"

static void TestVector();
int main()
{

    TestVector();
    printf("All vector tests have been passed successfully\n");

    return 0;
}

static void TestVector()
{
    size_t obj = 255;
    size_t b = 14;
    int res = 0;
    void *elm = NULL;

    vector_t *v = VectorCreate(10, sizeof(size_t));
    assert(VectorCapacity(v) == 10);
    assert(VectorSize(v) == 0);
    assert(VectorIsEmpty(v) == 1);

    res = VectorPushBack(v, &obj);
    elm = VectorGetAccessToElement(v, 0);
    assert(0 == res);
    assert(VectorSize(v) == 1);
    assert(VectorIsEmpty(v) == 0);
    assert(255 == *(size_t *)(elm));

    VectorPushBack(v, &b);
    VectorPushBack(v, &b);
    assert(VectorCapacity(v) == 10);
    assert(VectorSize(v) == 3);

    VectorPopBack(v); 
    assert(VectorSize(v)  == 2);
    assert(VectorCapacity(v) == 5);

    res = VectorReserve(v, 20);
    assert(0 == res);
    assert(VectorCapacity(v) == 20);
    VectorReserve(v, 10);
    assert(VectorCapacity(v) == 20);

    res = VectorShrinkToSize(v);
    assert(0 == res);
    assert(VectorCapacity(v) == 3);
    
    VectorDestroy(v);

}
