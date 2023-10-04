
#include <assert.h>
#include <stdlib.h> /* malloc*/
#include <stdio.h>  /* printf*/
#include <string.h> /* strlen*/
#include <sys/types.h> /*ssize_t*/

#include "hash.h"

#define BIGGEST_WORD_SIZE (100)
#define DICTIONARY_PATH ("/usr/share/dict/american-english")
#define DICTIONARY_HASH_SIZE (1000)
#define PRIME_NUMBER (31)
#define MAX_STRING_LENGTH (50)

/*******************Dictionary function prototypes******************************************/
static size_t GetWordsNum(char *path);
static size_t GetLongestWordSize(char *path);
static void FillWordBuffer(char ***buffer, size_t words_number, size_t max_word_size);
static void FillHashTable(hash_table_t *table, char **words, size_t words_buff_size);
static void DestroyWordsBuffer(char **buff, size_t size);
static void SpellChecker(const hash_table_t *table);


/**************************Test function prototypes*******************************************/

static void TestCreate(void);
static void TestInsert(void);
static void TestFind(void);
static void TestRemove(void);
static void TestForEach(void);
static void TestDictionary(void);

size_t GetHash(const void *value);
static int IsSameString(const void *lhs, const void *rhs);
static int PrintValue(void *lhs, void *rhs);


int main()
{
    TestCreate();
    TestInsert();
    TestFind();
    TestRemove();
    TestForEach();
    TestDictionary();

    printf("All tests have been passed\n");

    return 0;
}

/************************************Test functions****************************************/

static void TestCreate()
{
    hash_table_t *new_table = HashTableCreate(10, GetHash, IsSameString);

    assert(NULL != new_table);
    assert(0 == HashTableSize(new_table));
    assert(0 == HashLoad(new_table));
    assert(1 == HashTableIsEmpty(new_table));

    HashTableDestroy(new_table);
}

static void TestInsert()
{
    hash_table_t *new_table = HashTableCreate(10, GetHash, IsSameString);
    char *buffer[] = {"Hello", "hunny", "I", "am", "home"};
    size_t arr_size = sizeof(buffer) / sizeof(buffer[0]);
    size_t i = 0;
    int res = 0;

    for (i = 0; i < arr_size && 0 == res; ++i)
    {
        res = HashTableInsert(new_table, buffer[i]);
    }

    assert(i == arr_size);
    assert(HashTableSize(new_table) == arr_size);
    assert(0 == HashTableIsEmpty(new_table));

    HashTableDestroy(new_table);
}

static void TestFind()
{
    hash_table_t *new_table = HashTableCreate(10, GetHash, IsSameString);
    char *buffer[] = {"hello", "hunny", "I", "am", "home"};
    size_t arr_size = sizeof(buffer) / sizeof(buffer[0]);
    size_t i = 0;
    int res = 0;
    char *expected = NULL;
    char *not_exist = NULL;

    for (i = 0; i < arr_size && 0 == res; ++i)
    {
        res = HashTableInsert(new_table, buffer[i]);
    }

    assert(i == arr_size);

    expected = (char *)HashTableFind(new_table, (void *)"hunny");
    not_exist = (char *)HashTableFind(new_table, (void *)"FOX");

    assert(0 == strcmp(expected, "hunny"));
    assert(NULL == not_exist);

    HashTableDestroy(new_table);
}

static void TestRemove()
{
    hash_table_t *new_table = HashTableCreate(10, GetHash, IsSameString);
    char *buffer[] = {"Hello", "hunny", "I", "am", "home"};
    size_t arr_size = sizeof(buffer) / sizeof(buffer[0]);
    size_t i = 0;
    int res = 0;

    for (i = 0; i < arr_size && 0 == res; ++i)
    {
        res = HashTableInsert(new_table, buffer[i]);
    }

    assert(i == arr_size);

    HashTableRemove(new_table, "am");

    assert(arr_size - 1 == HashTableSize(new_table));

    HashTableRemove(new_table, "FOX"); /*doesn't exist*/

    assert(arr_size - 1 == HashTableSize(new_table));

    HashTableDestroy(new_table);
}

static void TestForEach()
{
    hash_table_t *new_table = HashTableCreate(10, GetHash, IsSameString);
    char *buffer[] = {"Hello", "hunny", "I", "am", "home"};
    size_t arr_size = sizeof(buffer) / sizeof(buffer[0]);
    size_t i = 0;
    int res = 0;

    for (i = 0; i < arr_size && 0 == res; ++i)
    {
        res = HashTableInsert(new_table, buffer[i]);
    }

    assert(i == arr_size);

    HashTableForEach(new_table, PrintValue, NULL);

    HashTableDestroy(new_table);
}

