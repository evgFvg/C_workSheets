#include <string.h> /*memcpy*/
#include <stdlib.h> /*malloc*/
#include <assert.h>


#include "trie.h"


#define SUCCESS (0)
#define FULL (1)   
#define DOUBLE_FREE (2) 
#define DS_FAILURE (3) 
#define INVALID_FREE (4) 


#define NOT_FULL (0)
#define VALID (1)
#define INVALID (0)
#define SIZE_OF_IPV4 (32)
#define BYTE (0xFF)
#define MSB_NUMBER (31)
#define BROADCAST_IP_BITS ((unsigned int)-1)
#define NETWORK_IP_BITS ((unsigned int)0)
#define SERVER_IP_BITS ((BROADCAST_IP_BITS) - (1))

typedef struct trie_node trie_node_t;

typedef enum child_pos
{
    LEFT = 0,
    RIGHT = 1,
    NUM_OF_CHILDREN = 2
} child_pos_t;

struct trie
{
    trie_node_t *root;
    size_t bits_in_subnet;
};

struct trie_node
{
    trie_node_t *children[NUM_OF_CHILDREN];
    trie_node_t *parent;
    int is_full;
};

static unsigned int CharArrayToInt(const unsigned char *ip);
static trie_node_t *CreateTrieNode();
static void UpdateFullness(trie_node_t *node);
static int IsFullNode(trie_node_t *node);
static void TrieDestroyRec(trie_node_t *node);
static int IsValid_IP(trie_t *trie,unsigned int network_id, unsigned int bit_ip);
static trie_node_t *GetLastNode(trie_t *trie, unsigned int bit_ip);
static void FlipBit(unsigned int *bit_ip, unsigned int bit_number);
static trie_node_t *CreateAndConnect(trie_node_t *node, int side);
static size_t CountRec(trie_node_t *root, int curr_bit);
size_t TrieCountFree(const trie_t *trie);

/***************************************Functions******************************/
trie_t *CreateTrie(size_t bits_in_subnet)
{
    trie_t *trie = (trie_t *)malloc(sizeof(trie_t));

    if (NULL != trie)
    {
        trie->bits_in_subnet = bits_in_subnet;
        trie->root = CreateTrieNode();

        if (NULL == trie->root)
        {
            free(trie);
            return NULL;
        }
    }

    return trie;
}

int TrieInsert(trie_t *trie, unsigned int *bit_ip)
{
    size_t host_id_size = 0;
    unsigned int mask = 1;
    size_t i = 0;
    int side = 0;
    trie_node_t *trie_runner = NULL;

    assert(NULL != trie);
    assert(NULL != bit_ip);

    host_id_size = SIZE_OF_IPV4 - trie->bits_in_subnet;
    trie_runner = trie->root;

    for (i = 0; i < host_id_size; ++i)
    {
        side = (mask << (MSB_NUMBER - i)) & *bit_ip;
        side = !!side;

        if (NULL == trie_runner->children[side])
        {
            if (NULL == CreateAndConnect(trie_runner, side))
            {
                return DS_FAILURE;
            }
        }

        if (trie_runner->children[side]->is_full == FULL)
        {
            side = !side;
            FlipBit(bit_ip, i);

            if (NULL == trie_runner->children[side])
            {
                if (NULL == CreateAndConnect(trie_runner, side))
                {
                    return DS_FAILURE;
                }
            }
        }

        trie_runner = trie_runner->children[side];
    }

    trie_runner->is_full = FULL;
    UpdateFullness(trie_runner->parent);

    return SUCCESS;
}

void TrieDestroy(trie_t *trie)
{
    trie_node_t *node = NULL;

    assert(NULL != trie);

    node = trie->root;

    TrieDestroyRec(node);
    free(trie);
}

size_t TrieCountFree(const trie_t *trie)
{
    size_t host_id_size = 0; 
    assert(NULL != trie);

    host_id_size = SIZE_OF_IPV4 - trie->bits_in_subnet;

    return (1 << host_id_size)- CountRec(trie->root, host_id_size);
}

