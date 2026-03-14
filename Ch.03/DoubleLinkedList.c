#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

typedef struct USERDATA
{
	int age;
	char name[32];
	char phone[32];
	struct USERDATA* pPrev;
	struct USERDATA* pNext;
} USERDATA;

//USERDATA* g_pHeadNode = NULL;
USERDATA g_HeadNode = { 0, "__Dummy Head__" };
USERDATA g_TailNode = { 0, "__Dummy Tail__" };


void AddNewNodeAtTail(int age, const char* pszName, const char* pszPhone)
{
	USERDATA* pNewNode = calloc(1, sizeof(USERDATA));
	pNewNode->age = age;
	strcpy_s(pNewNode->name, sizeof(pNewNode->name), pszName);
	strcpy_s(pNewNode->phone, sizeof(pNewNode->phone), pszPhone);

	//pNewNode 정리
	USERDATA* pPrevNode = g_TailNode.pPrev;
	pNewNode->pPrev = g_TailNode.pPrev;
	pNewNode->pNext = &g_TailNode;

	//pNewNode 앞뒤 정리
	pPrevNode->pNext = pNewNode;
	g_TailNode.pPrev = pNewNode;
}

// 리스트 초기화
void InitList(void)
{
	g_HeadNode.pNext = &g_TailNode;
	g_TailNode.pPrev = &g_HeadNode;
}

void ReleaseList(void)
{
	USERDATA* pTmp = g_HeadNode.pNext;
	USERDATA* pDelete;
	while (pTmp != NULL && pTmp != &g_TailNode)
	{
		pDelete = pTmp;
		pTmp = pTmp->pNext;

		printf("Delete: [%p] %d, %s, %s [%p]\n", pDelete,
			pDelete->age, pDelete->name, pDelete->phone, pDelete->pNext);

		free(pDelete);
	}

	InitList();
}

void InitDummyData(void)
{
	//Input
	AddNewNodeAtTail(10, "Hoon", "010-1111-1111");
	AddNewNodeAtTail(11, "Choi", "010-2222-2222");
	AddNewNodeAtTail(13, "Jang", "010-3333-3333");
}

// 노드를 반환함 
USERDATA* SearchByName(const char* pszName)
{
	USERDATA* pTmp = g_HeadNode.pNext;
	while (pTmp != NULL)
	{
		if (strcmp(pTmp->name, pszName) == 0)
		{
			printf("\"%s\": Found\n", pszName);
			return pTmp;
		}

		pTmp = pTmp->pNext;
	}

	printf("\"%s\": Not found\n", pszName);
	return NULL;
}

/*
RemoveNode 사용법
	USERDATA* p = SearchByName("Choi");
	if(p)
		RemoveNode(p);
*/
void RemoveNode(USERDATA* pRemove)
{
	USERDATA* pPrev = pRemove->pPrev;
	USERDATA* pNext = pRemove->pNext;

	pPrev->pNext = pRemove->pNext;
	pNext->pPrev = pRemove->pPrev;

	printf("RemoveNode(): %s\n", pRemove->name);
	free(pRemove);
}

//내가 만든 함수
int RemoveByName(const char* pszName)
{
	USERDATA* pNode = SearchByName(pszName);

	if (pNode == NULL)
		return 0;

	RemoveNode(pNode);
	return 1;
}

void PrintList(void)
{
	USERDATA* pTmp = &g_HeadNode;
	while (pTmp != NULL)
	{
		printf("[%p] %d, %s, %s [%p]\n", pTmp,
			pTmp->age, pTmp->name, pTmp->phone, pTmp->pNext);
		pTmp = pTmp->pNext;
	}
	putchar('\n');
}

void PrintListReverse(void)
{
	USERDATA* pTmp = g_TailNode.pPrev;
	while (pTmp != NULL)
	{
		printf("[%p] %d, %s, %s [%p]\n", pTmp,
			pTmp->age, pTmp->name, pTmp->phone, pTmp->pNext);
		pTmp = pTmp->pPrev;
	}
	putchar('\n');
}

void TestStep01(void)
{
	puts("TestStep01()----------------------------");
	AddNewNodeAtTail(10, "Choi", "010-2222-2222");
	AddNewNodeAtTail(10, "Hoon", "010-1111-1111");
	AddNewNodeAtTail(13, "Jang", "010-3333-3333");
	PrintList();

	USERDATA* pPrev = SearchByName("Choi");
	if (pPrev != NULL)
		RemoveNode(pPrev);

	PrintListReverse();
	ReleaseList();
	putchar('\n');
}

void TestStep02(void)
{
	puts("TestStep02()----------------------------");
	AddNewNodeAtTail(10, "Choi", "010-2222-2222");
	AddNewNodeAtTail(10, "Hoon", "010-1111-1111");
	AddNewNodeAtTail(13, "Jang", "010-3333-3333");
	PrintList();

	USERDATA* pPrev = SearchByName("Hoon");
	if (pPrev != NULL)
		RemoveNode(pPrev);
	ReleaseList();
	putchar('\n');
}

void TestStep03(void)
{
	puts("TestStep03()----------------------------");
	AddNewNodeAtTail(10, "Choi", "010-2222-2222");
	AddNewNodeAtTail(10, "Hoon", "010-1111-1111");
	AddNewNodeAtTail(13, "Jang", "010-3333-3333");
	PrintList();

	USERDATA* pPrev = SearchByName("Jang");
	if (pPrev != NULL)
		RemoveNode(pPrev);
	ReleaseList();
	putchar('\n');
}

int main(void)
{
	InitList();
	TestStep01();
	TestStep02();
	TestStep03();
	return 0;
}

