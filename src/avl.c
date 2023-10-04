/************************************
* AVL implementation	
* Developer: 	Evgenii Feigin 
* Reviewer:   	Harel Salhuv		
* Date:         26.03.2023		
************************************/
#include <assert.h> 
#include <stdlib.h> /* malloc */

#include "avl.h" 

typedef struct avl_node avl_node_t;

#define IS_HAS_LEFT_CHILD(node) (NULL != node->children[LEFT])
#define IS_HAS_RIGHT_CHILD(node) (NULL != node->children[RIGHT])
#define MAX(a, b) (a > b ? a : b)

typedef enum insert_status
{
    SUCCESS = 0,
    FAILURE = 1
} ins_status_t;

typedef enum child_pos
{
    LEFT = 0,
    RIGHT = 1,
    NUM_OF_CHILDREN = 2
} child_pos_t;

struct avl_node
{
    avl_node_t *children[NUM_OF_CHILDREN];
    void *data;
    size_t height;
};

struct avl_tree
{
    avl_node_t dummy_root;
    int (*cmp_func)(const void *, const void *);
};


static avl_node_t *CreateNode(void *data);
static void RecInsert(avl_t *avl, avl_node_t *node, avl_node_t *parent, int side, avl_node_t *new_node);
static void *RecFind(const avl_t *avl, avl_node_t *root, const void *key);
static void RecDestroy(avl_node_t *node);
static size_t RecCount(const avl_node_t *root);

static void BalanceTree(avl_node_t *node, avl_node_t *parent);
static int GetBalanceFactor(avl_node_t *node);
static void LeftRotation(avl_node_t *node, avl_node_t *parent);
static void RightRotation(avl_node_t *node, avl_node_t *parent);
static void UpdateHeight(avl_node_t *node);

static int IsLeaf(const avl_node_t *node);
static int IsLeftChild(avl_node_t *node, avl_node_t *parent);
static int IsOneChild(const avl_node_t *node);

static void RemoveRec(avl_t *avl, avl_node_t *node, avl_node_t *node_parent, const void *key);
static void RemoveLeaf(avl_node_t *node, avl_node_t *parent, int child_side);
static void RemoveOneChildParent(avl_node_t *node, avl_node_t *parent, int child_side);
static void RemoveTwoChildParent(avl_t *avl, avl_node_t *node);
static avl_node_t *GetSuccessor(avl_node_t *node);
static child_pos_t GetChildSide(avl_t *avl, avl_node_t *parent, const void *key);


static int ForEachInOrder(avl_node_t *node, int (*action_func)(void *, void *), void *param);
static int ForEachPostOrder(avl_node_t *node, int (*action_func)(void *, void *), void *param);
static int ForEachPreOrder(avl_node_t *node, int (*action_func)(void *, void *), void *param);

/**************************Functions**********************************/

avl_t *AVLCreate(int (*cmp_func)(const void *, const void *))
{
    avl_t *new_avl = NULL;
    avl_node_t dummy = {0};

    assert(NULL != cmp_func);

    new_avl = (avl_t *)malloc(sizeof(avl_t));

    if (NULL != new_avl)
    {
        new_avl->cmp_func = cmp_func;
        new_avl->dummy_root = dummy;
    }

    return new_avl;
}

void AVLDestroy(avl_t *avl)
{
    avl_node_t *root = avl->dummy_root.children[LEFT];

    assert(NULL != avl);

    RecDestroy(root);
}


int AVLInsert(avl_t *avl, void *data)
{
    avl_node_t *new_node = CreateNode(data);
    avl_node_t *root = NULL;
    int res = SUCCESS;

    assert(NULL != avl);

    if (NULL == new_node)
    {
        res = FAILURE;
    }
    else
    {
        root = avl->dummy_root.children[LEFT];

        if (NULL == root) /*root creating*/
        {
            avl->dummy_root.children[LEFT] = new_node;
        }
        else
        {
            RecInsert(avl, root, &avl->dummy_root, LEFT, new_node);
        }
    }

    return res;
}

void AVLRemove(avl_t *avl, const void *key)
{
    assert(NULL != avl);
    assert(NULL != key);

    if (!AVLIsEmpty(avl))
    {
        RemoveRec(avl, avl->dummy_root.children[LEFT], &avl->dummy_root, key);
    }
}

void *AVLFind(const avl_t *avl, const void *key)
{
    avl_node_t *root = NULL;

    assert(NULL != avl);

    root = avl->dummy_root.children[LEFT];

    return RecFind(avl, root, key);
}

int AVLForEach(avl_t *avl, int (*action_func)(void *, void *), void *param, order_t order)
{
    int res = 0;
    avl_node_t *root = NULL;

    assert(NULL != avl);
    assert(NULL != action_func);

    root = avl->dummy_root.children[LEFT];

    switch (order)
    {
    case IN_ORDER:
        res = ForEachInOrder(root, action_func, param);
        break;
    case POST_ORDER:
        res = ForEachPostOrder(root, action_func, param);
        break;
    case PRE_ORDER:
        res = ForEachPreOrder(root, action_func, param);
        break;

    default:
        res = ForEachInOrder(root, action_func, param);
        break;
    }

    return res;
}


