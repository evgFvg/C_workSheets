
#include <assert.h>
#include <stdio.h>  /*printf*/
#include <string.h> /*strlen*/

#include "recursion.h"
#include "stack.h"

#define FIBONACCI_NUMBER (5)

static void TestFibonacci(void);
static void TestFlip(void);
static void TestStrlen(void);
static void TestStrcmp(void);
static void TestStrcpy(void);
static void TestStrcat(void);
static void TestStrstr(void);
static void TestStack(void);

int main()
{

    TestFibonacci();
    TestFlip();
    TestStrlen();
    TestStrcmp();
    TestStrcpy();
    TestStrcat();
    TestStrstr();
    
    TestStack();
    
    printf("All tests have been passed\n");

    return 0;
}

static void TestFibonacci()
{
    int res = Fibonacci_R(FIBONACCI_NUMBER);
    int res2 = Fibonacci_NotR(FIBONACCI_NUMBER);
    int expected = 34;

    assert(res == res2);
    assert(expected = Fibonacci_R(9));
    assert(expected = Fibonacci_NotR(9));
}

static void TestFlip()
{
    int a = 5, b = 6, c = 7, d = 8, e = 9;
    node_t *flipped_head = NULL;

    node_t head, n2, n3, n4, tail;

    head.data = &a;
    head.next = &n2;

    n2.data = &b;
    n2.next = &n3;

    n3.data = &c;
    n3.next = &n4;

    n4.data = &d;
    n4.next = &tail;

    tail.data = &e;
    tail.next = NULL;

    flipped_head = FlipList(&head);

    assert(flipped_head->data == &e);
    assert(flipped_head->next->data = &d);
    assert(n4.next == &n3);
    assert(n3.next == &n2);
    assert(n2.next == &head);
    assert(NULL == head.next);
}

static void TestStrlen(void)
{
    size_t len = Strlen("Hello");
    assert(strlen("Hello") == len);
    assert(strlen(" ") == Strlen(" "));
    assert(strlen("") == Strlen(""));
}

static void TestStrcmp(void)
{
    assert(strcmp("Hello", "Hello") == Strcmp("Hello", "Hello"));
    assert(strcmp(" ", " ") == Strcmp(" ", " "));
    assert(Strcmp("Hello", "hello") < 0);
    assert(0 < Strcmp("hello", "Hello"));
}

static void TestStrcpy(void)
{
    char dest[10] = {0};
    char src[] = "Hello";
    Strcpy(dest, src);
    
    assert(strcmp(dest, src) == 0);
}

static void TestStrcat(void)
{
    char dest[20] = "Hello ";
    char src[] = "world";
    Strcat(dest, src);

    assert(strcmp(dest, "Hello world") == 0);
}

static void TestStrstr(void)
{
    char str[] = "CrSEStack";
    char sub[] = "SES";
    char nonexist[] = "SWS";

    assert(strstr(str, sub) == Strstr(str, sub));
    assert(NULL == Strstr(str, nonexist));
}


static void TestStack()
{
    int a = 70, b = 60, c = 3, d = 2, e = 11;
    stack_t *new_stack = CreateStack(5, sizeof(int));

    Push(new_stack, &e);
    Push(new_stack, &d);
    Push(new_stack, &c);
    Push(new_stack, &b);
    Push(new_stack, &a);
    SortStack_Rec(new_stack);

    while (1 != StackSize(new_stack))
    {
        a = *(int *)Peek(new_stack);
        Pop(new_stack);
        assert(a < *(int *)Peek(new_stack));
    }

    DestroyStack(new_stack);
}


