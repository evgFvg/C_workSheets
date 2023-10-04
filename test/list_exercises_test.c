#include <stdio.h>/*printf*/
#include <assert.h>
#include "list_exercises.h"

static void PrintList(const node_t *head);

int main()
{
    int a = 5, b = 6, c = 7, d = 8, e = 9;
    node_t *intersection = NULL;
    node_t *flipped_sll = NULL;
    int is_has_loop = 0;

    node_t head, n2, n3, n4, tail;
    node_t head2, k2;

    head.data = &a;
    head.next = &n2;

    n2.data = &b;
    n2.next = &n3;

    n3.data = &c;
    n3.next = &n4;

    n4.data = &d;
    n4.next = &tail;

    tail.data = &e;
    tail.next = &head; /*Is Has Loop testing*/
    is_has_loop = HasLoop(&head);
    assert(1 == is_has_loop);
    tail.next = NULL; /*No loop any longer*/

    head2.data = &a;
    head2.next = &k2;

    k2.data = &b;
    k2.next = &n4; /*Intersection*/
    intersection = FindIntersection(&head, &head2);
    assert(intersection == &n4);
    
    printf("before Flip\n");
    PrintList(&head);
    flipped_sll = Flip(&head);
    printf("after Flip\n");
    PrintList(flipped_sll);
    
    printf("All tests have been passed\n");

    return 0;
}

static void PrintList(const node_t *head)
{
    while (NULL != head)
    {
        printf("%d\n", *head->data);
        head = head->next;
    }
}