int TrieRemoveIp(trie_t *trie, unsigned int network_id,  unsigned char *ip)
{
    unsigned int bit_ip = 0;
    int res = SUCCESS;
    trie_node_t *last_node = NULL;

    assert(NULL != trie);
    assert(NULL != ip);

    bit_ip = CharArrayToInt(ip);
    
    if(trie->root->is_full == FULL)
    {
        return FULL;
    }

    if (VALID != IsValid_IP(trie, network_id, bit_ip))
    {
        res = INVALID_FREE;
    }
    else if (NULL == (last_node = GetLastNode(trie, bit_ip)) || NOT_FULL == last_node->is_full)
    {
        res = DOUBLE_FREE;
    }
    else
    {
        last_node->is_full = NOT_FULL;
        UpdateFullness(last_node);
    }

    return res;
}


/****************************Static Functions*******************************/

static trie_node_t *CreateTrieNode()
{
    trie_node_t *new_node = (trie_node_t *)malloc(sizeof(trie_node_t));

    if (NULL != new_node)
    {
        new_node->children[LEFT] = NULL;
        new_node->children[RIGHT] = NULL;
        new_node->parent = NULL;
        new_node->is_full = NOT_FULL;
    }

    return new_node;
}



static trie_node_t *CreateAndConnect(trie_node_t *node, int side)
{
    node->children[side] = CreateTrieNode();

    if (NULL != node->children[side])
    {
        node->children[side]->parent = node;
    }

    return node->children[side];
}

static void FlipBit(unsigned int *bit_ip, unsigned int bit_number)
{
    unsigned int mask = 1;
    *bit_ip = (mask << (MSB_NUMBER - bit_number)) ^ *bit_ip;
}

static void UpdateFullness(trie_node_t *node)
{
    while (NULL != node)
    {
        if (FULL == IsFullNode(node))
        {
            node->is_full = FULL;
        }
        else
        {
            node->is_full = NOT_FULL;
        }

        node = node->parent;
    }
}

static int IsFullNode(trie_node_t *node)
{
    return NULL != node->children[LEFT] &&
           NULL != node->children[RIGHT] &&
           node->children[LEFT]->is_full == FULL &&
           node->children[RIGHT]->is_full == FULL;
}



static void TrieDestroyRec(trie_node_t *node)
{
    size_t i = 0;

    for (i = 0; i < NUM_OF_CHILDREN; ++i)
    {
        if (NULL != node->children[i])
        {
            TrieDestroyRec(node->children[i]);
        }
    }

    free(node);
}

static size_t CountRec(trie_node_t *root, int curr_bit)
{
    if (NULL == root)
    {
        return 0;
    }

    if (0 == curr_bit && root->is_full == FULL)
    {
        return 1;
    }

    return CountRec(root->children[LEFT], curr_bit - 1) +
           CountRec(root->children[RIGHT], curr_bit - 1);
}



static trie_node_t *GetLastNode(trie_t *trie, unsigned int bit_ip)
{
    size_t host_id_size = SIZE_OF_IPV4 - trie->bits_in_subnet;
    unsigned int mask = 1;
    size_t i = 0;
    int side = 0;
    trie_node_t *trie_runner = trie->root;

    for (i = 0; i < host_id_size; ++i)
    {
        side = (mask << (MSB_NUMBER - i)) & bit_ip;
        side = !!side;

        if (NULL == trie_runner->children[side])
        {
            break;
        }
        else
        {
            trie_runner = trie_runner->children[side];
        }
    }

    return (i == host_id_size ? trie_runner : NULL);
}

static int IsValid_IP(trie_t *trie,unsigned int network_id, unsigned int bit_ip)
{
    unsigned char host_id_size = SIZE_OF_IPV4 - trie->bits_in_subnet;
    unsigned int mask = (1 << host_id_size) - 1;
    unsigned int subnet_mask = ( 1 << trie->bits_in_subnet) - 1;
    int res = VALID;

    if (((bit_ip >> trie->bits_in_subnet & mask)) == (BROADCAST_IP_BITS & mask) ||
        ((bit_ip >> trie->bits_in_subnet & mask)) == (SERVER_IP_BITS & mask) ||
        ((bit_ip >> trie->bits_in_subnet & mask)) == (NETWORK_IP_BITS & mask) || 
        (subnet_mask & bit_ip) != network_id)
    {
        res = INVALID;
    }

    return res;
}


static unsigned int CharArrayToInt(const unsigned char *ip)
{
    unsigned int res = 0;
    memcpy(&res, ip, sizeof(int));

    return res;
}


