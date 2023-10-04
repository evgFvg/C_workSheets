#include "sorted_list.h"
#include <stdio.h>/*printf(), */

void TestBasicFunctions(void);
void TestForEach(void);
void TestPushPop(void);
void TestFindAndFindif(void);
void TestMerge(void);

int MatchLargerThen(const void *larger,const void *lower);
int MatchInt(void *num1, void *num2);
int AddNumbers(void *num1, void *num2);
void PrintTestResult(size_t *test_num, int result);

int main(void)
{
	TestBasicFunctions();
	TestForEach();
	TestPushPop();
	TestFindAndFindif();
	TestMerge();
	return 0;
}

void TestMerge(void)
{
	sol_t *src = NULL;
	sol_t *dest = NULL;
	int num1 = 1;
	int num2 = 2;
	int num3 = 3;
	int num4 = 4;
	int num5 = 5;
	int num6 = 6;
	int num7 = 7;
	int num8 = 8;
	int num9 = 9;
	int num10 = 10;
	size_t test_num = 1;
	
	printf("\nTesting Merge\n");
	src = SortedListCreate(&MatchLargerThen);
	PrintTestResult(&test_num ,(NULL != src ? 1 : 0));
	dest = SortedListCreate(&MatchLargerThen);
	PrintTestResult(&test_num ,(NULL != dest ? 1 : 0));

	SortedListInsert(src, &num1);
	SortedListInsert(src, &num2);
	SortedListInsert(src, &num3);
	SortedListInsert(src, &num4);
	SortedListInsert(src, &num5);
	SortedListInsert(src, &num6);
	SortedListInsert(src, &num7);
	SortedListInsert(src, &num8);
	SortedListInsert(src, &num9);
	SortedListInsert(src, &num10);	
	
	SortedListInsert(dest, &num8);
	
	PrintTestResult(&test_num, (10 == SortedListSize(src) ? 1 : 0));
	PrintTestResult(&test_num, (1 == SortedListSize(dest) ? 1 : 0));
	
	SortedListMerge(dest, src);
	PrintTestResult(&test_num, (0 == SortedListSize(src) ? 1 : 0));
	PrintTestResult(&test_num, (11 == SortedListSize(dest) ? 1 : 0));
	
	SortedListMerge(src, dest);
	PrintTestResult(&test_num, (11 == SortedListSize(src) ? 1 : 0));
	PrintTestResult(&test_num, (0 == SortedListSize(dest) ? 1 : 0));
	
	SortedListDestroy(src);
	SortedListDestroy(dest);

	src = SortedListCreate(&MatchLargerThen);
	PrintTestResult(&test_num ,(NULL != src ? 1 : 0));
	dest = SortedListCreate(&MatchLargerThen);
	PrintTestResult(&test_num ,(NULL != dest ? 1 : 0));	
	
	SortedListInsert(dest, &num1);
	SortedListInsert(dest, &num5);
	SortedListInsert(dest, &num6);
	SortedListInsert(dest, &num7);
	
	SortedListInsert(src, &num2);
	SortedListInsert(src, &num3);
	SortedListInsert(src, &num4);
	SortedListInsert(src, &num8);
	SortedListInsert(src, &num9);
	SortedListInsert(src, &num10);

	SortedListMerge(dest, src);
	PrintTestResult(&test_num, (0 == SortedListSize(src) ? 1 : 0));
	PrintTestResult(&test_num, (10 == SortedListSize(dest) ? 1 : 0));
	
	SortedListDestroy(src);
	SortedListDestroy(dest);	
}

