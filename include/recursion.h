#ifndef __ILRD_OL139_40_RECURSION_H__
#define __ILRD_OL139_40_RECURSION_H__

#include <stddef.h> /* size_t */

#include "stack.h" /* stack_t */

typedef struct node node_t;

struct node
{
    int *data;
    node_t *next;
};


int Fibonacci_R(int n);
int Fibonacci_NotR(int n);

node_t *FlipList(node_t *head);

void SortStack_Rec(stack_t *stack);

size_t Strlen(const char *str);
int Strcmp(const char *s1, const char *s2);
char *Strcpy(char *dest, const char *src);
char *Strcat(char *dest, const char *src);
char *Strstr(const char *haystack, const char *needle);



#endif
