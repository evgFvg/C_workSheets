/*************************************************
 * developer: Evgenii F.
 * reviewer: Neviu
 * date: 
 *
 **************************************************/

#include <stdlib.h> /*malloc*/
#include <stdio.h>  /*perror*/
#include <assert.h>

#include "queue.h"
#include "linked_list.h"

struct queue
{
    sll_t *queue;
};

/***************Functions***********************************/

queue_t *QueueCreate(void)
{
    queue_t *new_queue = (queue_t *)malloc(sizeof(queue_t));
    if (NULL == (new_queue))
    {
        perror("Memory allocation error\n");
        return NULL;
    }
    new_queue->queue = SLLCreate();

    return new_queue;
}

void QueueDestroy(queue_t *queue)
{
    assert(NULL != queue);
    SLLDestroy(queue->queue);
    free(queue);
}

int QueueEnqueue(queue_t *queue, void *data)
{
    assert(NULL != queue);

    return NULL == SLLInsertBefore(SLLEndIter(queue->queue), data) ? 1 : 0;
}

void QueueDequeue(queue_t *queue)
{
    assert(NULL != queue);

    SLLRemove(SLLBeginIter(queue->queue));
}

void *QueuePeek(const queue_t *queue)
{
    assert(NULL != queue);
    return SLLGetData(SLLBeginIter(queue->queue));
}

int QueueIsEmpty(const queue_t *queue)
{
    assert(NULL != queue);
    return SLLIsEmpty(queue->queue);
}

size_t QueueSize(const queue_t *queue)
{
    assert(NULL != queue);
    return SLLCount(queue->queue);
}

queue_t *QueueAppend(queue_t *dest, queue_t *src)
{
    assert(NULL != dest && NULL != src);
    SLLAppend(dest->queue, src->queue);
    return dest;
}
