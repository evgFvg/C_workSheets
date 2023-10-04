/************************************
* Binary Search Tree implementation	
* Developer: 	Evgenii Feigin 
* Reviewer:   	Lihi Cohen		
* Date:         22.03.2023		
************************************/
#include <assert.h> 
#include <stdlib.h> /* malloc */

#include "bst.h"

typedef enum child_pos
{
    LEFT = 0,
    RIGHT = 1,
    NUM_OF_CHILDREN = 2
} child_pos_t;

struct bst_node
{
    bst_node_t *parent;
    bst_node_t *children[NUM_OF_CHILDREN];
    void *data;
};

struct binary_search_tree
{
    bst_node_t dummy_root;
    int (*cmp_func)(const void *, const void *);
};

typedef enum
{
    SUCCESS = 0
} func_status_t;

static bst_node_t *IterToNode(bst_iter_t iter);
static bst_iter_t NodeToIter(bst_node_t *node);

static int IsLeaf(const bst_iter_t iter);
static int IsOneChild(const bst_iter_t iter);
static int IsLeftChild(const bst_node_t *node);

static void RemoveLeaf(bst_iter_t iter);
static void RemoveOneChildParent(bst_iter_t iter);
static void RemoveTwoChildParent(bst_iter_t iter);

static bst_node_t *CreateNode(void *data);
static size_t GetHeight(const bst_node_t *root);

/*****************************Functions****************************************/

bst_t *BSTCreate(int (*cmp_func)(const void *, const void *))
{
    bst_t *new_bst = NULL;
    bst_node_t dummy = {0};

    assert(NULL != cmp_func);

    new_bst = (bst_t *)malloc(sizeof(bst_t));

    if (NULL != new_bst)
    {
    	new_bst->cmp_func = cmp_func;
    	new_bst->dummy_root = dummy;
    }

    return new_bst;
}

void BSTDestroy(bst_t *bst)
{
    assert(NULL != bst);

    while (0 == BSTIsEmpty(bst))
    {
       BSTRemove(BSTBeginIter(bst));
    }

    free(bst);
}

bst_iter_t BSTInsert(bst_t *bst, void *data)
{
    bst_node_t *new_node = CreateNode(data);
    bst_node_t *parent = NULL;
    bst_node_t *runner = NULL;
    int cmp_res = 0;
    int son_side = LEFT;

    if (NULL == new_node)
    {
        return BSTEndIter(bst);
    }
    

    assert(NULL != bst);

    runner = bst->dummy_root.children[LEFT];
    parent = &bst->dummy_root;

    while (NULL != runner)
    {
        parent = runner;

        if (0 < (cmp_res = bst->cmp_func(runner->data, data)))
        {
            runner = runner->children[LEFT];
            son_side = LEFT;
        }
        else
        {
            runner = runner->children[RIGHT];
            son_side = RIGHT;
        }
    }

    if (parent == &bst->dummy_root) /*creating root*/
    {
        bst->dummy_root.children[LEFT] = new_node;
    }
    else
    {
   	parent->children[son_side] = new_node;
    }

    new_node->parent = parent;

    return NodeToIter(new_node);
}

void BSTRemove(bst_iter_t iter)
{
    if (1 == IsLeaf(iter))
    {
        RemoveLeaf(iter);
    }
    else if (1 == IsOneChild(iter))
    {
        RemoveOneChildParent(iter);
    }
    else
    {
        RemoveTwoChildParent(iter);
    }

    return;
}

bst_iter_t BSTFind(const bst_t *bst, const void *key)
{
    int cmp_res = 0;
    bst_node_t *runner = NULL;

    assert(NULL != bst);

    runner = bst->dummy_root.children[LEFT];

    while (NULL != runner && 0 != (cmp_res = bst->cmp_func(runner->data, key)))
    {
        if (0 < cmp_res)
        {
            runner = runner->children[LEFT];
        }
        else
        {
            runner = runner->children[RIGHT];
        }
    }

    return NULL == runner ? BSTEndIter(bst) : NodeToIter(runner);
}

int BSTForEach(bst_iter_t from, bst_iter_t to, int (*action_func)(void *, void *), void *param)
{
    int res = SUCCESS;
    bst_node_t *runner = IterToNode(from);
    bst_node_t *end = IterToNode(to);

    assert(NULL != action_func);

    while (runner != end && SUCCESS == res)
    {
        res = action_func(runner->data, param);
        from = BSTNextIter(from);
        runner = IterToNode(from);
    }

    return res;
}

int BSTIsEmpty(const bst_t *bst)
{
    assert(NULL != bst);
    
    return NULL == bst->dummy_root.children[LEFT];
}

size_t BSTSize(const bst_t *bst)
{
    bst_iter_t runner = {0};
    bst_iter_t end = {0};
    size_t count = 0;

    assert(NULL != bst);

    runner = BSTBeginIter(bst);
    end = BSTEndIter(bst);

    while (0 == BSTIsSameIter(runner, end))
    {
        ++count;
        runner = BSTNextIter(runner);
    }

    return count;
}

void *BSTGetData(bst_iter_t iter)
{
    bst_node_t *node = IterToNode(iter);

    return node->data;
}

size_t BSTHeight(const bst_t *bst)
{
    bst_node_t *root = NULL;

    assert(NULL != bst);

    root = bst->dummy_root.children[LEFT];
    
    return GetHeight(root);
}

