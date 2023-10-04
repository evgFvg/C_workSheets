#ifndef __ILRD__OL13940_TRIE__
#define __ILRD__OL13940_TRIE__

#include <stddef.h>

typedef struct trie trie_t;


trie_t *CreateTrie(size_t bits_in_subnet);
void TrieDestroy(trie_t *trie);
int TrieInsert(trie_t *trie, unsigned int *bit_ip);
size_t TrieCountFree(const trie_t *trie);
int TrieRemoveIp(trie_t *trie, unsigned char *ip);

#endif
