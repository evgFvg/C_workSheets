
#include <assert.h>
#include <string.h> /*strncmp*/

#include "recursion.h"
#include "stack.h"

static void DisassembleStack(stack_t *stack);
static void InsertInOrder(stack_t *stack, int x);

/****************************Functions******************************/

int Fibonacci_R(int n)
{
    if (1 >= n)
    {
        return n;
    }
    return (Fibonacci_R(n - 1) + Fibonacci_R(n - 2));
}

int Fibonacci_NotR(int n)
{
    int i = 0;
    int res = 0;
    int prev = 0, next = 1;

    if (0 == n)
    {
        return 0;
    }

    for (i = 2; i <= n; ++i)
    {
        res = prev + next;
        prev = next;
        next = res;
    }

    return next;
}

node_t *FlipList(node_t *head)
{
    node_t *new_head = NULL;

    if (NULL == head || NULL == head->next)
    {
        return head;
    }

    new_head = FlipList(head->next);
    head->next->next = head;
    head->next = NULL;

    return new_head;
}

size_t Strlen(const char *str)
{
    size_t count = 0;

    assert(NULL != str);

    if ('\0' == *str)
    {
        return 0;
    }

    return ++count + Strlen(++str);
}

int Strcmp(const char *s1, const char *s2)
{
    assert(NULL != s1);
    assert(NULL != s2);
	
    if ('\0' == *s1 || 0 != (*s1 - *s2))
    {
        return *s1 - *s2;
    }
    
    return Strcmp(++s1, ++s2);
}

char *Strcpy(char *dest, const char *src)
{
    assert(NULL != dest);
    assert(NULL != src);

    if ('\0' == *src)
    {
        *dest = *src;
        return dest;
    }

    *dest = *src;

    Strcpy(dest + 1, src + 1);

    return dest;
}

char *Strcat(char *dest, const char *src)
{
    assert(NULL != dest);
    assert(NULL != src);
    
    if ('\0' == *dest)
    {
        return Strcpy(dest, src);
    }
    
    Strcat((char *)dest + 1, src);
    
    return dest;
}

char *Strstr(const char *haystack, const char *needle)
{
    size_t n_len = 0;
    
    assert(NULL != haystack);
    assert(NULL != needle);
    
    n_len = Strlen(needle);

    if ('\0' == *haystack)
    {
        return NULL;
    }
    if (0 == strncmp(haystack, needle, n_len))
    {
        return (char *)haystack;
    }
    
    return Strstr(++haystack, needle);
}


void SortStack_Rec(stack_t *stack)
{
    DisassembleStack(stack);
}

/**************************Static Functions*************************/
static void DisassembleStack(stack_t *stack)
{
    int prev = 0;

    if (1 == IsEmpty(stack))
    {
        return;
    }

    prev = *(int *)Peek(stack);
    Pop(stack);
    DisassembleStack(stack);
    InsertInOrder(stack, prev);
}

static void InsertInOrder(stack_t *stack, int x)
{
    int current = 0;

    if (1 == IsEmpty(stack) || x < (current = *(int *)Peek(stack)))
    {
        Push(stack, &x);
        return;
    }
    else
    {
        Pop(stack);
        InsertInOrder(stack, x);
        Push(stack, &current);
    }
}


