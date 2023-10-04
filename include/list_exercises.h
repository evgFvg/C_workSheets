#ifndef __LIST_EXERCISES_H__
#define __LIST_EXERCISES_H__

typedef struct Node node_t;

struct Node
{
    int *data;
    node_t *next;
};

node_t *Flip(node_t *head);
int HasLoop(const node_t *head);
node_t *FindIntersection(node_t *head_1, node_t *head_2);

#endif
