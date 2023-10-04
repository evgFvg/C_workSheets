/*************************************************
 * developer: Evgenii F.
 * reviewer: Mor Z.
 * date: 
 *
 **************************************************/

#include <stdlib.h> /*malloc*/
#include <stdio.h>  /*perror*/
#include <assert.h>
#include "linked_list.h"

static int CountNodes(void *node_data, void *param);

struct Node
{
    void *data;
    node_t *next;
};

struct SingleLinkedList
{
    node_t *head;
    node_t *tail;
};

/******************Create Functions***************************/

sll_t *SLLCreate()
{
    node_t *dummy = NULL;
    sll_t *new_sll = (sll_t *)malloc(sizeof(sll_t));
    if (NULL == (new_sll))
    {
        perror("Memory allocation error\n");
        /*need return or 1 return but additional else?*/
      
    }
    else
    {
        dummy = (node_t *)malloc(sizeof(node_t));
        if (NULL == dummy)
        {
            perror("Memory allocation error\n");
            free(new_sll);
            new_sll = NULL; /* or return NULL*/
        }
        else
        {

            dummy->data = new_sll;
            dummy->next = NULL;

            new_sll->head = dummy;
            new_sll->tail = dummy;
        }
    }

    return new_sll;
}

void SLLDestroy(sll_t *sll)
{
    iterator_t runner = NULL;
    iterator_t temp  = NULL;
    assert(NULL != sll);

    runner = SLLBeginIter(sll);
    while (NULL != runner)
    {
        temp = runner;
        runner = runner->next;
        free(temp); 
    }
    free(sll);
}

/*********************Implementation**************************/

iterator_t SLLBeginIter(const sll_t *sll)
{
    assert(NULL != sll);
    
    return (iterator_t)sll->head;
}

iterator_t SLLNextIter(const iterator_t iterator)
{
    assert(NULL != iterator);
    assert(NULL != iterator->next); /* is needed ?*/
    
    return (iterator_t)iterator->next;
}

iterator_t SLLEndIter(const sll_t *sll)
{
    assert(NULL != sll);
    
    return (iterator_t)sll->tail;
}

int SLLIsSameIter(const iterator_t iter1, const iterator_t iter2)
{
    assert(NULL != iter1 && NULL != iter2);
    
    return (iter1 == iter2);
}

iterator_t SLLInsertBefore(iterator_t iterator, void *data)
{
    node_t *new_node = NULL;
    sll_t *list = NULL;
    assert(NULL != iterator);
    
    new_node = (node_t *)malloc(sizeof(node_t));
    if (NULL == new_node)
    {
        perror("Memory allocation error\n");
        while (NULL != iterator->next)
        {
            iterator = SLLNextIter(iterator);
        }
    }
    else
    {
        new_node->data = iterator->data;
        new_node->next = iterator->next;

        iterator->data = data;
        iterator->next = new_node;
        if (NULL == new_node->next) /*insertion the first node or iterator == tail. Need to  renew the tail in sll*/
        {
            list = (sll_t *)new_node->data;
            list->tail = new_node;
        }
    }
    return iterator;
}

int SLLIsEmpty(const sll_t *sll)
{
    assert(NULL != sll);
    
    return sll->head == sll->tail; /* == IsSameIterator(... ...) ?*/
}

void *SLLGetData(iterator_t iterator)
{
    assert(NULL != iterator);
    
    return iterator->data;
}

void SLLSetData(const iterator_t iterator, void *data)
{
    assert(NULL != iterator);
    assert(NULL != iterator->next);
    
    iterator->data = data;
}

iterator_t SLLRemove(iterator_t iterator)
{
    iterator_t to_be_deleted  = NULL;
    sll_t *list = NULL;
    assert(NULL != iterator);
    assert(NULL != iterator->next);
    
    to_be_deleted = iterator->next; /* == SLLGetNextIter(...) ?*/
    iterator->data = to_be_deleted->data;
    iterator->next = to_be_deleted->next;

    free(to_be_deleted);

    if (NULL == iterator->next) /*if there is only one element in the list, need to renew tail in sll*/
    {
        list = (sll_t *)iterator->data;
        list->tail = iterator;
    }
    return iterator;
}

int SLLForEach(iterator_t from, const iterator_t to, action_func_t user_func, void *param)
{
    int res = 0;
    assert(NULL != from && NULL != to);
    
    while (from != to && 0 == res)
    {
        res = user_func(from->data, param);
        from = from->next;
    }
    return res;
}


size_t SLLCount(const sll_t *sll)
{
    size_t count = 0;
    assert(NULL != sll);
    
    SLLForEach(sll->head, sll->tail, CountNodes, (void *)&count); 
    return count;
}


iterator_t SLLFind(iterator_t from, iterator_t to, is_match_t user_func, void *param)
{
    assert(NULL != from && NULL != to);
    
    while (from != to && 0 == user_func(from->data, param))
    {
        from = from->next;
    }
    return from;
}

sll_t *SLLAppend(sll_t *dest, sll_t *src)
{
    assert(NULL != dest && NULL != src);
    
    if (1 == SLLIsEmpty(src))
    {
        return dest;
    }
    dest->tail->data = src->head->data;
    dest->tail->next = src->head->next;

    src->tail->data = (void *)dest;
    /*renew the tail address in the sll */
    dest->tail = src->tail;

    /*Using the first elm of the src as a new dummy*/
    src->head->data = (void *)src;
    src->head->next = NULL;
    /*renew the tail address in the src */
    src->tail = src->head;

    return dest;
}

/*****************Static Functions******************/

static int CountNodes(void *node_data, void *counter)
{
    (void)node_data;
    (*(size_t *)counter)++;
    return 0;
}
