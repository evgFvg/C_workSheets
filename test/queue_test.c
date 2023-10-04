#include <assert.h>
#include <stdio.h>/*printf*/

#include "queue.h"
#include "linked_list.h"

static void TestQueue(void);

int main()
{
    TestQueue();
    printf("All Queue tests have been passed\n");
    return 0;
}

static void TestQueue(void)
{
    int a = 5, b = 6, c = 8, d = 9;
    queue_t *new_queue = QueueCreate();
    queue_t *test_append = QueueCreate();

    assert(0 == QueueSize(new_queue));
    assert(1 == QueueIsEmpty(new_queue));
    assert(0 == QueueEnqueue(new_queue, (void *)&a));
    assert(a == *(int *)QueuePeek(new_queue));

    QueueEnqueue(new_queue, (void *)&b);
    QueueEnqueue(new_queue, (void *)&c);
    QueueEnqueue(new_queue, (void *)&d);

    QueueDequeue(new_queue); /* a should be deleted*/
    assert(b == *(int *)QueuePeek(new_queue));
    assert(3 == QueueSize(new_queue));

    QueueEnqueue(test_append, (void *)&d);
    QueueEnqueue(test_append, (void *)&c);
    QueueEnqueue(test_append, (void *)&b);

    new_queue = QueueAppend(new_queue, test_append);
    assert(6 == QueueSize(new_queue));

    QueueDestroy(new_queue);
    QueueDestroy(test_append);	
}