void TestFindAndFindif(void)
{
	sol_t *sol = NULL;
	
	int num1 = 1;
	int num2 = 2;
	int num3 = 3;
	int num4 = 4;
	int num5 = 5;
	int num6 = 6;
	int num7 = 7;
	int num8 = 8;
	int num9 = 9;
	int num10 = 10;
	int not_in_list = 11;
	size_t test_num = 1;
	int result = 0;
	
	iterator_st end;
	iterator_st begin;
	iterator_st mid;
	
	printf("\nTesting Find/Findif\n");
	sol = SortedListCreate(&MatchLargerThen);
	PrintTestResult(&test_num ,(NULL != sol ? 1 : 0));
	
	end = SortedListInsert(sol, &num10);
	SortedListInsert(sol, &num7);
	SortedListInsert(sol, &num8);
	SortedListInsert(sol, &num2);
	mid = SortedListInsert(sol, &num3);
	SortedListInsert(sol, &num4);
	SortedListInsert(sol, &num5);
	SortedListInsert(sol, &num6);
	SortedListInsert(sol, &num9);
	begin = SortedListInsert(sol, &num1);
	
	
	PrintTestResult(&test_num, (10 == SortedListSize(sol) ? 1 : 0));
	
	result = SortedListIsSameIter(end, \
	SortedListFind(sol, begin, SortedListEndIter(sol), &num10));	
	PrintTestResult(&test_num, 1 == result ? 1 : 0);
	
	result = SortedListIsSameIter(end, \
	SortedListFind(sol, begin, end, &num10));	
	PrintTestResult(&test_num, 1 == result ? 1 : 0);

	result = SortedListIsSameIter(SortedListEndIter(sol), \
	SortedListFind(sol, begin, SortedListEndIter(sol), &not_in_list));	
	PrintTestResult(&test_num, 1 == result ? 1 : 0);
	printf("testing FindIf\n");
	result = SortedListIsSameIter(mid, SortedListFindIf(begin, SortedListEndIter(sol), \
	(is_match_t) &MatchInt, &num3));
	PrintTestResult(&test_num, 1 == result ? 1 : 0);
	
	result = SortedListIsSameIter(mid, SortedListFindIf(begin, SortedListEndIter(sol), \
	(is_match_t) &MatchInt, &not_in_list));
	PrintTestResult(&test_num, 1 == result ? 0 : 1);
	
	SortedListDestroy(sol);
		
}

void TestPushPop(void)
{
	sol_t *sol = NULL;
	int num1 = 1;
	int num2 = 2;
	int num3 = 3;
	
	iterator_st iter1;
	iterator_st iter2;
	iterator_st iter3;
	
	size_t test_num = 1;
	
	printf("\nTesting PopFront/Back\n");
	sol = SortedListCreate(&MatchLargerThen);
	PrintTestResult(&test_num ,(NULL != sol ? 1 : 0));
	
	iter1 = SortedListInsert(sol, &num1);
	PrintTestResult(&test_num ,(1 == *(int *)SortedListGetData(iter1) ? 1 : 0));
	PrintTestResult(&test_num, (1 == SortedListSize(sol) ? 1 : 0));	
	PrintTestResult(&test_num, (&num1 == SortedListPopFront(sol) ? 1 : 0));
	PrintTestResult(&test_num, (0 == SortedListSize(sol) ? 1 : 0));
	
	iter1 = SortedListInsert(sol, &num1);
	PrintTestResult(&test_num ,(1 == *(int *)SortedListGetData(iter1) ? 1 : 0));
	PrintTestResult(&test_num, (1 == SortedListSize(sol) ? 1 : 0));
	PrintTestResult(&test_num, (&num1 == SortedListPopBack(sol) ? 1 : 0));
	PrintTestResult(&test_num, (0 == SortedListSize(sol) ? 1 : 0));
		
	iter2 = SortedListInsert(sol, &num2);
	iter1 = SortedListInsert(sol, &num1);
	iter3 = SortedListInsert(sol, &num3);
	PrintTestResult(&test_num ,(1 == *(int *)SortedListGetData(iter1) ? 1 : 0));
	PrintTestResult(&test_num ,(2 == *(int *)SortedListGetData(iter2) ? 1 : 0));
	PrintTestResult(&test_num ,(3 == *(int *)SortedListGetData(iter3) ? 1 : 0));	
	PrintTestResult(&test_num, (3 == SortedListSize(sol) ? 1 : 0));
	
		
	PrintTestResult(&test_num, (&num3 == SortedListPopBack(sol) ? 1 : 0));
	PrintTestResult(&test_num, (&num1 == SortedListPopFront(sol) ? 1 : 0));
	
	PrintTestResult(&test_num, (1 == SortedListSize(sol) ? 1 : 0));
		
	SortedListDestroy(sol);	
}

void TestForEach(void)
{
	sol_t *sol = NULL;
	int num1 = 1;
	int num2 = 2;
	int num3 = 3;
	int num4 = 4;
	int num5 = 5;
	iterator_st iter1;
	iterator_st iter2;
	iterator_st iter3;
	iterator_st iter4;
	iterator_st iter5;	
	int param = -1;
	size_t test_num = 1;
	int res = 0;
	
	printf("\nTesting ForEach\n");
	sol = SortedListCreate(&MatchLargerThen);
	PrintTestResult(&test_num ,(NULL != sol ? 1 : 0));
	iter1 = SortedListInsert(sol, &num1);
	iter2 = SortedListInsert(sol, &num2);
	iter3 = SortedListInsert(sol, &num3);
	iter4 = SortedListInsert(sol, &num4);
	iter5 = SortedListInsert(sol, &num5);
	
	
	res = SortedListForEach(SortedListBeginIter(sol), SortedListEndIter(sol), &AddNumbers, &param);
	
	PrintTestResult(&test_num ,(0 == res ? 1 : 0));

	PrintTestResult(&test_num ,(0 == *(int *)SortedListGetData(iter1) ? 1 : 0));
	PrintTestResult(&test_num ,(1 == *(int *)SortedListGetData(iter2) ? 1 : 0));
	PrintTestResult(&test_num ,(2 == *(int *)SortedListGetData(iter3) ? 1 : 0));
	PrintTestResult(&test_num ,(3 == *(int *)SortedListGetData(iter4) ? 1 : 0));
	PrintTestResult(&test_num ,(4 == *(int *)SortedListGetData(iter5) ? 1 : 0));
	
	SortedListDestroy(sol);					
}