/*************************************SpellChecker********************************************/
static void TestDictionary()
{
    hash_table_t *new_table = HashTableCreate(DICTIONARY_HASH_SIZE, GetHash, IsSameString);
    size_t max_word_size = GetLongestWordSize(DICTIONARY_PATH);
    size_t words_number = GetWordsNum(DICTIONARY_PATH);
    char **words_buffer = NULL;

    FillWordBuffer(&words_buffer, words_number, max_word_size);
    FillHashTable(new_table, words_buffer, words_number);
    SpellChecker(new_table);

    HashTableDestroy(new_table);
    DestroyWordsBuffer(words_buffer, words_number);
}


static void SpellChecker(const hash_table_t *table)
{
    char user_input[MAX_STRING_LENGTH] = {0};
    void *is_in_table = NULL;

    while (1)
    {
        printf("Enter a word to search in the hash table, or 0 to exit:\n");
        scanf(" %s", user_input);

        if (0 == strcmp("0", user_input))
        {
            break;
        }
        else
        {
            is_in_table = HashTableFind(table, (void *)user_input);

            if (NULL != is_in_table)
            {
                printf("The word is in the dictionary\n");
            }
            else
            {
                printf("There is no such a word in the dictionary\n");
            }
        }
    }
}

static void DestroyWordsBuffer(char **buff, size_t size)
{
    size_t i = 0;

    for(i = 0; i < size; ++i)
    {
        free(buff[i]);
    }

    free(buff);
}

static void FillHashTable(hash_table_t *table, char **words, size_t words_buff_size)
{
    size_t i = 0;
    int res = 0;
    char *str_end = NULL;

    for (i = 0; i < words_buff_size && 0 == res; ++i)
    {
        str_end = strchr(words[i], '\n');

        if (NULL != str_end)
        {
            *str_end = '\0';
        }

        res = HashTableInsert(table, (void *)words[i]);
    }

    assert(i == words_buff_size);
}

static void FillWordBuffer(char ***buffer, size_t words_number, size_t max_word_size)
{
    ssize_t i = 0;
    FILE *fp = NULL;
    char *one_word = NULL;

    *buffer = (char **)malloc(words_number * sizeof(char *));

    if (NULL == *buffer)
    {
        return;
    }

    one_word = (char *)malloc((max_word_size + 1) * sizeof(char));

    if (NULL == one_word)
    {
        free(*buffer);
        return;
    }

    fp = fopen(DICTIONARY_PATH, "r");

    if (fp == NULL)
    {
        free(one_word);
        free(*buffer);
        return;
    }

    while (i < (ssize_t)words_number && EOF != fscanf(fp, "%s", one_word))
    {
        (*buffer)[i] = (char *)malloc((max_word_size + 1) * sizeof(char));

        if (NULL == (*buffer)[i])
        {
            free(one_word);
            fclose(fp);
            while (i >= 0)
            {
                free((*buffer)[i]);
                --i;
            }

            return;
        }

        strcpy((*buffer)[i], one_word);
        ++i;
    }

    fclose(fp);
    free(one_word);
}

static size_t GetLongestWordSize(char *path)
{

    char word[BIGGEST_WORD_SIZE];
    size_t max_word_length = 0;
    size_t curr_word_length = 0;

    FILE *fp = fopen(path, "r");

    if (fp == NULL)
    {
        return 0;
    }

    while (fscanf(fp, "%s", word) == 1)
    {
        curr_word_length = strlen(word);
        
        if (curr_word_length > max_word_length)
        {
            max_word_length = curr_word_length;
        }
    }

    fclose(fp);
    return max_word_length;
}

static size_t GetWordsNum(char *path)
{
    char buffer[BIGGEST_WORD_SIZE] = {0};
    int line_count = 0;

    FILE *fp = fopen(path, "r");
    
    if (fp == NULL)
    {
        return 0;
    }

    while (NULL != fgets(buffer, sizeof(buffer), fp))
    {
        line_count++;
    }
	
    fclose(fp);
    return line_count;
}



size_t GetHash(const void *value)
{
    char *runner = NULL;
    size_t hash = PRIME_NUMBER;
    assert(NULL != value);

    runner = (char *)value;
    while (*runner != '\0')
    {
        hash = ((hash << 5) + hash) + *runner;
        ++runner;
    }

    return hash;
}

static int IsSameString(const void *lhs, const void *rhs)
{
    char *str1 = (char *)lhs;
    char *str2 = (char *)rhs;
    int res = !strcmp(str1, str2);

    return res;
}

static int PrintValue(void *lhs, void *rhs)
{
    
    char *str = (char *)lhs;
    (void)rhs;
    printf("%s\n", str);
    return 0;
}






