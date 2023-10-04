
/*************************************************
 * developer: Evgenii F.
 * reviewer: Aviv S.
 * date: 
 *
 **************************************************/
#include <assert.h>
#include <stddef.h> /*NULL*/
#include "list_exercises.h"

node_t *Flip(node_t *head)
{
    node_t *prev = NULL;
    node_t *next = NULL;
    node_t *current = head;
    
    assert(NULL != head);

    while (current != NULL)
    {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    
    return prev;
}

/*Traverse linked list using two pointers.
Move one pointer(slow_iter) by one and another pointer(fast_iter) by two.
If these pointers meet at the same node then there is a loop. 
If pointers do not meet then the linked list doesn’t have a loop.*/

int HasLoop(const node_t *head)
{
    int res = 0;
    node_t *fast_iter = (node_t *) head;
    node_t *slow_iter = (node_t *) head;
    
    assert(NULL != head);

    while (NULL != fast_iter && NULL != slow_iter && fast_iter->next != NULL)
    {
        slow_iter = slow_iter->next;
        fast_iter = fast_iter->next->next;
        if (slow_iter == fast_iter)
        {
            res = 1;
            break;
        }
    }
    
    return res;
}

/*The total number of nodes in the first list + distance from the head of the second list to the intersection point = 
The total number of nodes in the second list + distance from the head of the first list to the intersection point.*/

node_t *FindIntersection(node_t *head_1, node_t *head_2)
{
    node_t *iter_1 = head_1;
    node_t *iter_2 = head_2;
    
    assert(NULL != head_1);
    assert(NULL != head_2);
    
    while (iter_1 != iter_2)
    {
        /* When the first list reaches its end, redirect it to the
        head of the second list*/
        iter_1 = (NULL == iter_1) ? head_2 : iter_1->next;
        iter_2 = (NULL == iter_2) ? head_1 : iter_2->next;
    }

    return iter_1;
}
