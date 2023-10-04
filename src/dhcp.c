#include <string.h>  /*memcpy*/
#include <stdlib.h> /*malloc*/
#include <assert.h>

#include "dhcp.h"
#include "trie.h"

#define SIZE_OF_IPV4 (32)
#define BYTE (0xFF)
#define BROADCAST_IP_BITS (0xFFFFFFFFu)
#define NETWORK_IP_BITS (0x0)
#define SERVER_IP_BITS ((BROADCAST_IP_BITS) - (1))
#define MAX_UINT ((unsigned int)-1)
#define CONST_IP_NUM (3)
#define MIN_BITS_TO_ALLOCATE (2)

typedef struct trie_node trie_node_t;

struct dhcp_s
{
    unsigned int subnet_mask;
    size_t bits_in_subnet;
    void *trie;
};


static void IntToCharArray(unsigned int number, unsigned char *arr);
static dhcp_status_t InitDHCP(dhcp_t *dhcp);
static unsigned int CharArrayToInt(const unsigned char *ip);

/***********************************User Functions*********************************/

dhcp_t *DHCPCreateDHCP(unsigned char *subnet_ip, size_t bits_in_subnet)
{
    dhcp_status_t status = SUCCESS;
    unsigned int mask = 0; 
    unsigned int bitt_ip  = 0;
    dhcp_t *dhcp = NULL; 

    assert(NULL != subnet_ip);
    assert(SIZE_OF_IPV4 - bits_in_subnet >= MIN_BITS_TO_ALLOCATE);

    dhcp  = (dhcp_t *)malloc(sizeof(dhcp_t));

    if (NULL == dhcp)
    {
        return NULL;
    }
    
    mask = (1 << bits_in_subnet) - 1;
    bitt_ip = CharArrayToInt(subnet_ip);
    dhcp->bits_in_subnet = bits_in_subnet;
    dhcp->subnet_mask = mask & bitt_ip;
    dhcp->trie = CreateTrie(bits_in_subnet);
    
    if (NULL == dhcp->trie)
    {
        free(dhcp);
        return NULL;
    }

    status = InitDHCP(dhcp);

    if (SUCCESS != status)
    {
        DHCPDestroy(dhcp);
        dhcp = NULL;
    }

    return dhcp;
}

void DHCPDestroy(dhcp_t *dhcp)
{
    assert(NULL != dhcp);
    assert(NULL != dhcp->trie);
    
    TrieDestroy(dhcp->trie);
    free(dhcp);
}

dhcp_status_t DHCPAllocateIp(dhcp_t *dhcp, unsigned char *requested_ip, unsigned char *result_ip)
{
    unsigned int bit_ip = 0;
    dhcp_status_t res = SUCCESS;
    unsigned int res_ip = 0;

    assert(NULL != dhcp);
    assert(NULL != dhcp->trie);
    assert(NULL != requested_ip);
    assert(NULL != result_ip);

    bit_ip = CharArrayToInt(requested_ip);
    res = TrieInsert(dhcp->trie, &bit_ip);

    bit_ip = bit_ip & (MAX_UINT << dhcp->bits_in_subnet);
    res_ip = bit_ip | dhcp->subnet_mask;
    IntToCharArray(res_ip, result_ip);

    return res;
}


dhcp_status_t DHCPFreeIp(dhcp_t *dhcp, unsigned char *ip_to_free)
{
    assert(NULL != dhcp);
    assert(NULL != dhcp->trie);

    return TrieRemoveIp(dhcp->trie, dhcp->subnet_mask,  ip_to_free);
}

size_t DHCPCountFree(const dhcp_t *dhcp)
{
    assert(NULL != dhcp);
    assert(NULL != dhcp->trie);

    return TrieCountFree(dhcp->trie);
}

/****************************Static Functions**********************************/

static dhcp_status_t InitDHCP(dhcp_t *dhcp)
{
    dhcp_status_t status = SUCCESS;
    size_t i = 0;
    unsigned int untouchable[CONST_IP_NUM] = {BROADCAST_IP_BITS, SERVER_IP_BITS, NETWORK_IP_BITS};

    for (i = 0; i < CONST_IP_NUM && SUCCESS == status; ++i)
    {
        status = TrieInsert(dhcp->trie, &untouchable[i]);
    }

    return i == CONST_IP_NUM ? SUCCESS : DS_FAILURE;
}

static void IntToCharArray(unsigned int number, unsigned char *arr)
{
    size_t i = 0;
    unsigned char byte = 0;
    
    for (i = 0; i < SIZE_OF_IPV4 / __CHAR_BIT__; ++i)
    {
        byte = (number >> (__CHAR_BIT__ * i)) & BYTE;
        arr[i] = byte;
    }
}

static unsigned int CharArrayToInt(const unsigned char *ip)
{
    unsigned int res = 0;
    memcpy(&res, ip, sizeof(int));

    return res;
}