int AVLIsEmpty(const avl_t *avl)
{
    assert(NULL != avl);

    return (NULL == avl->dummy_root.children[LEFT]);
}

size_t AVLCount(const avl_t *avl)
{
    avl_node_t *root = NULL;

    assert(NULL != avl);

    root = avl->dummy_root.children[LEFT];

    return RecCount(root);
}

size_t AVLHeight(const avl_t *avl)
{
    avl_node_t *root = NULL;

    assert(NULL != avl);

    root = avl->dummy_root.children[LEFT];

    return NULL == root ? 0 : root->height;
}


/*********************Static functions**********************/

static void RecDestroy(avl_node_t *node)
{
    if (NULL == node)
    {
        return;
    }
    if (1 == IsLeaf(node))
    {
        free(node);
        return;
    }

    RecDestroy(node->children[LEFT]);
    RecDestroy(node->children[RIGHT]);
    free(node);
}


static void RemoveRec(avl_t *avl, avl_node_t *node, avl_node_t *node_parent, const void *key)
{
    int cmp_res = 0;
    child_pos_t child_pos = LEFT;

    if (NULL == node)
    {
        return;
    }

    cmp_res = avl->cmp_func(node->data, key);
    child_pos = 0 < cmp_res ? LEFT : RIGHT;

    if (0 != cmp_res)
    {
        RemoveRec(avl, node->children[child_pos], node, key);
    }
    else
    {
        child_pos = GetChildSide(avl, node_parent, key);

        if (1 == IsLeaf(node))
        {
            RemoveLeaf(node, node_parent, child_pos);
            node = NULL;
        }
        else if (1 == IsOneChild(node))
        {
            RemoveOneChildParent(node, node_parent, child_pos);
            node = NULL;
        }
        else
        {
            RemoveTwoChildParent(avl, node);
        }
    }

    UpdateHeight(node_parent);
    BalanceTree(node, node_parent);
}

static void RecInsert(avl_t *avl, avl_node_t *node, avl_node_t *parent, int side, avl_node_t *new_node)
{
    int cmp_res = 0;
    
    if (NULL == node)
    {
        parent->children[side] = new_node;

        return;
    }

    cmp_res = avl->cmp_func(node->data, new_node->data);

    if (0 < cmp_res)
    {
        RecInsert(avl, node->children[LEFT], node, LEFT, new_node);
    }
    else
    {
        RecInsert(avl, node->children[RIGHT], node, RIGHT, new_node);
    }

    UpdateHeight(node);
    BalanceTree(node, parent);
}


static int ForEachInOrder(avl_node_t *node, int (*action_func)(void *, void *), void *param)
{
    if (NULL == node)
    {
        return 0;
    }

    if (0 != (ForEachInOrder(node->children[LEFT], action_func, param)))
    {
        return 1;
    }

    if (0 != action_func(node->data, param))
    {
        return 1;
    }
    if (0 != (ForEachInOrder(node->children[RIGHT], action_func, param)))
    {
        return 1;
    }

    return 0;
}

static int ForEachPostOrder(avl_node_t *node, int (*action_func)(void *, void *), void *param)
{
    if (NULL == node)
    {
        return 0;
    }

    if (0 != (ForEachPostOrder(node->children[LEFT], action_func, param)))
    {
        return 1;
    }

    if (0 != (ForEachPostOrder(node->children[RIGHT], action_func, param)))
    {
        return 1;
    }

    if (0 != action_func(node->data, param))
    {
        return 1;
    }

    return 0;
}

static int ForEachPreOrder(avl_node_t *node, int (*action_func)(void *, void *), void *param)
{
    if (NULL == node)
    {
        return 0;
    }

    if (0 != action_func(node->data, param))
    {
        return 1;
    }

    if (0 != (ForEachPreOrder(node->children[LEFT], action_func, param)))
    {
        return 1;
    }

    if (0 != (ForEachPreOrder(node->children[RIGHT], action_func, param)))
    {
        return 1;
    }

    return 0;
}


static avl_node_t *CreateNode(void *data)
{
    avl_node_t *new_node = (avl_node_t *)malloc(sizeof(avl_node_t));

    if (NULL != new_node)
    {
        new_node->data = data;
        new_node->children[LEFT] = NULL;
        new_node->children[RIGHT] = NULL;
        new_node->height = 1;
    }

    return new_node;
}

