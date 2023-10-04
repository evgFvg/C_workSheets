#include<assert.h>
#include<string.h> /*strcmp*/
#include<stdio.h> /*printf*/
#include "stack.h"

static void TestStack();

int main()
{
    struct stack {int x;};
    TestStack();
    return 0;
}

static void TestStack()
{
    char buffer[8] = "Hello";
    double test_d = 45.70;
    
    stack_t *new_stack = StackCreate(15, 8);
    assert(15 == StackCapacity(new_stack));
    
    StackPush(new_stack, &test_d);
    StackPush(new_stack, buffer);
    
    assert(2 == StackSize(new_stack));
    assert(0 == strcmp((char *)StackPeek(new_stack), buffer));

    StackPop(new_stack);
    StackPop(new_stack);
    assert(1 == StackIsEmpty(new_stack));
    StackDestroy(new_stack);
    
    printf("All stack tests have been passed successfully\n");
}