void TestBasicFunctions(void)
{
	sol_t *sol = NULL;
	iterator_st iter1;
	iterator_st iter2;
	iterator_st iter3;
	int num0 = 0;
	int num1 = 1;
	int num2 = 2;
	size_t test_num = 1;
	
	printf("\nTesting besic functions\n");
	sol = SortedListCreate(&MatchLargerThen);
	PrintTestResult(&test_num ,(NULL != sol ? 1 : 0));
	PrintTestResult(&test_num, (1 == SortedListIsEmpty(sol) ? 1 : 0));
	PrintTestResult(&test_num, (0 == SortedListSize(sol) ? 1 : 0));
	PrintTestResult(&test_num, \
	(SortedListIsSameIter(SortedListEndIter(sol), SortedListBeginIter(sol)) ? 1 : 0));
	iter1 = SortedListInsert(sol, &num0);
	
	PrintTestResult(&test_num,\
	SortedListIsSameIter(SortedListBeginIter(sol) , iter1) ? 1 : 0);
	
	iter2 = SortedListInsert(sol, &num1);
	PrintTestResult(&test_num,\
	SortedListIsSameIter(SortedListBeginIter(sol) , iter1) ? 1 : 0);
	
	iter3 = SortedListNextIter(iter1);	
	PrintTestResult(&test_num, SortedListIsSameIter(iter3 ,iter2) ? 1 : 0);
	
	iter3 = SortedListPrevIter(iter2);		
	PrintTestResult(&test_num, SortedListIsSameIter(iter3 ,iter1) ? 1 : 0);
	
	iter3 = SortedListInsert(sol, &num2);

	PrintTestResult(&test_num, (0 == SortedListIsEmpty(sol) ? 1 : 0));
	PrintTestResult(&test_num, (3 == SortedListSize(sol) ? 1 : 0));
	
	printf("\nTesting GetData\n");
	iter1 = SortedListBeginIter(sol);
	PrintTestResult(&test_num, &num0 == SortedListGetData(iter1) ? 1 : 0);
	iter1 = SortedListNextIter(iter1);
	PrintTestResult(&test_num, &num1 == SortedListGetData(iter1) ? 1 : 0);
	iter1 = SortedListNextIter(iter1);
	PrintTestResult(&test_num, &num2 == SortedListGetData(iter1) ? 1 : 0);
	
	
	printf("\nTesting Remove\n");
	PrintTestResult(&test_num, (3 == SortedListSize(sol) ? 1 : 0));
	iter3 = SortedListRemove(iter3);
	PrintTestResult(&test_num, (2 == SortedListSize(sol) ? 1 : 0));
	iter2 = SortedListRemove(iter2);
	PrintTestResult(&test_num, (1 == SortedListSize(sol) ? 1 : 0));
	
	iter1 = SortedListRemove(SortedListBeginIter(sol));
	PrintTestResult(&test_num, (0 == SortedListSize(sol) ? 1 : 0));
	PrintTestResult(&test_num, (1 == SortedListIsEmpty(sol) ? 1 : 0));
	
	printf("\nTesting destroy\n");
	SortedListDestroy(sol);
	sol = SortedListCreate(&MatchLargerThen);
	SortedListInsert(sol, &num2);
	SortedListInsert(sol, &num1);
	SortedListInsert(sol, &num0);
	SortedListDestroy(sol);	
}

void PrintTestResult(size_t *test_num, int result)
{

	if (1 == result)
	{
		printf("test %lu \033[4;33mResult:\033[0m \033[5;42mSUCCESS\033[0m\n",\
		*test_num);
	}
	else
	{
		printf("test %lu \033[4;33mResult:\033[0m \033[5;41mFAILURE\033[0m\n",\
		*test_num);
	}
	++(*test_num);
}

int MatchLargerThen(const void *larger, const void *lower)
{
	return *(int *)larger - *(int *)lower;	
}

int AddNumbers(void *num1, void *num2)
{
	*(int *)num1 += *(int *)num2;
	return 0;
}

int MatchInt(void *num1, void *num2)
{
	return *(int *)num1 == *(int *)num2;
}