/*************************** Iterator Functions ****************************/

bst_iter_t BSTBeginIter(const bst_t *bst)
{
    bst_node_t *runner = NULL;

    assert(NULL != bst);

    runner = bst->dummy_root.children[LEFT];

    if(NULL == runner)/*Tree is empty*/
    {
        return BSTEndIter(bst);
    }

    while (NULL != runner->children[LEFT])
    {
        runner = runner->children[LEFT];
    }

    return NodeToIter(runner);
}

bst_iter_t BSTEndIter(const bst_t *bst)
{
    return NodeToIter((bst_node_t *)&bst->dummy_root);
}

bst_iter_t BSTNextIter(const bst_iter_t iter)
{
    bst_node_t *curr = IterToNode(iter);
    bst_node_t *parent = curr->parent;
    bst_node_t *res = NULL;

    if (NULL != curr->children[RIGHT])
    {
        curr = curr->children[RIGHT];

        while (NULL != curr->children[LEFT])
        {
            curr = curr->children[LEFT];
        }

        res = curr;
    }
    else
    {
        while (curr != parent->children[LEFT])
        {
            curr = parent;
            parent = curr->parent;
        }

        res = parent;
    }

    return NodeToIter(res);
}

bst_iter_t BSTPrevIter(const bst_iter_t iter)
{
    bst_node_t *curr = IterToNode(iter);
    bst_node_t *parent = curr->parent;
    bst_node_t *res = NULL;

    if (NULL != curr->children[LEFT])
    {
        curr = curr->children[LEFT];

        while (NULL != curr->children[RIGHT])
        {
            curr = curr->children[RIGHT];
        }

        res = curr;
    }
    else
    {
        while (curr != parent->children[RIGHT])
        {
            curr = parent;
            parent = curr->parent;
        }

        res = parent;
    }
    return NodeToIter(res);
}

int BSTIsSameIter(const bst_iter_t iter1, const bst_iter_t iter2)
{
    return iter1 == iter2;
}

/********************************Static functions*************************/

static bst_node_t *CreateNode(void *data)
{
    bst_node_t *new_node = (bst_node_t *)malloc(sizeof(bst_node_t));
    if (NULL == new_node)
    {
        return NULL;
    }
    new_node->data = data;
    new_node->parent = NULL;
    new_node->children[LEFT] = NULL;
    new_node->children[RIGHT] = NULL;

    return new_node;
}

static bst_node_t *IterToNode(bst_iter_t iter)
{
    assert(NULL != (bst_node_t *)iter);
    
    return (bst_node_t *)iter;
}

static bst_iter_t NodeToIter(bst_node_t *node)
{
    return (bst_iter_t)node;
}

static int IsLeaf(const bst_iter_t iter)
{
    bst_node_t *node = IterToNode(iter);

    return (NULL == node->children[LEFT] && NULL == node->children[RIGHT]);
}

static int IsLeftChild(const bst_node_t *node)
{
    return node->parent->children[LEFT] == node;
}

static int IsOneChild(const bst_iter_t iter)
{
    bst_node_t *node = NULL;
    int res = 1;

    assert(NULL != iter);

    node = IterToNode(iter);

    if ((NULL == node->children[RIGHT] && NULL == node->children[LEFT]) ||
        (NULL != node->children[RIGHT] && NULL != node->children[LEFT]))
    {
        res = 0;
    }

    return res;
}

static void RemoveLeaf(bst_iter_t iter)
{
    bst_node_t *to_del = IterToNode(iter);
    bst_node_t *parent = to_del->parent;

    if (1 == IsLeftChild(to_del))
    {
        parent->children[LEFT] = NULL;
    }
    else
    {
        parent->children[RIGHT] = NULL;
    }

    free(to_del);
}

static void RemoveOneChildParent(bst_iter_t iter)
{
    bst_node_t *to_del = IterToNode(iter);
    bst_node_t *parent = to_del->parent;
    bst_node_t *child = to_del->children[RIGHT] == NULL ? to_del->children[LEFT] : to_del->children[RIGHT];

    if (1 == IsLeftChild(to_del))
    {
        parent->children[LEFT] = child;
    }
    else
    {
        parent->children[RIGHT] = child;
    }

    child->parent = parent;

    free(to_del);
}

static void RemoveTwoChildParent(bst_iter_t iter)
{
    bst_node_t *to_del = IterToNode(iter);
    bst_iter_t next_iter = BSTNextIter(iter);
    bst_node_t *successor = IterToNode(next_iter);
    bst_node_t *successor_parent = successor->parent;
    
    if (1 == IsLeftChild(successor))
    {
        successor_parent->children[LEFT] = successor->children[RIGHT];

        if (NULL != successor->children[RIGHT])
        {
            successor->children[RIGHT]->parent = successor_parent;
        }
    }
    else
    {
        successor_parent->children[RIGHT] = successor->children[RIGHT];

        if (NULL != successor->children[RIGHT])
        {
            successor->children[RIGHT]->parent = successor_parent;
        }
    }

    to_del->data = successor->data;

    free(successor);
}

static size_t GetHeight(const bst_node_t *root)
{
    size_t height_left = 0;
    size_t height_right = 0;

    if (NULL == root)
    {
        return 0;
    }
    
    height_left = GetHeight(root->children[LEFT]);
    height_right = GetHeight(root->children[RIGHT]);

    return (height_left > height_right ? height_left : height_right) + 1;
}