static void BalanceTree(avl_node_t *node, avl_node_t *parent)
{
    int b_factor = 0;
    
    if (NULL == node)
    {
        return;
    }

    b_factor = GetBalanceFactor(node);

    if (b_factor > 1)
    {
        if (GetBalanceFactor(node->children[LEFT]) < 0)
        {
            LeftRotation(node->children[LEFT], node);
        }
        RightRotation(node, parent);
    }
    else if (b_factor < -1)
    {
        if (GetBalanceFactor(node->children[RIGHT]) > 0)
        {
            RightRotation(node->children[RIGHT], node);
        }
        LeftRotation(node, parent);
    }
}


static void RightRotation(avl_node_t *node, avl_node_t *parent)
{
    avl_node_t *new_root = node->children[LEFT];
    avl_node_t *right_sub_tree = new_root->children[RIGHT];
    child_pos_t child_pos = LEFT;

    new_root->children[RIGHT] = node;
    node->children[LEFT] = right_sub_tree;
    child_pos = (1 == IsLeftChild(node, parent) ? LEFT : RIGHT);
    parent->children[child_pos] = new_root;

    UpdateHeight(node);
    UpdateHeight(new_root);
}


static void LeftRotation(avl_node_t *node, avl_node_t *parent)
{
    avl_node_t *new_root = node->children[RIGHT];
    avl_node_t *left_sub_tree = new_root->children[LEFT];
    child_pos_t child_pos = LEFT;

    new_root->children[LEFT] = node;
    node->children[RIGHT] = left_sub_tree;
    child_pos = (1 == IsLeftChild(node, parent) ? LEFT : RIGHT);
    parent->children[child_pos] = new_root;

    UpdateHeight(node);
    UpdateHeight(new_root);
}

static int GetBalanceFactor(avl_node_t *node)
{
    size_t left_height = 0;
    size_t right_height = 0;
    int res = 0;

    left_height = (IS_HAS_LEFT_CHILD(node) ? node->children[LEFT]->height : 0);
    right_height = (IS_HAS_RIGHT_CHILD(node) ? node->children[RIGHT]->height : 0);
    res = (int)left_height - (int)right_height;

    return res;
}

static void UpdateHeight(avl_node_t *node)
{
    size_t left_height = 0;
    size_t right_height = 0;

    if (NULL != node)
    {
        left_height = (IS_HAS_LEFT_CHILD(node) ? node->children[LEFT]->height : 0);
        right_height = (IS_HAS_RIGHT_CHILD(node) ? node->children[RIGHT]->height : 0);
        node->height = 1 + MAX(left_height, right_height);
    }
}

static void *RecFind(const avl_t *avl, avl_node_t *root, const void *key)
{
    if (NULL == root)
    {
        return NULL;
    }

    if (0 == avl->cmp_func(root->data, key))
    {
        return root->data;
    }

    return 0 < avl->cmp_func(root->data, key) ? RecFind(avl, root->children[LEFT], key) : RecFind(avl, root->children[RIGHT], key);
}

static size_t RecCount(const avl_node_t *root)
{
    size_t count_left = 0;
    size_t count_right = 0;

    if (NULL == root)
    {
        return 0;
    }
    else
    {
        count_left = RecCount(root->children[LEFT]);
        count_right = RecCount(root->children[RIGHT]);
    }

    return count_left + count_right + 1;
}


static int IsLeaf(const avl_node_t *node)
{
    return (NULL == node->children[LEFT] && NULL == node->children[RIGHT]);
}

static int IsOneChild(const avl_node_t *node)
{
    int res = 1;

    if ((NULL == node->children[RIGHT] && NULL == node->children[LEFT]) ||
        (NULL != node->children[RIGHT] && NULL != node->children[LEFT]))
    {
        res = 0;
    }

    return res;
}

static void RemoveLeaf(avl_node_t *node, avl_node_t *parent, int child_side)
{
    parent->children[child_side] = NULL;

    free(node);
}

static void RemoveOneChildParent(avl_node_t *node, avl_node_t *parent, int child_side)
{
    avl_node_t *node_child = node->children[RIGHT] == NULL ? node->children[LEFT] : node->children[RIGHT];
    parent->children[child_side] = node_child;

    free(node);
}

static void RemoveTwoChildParent(avl_t *avl, avl_node_t *node)
{
    avl_node_t *successor = GetSuccessor(node->children[RIGHT]);
    node->data = successor->data;
    RemoveRec(avl, node->children[RIGHT], node, successor->data);
}

static avl_node_t *GetSuccessor(avl_node_t *node)
{
    if (NULL == node->children[LEFT])
    {
        return node;
    }

    return GetSuccessor(node->children[LEFT]);
}

static int IsLeftChild(avl_node_t *node, avl_node_t *parent)
{
    return parent->children[LEFT] == node;
}

static child_pos_t GetChildSide(avl_t *avl, avl_node_t *parent, const void *key)
{
    child_pos_t res = RIGHT;

    if (NULL != parent->children[LEFT] && 0 == avl->cmp_func(parent->children[LEFT]->data, key))
    {
        res = LEFT;
    }

    return res;
}


