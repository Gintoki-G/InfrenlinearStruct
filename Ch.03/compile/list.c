
#include "list.h"

USERDATA* AddNewNodeAtTail(int age, const char* pszName, const char* pszPhone)
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

	return pNewNode;
}

USERDATA* AddNewNodeAtHead(int age, const char* pszName, const char* pszPhone)
{
	USERDATA* pNewNode = calloc(1, sizeof(USERDATA));
	pNewNode->age = age;
	strcpy_s(pNewNode->name, sizeof(pNewNode->name), pszName);
	strcpy_s(pNewNode->phone, sizeof(pNewNode->phone), pszPhone);

	//pNewNode 정리
	USERDATA* pNextNode = g_HeadNode.pNext;
	pNewNode->pPrev = &g_HeadNode;
	pNewNode->pNext = pNextNode;

	//pNewNode 앞뒤 정리
	pNextNode->pPrev = pNewNode;
	g_HeadNode.pNext = pNewNode;

	return pNewNode;
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

int IsEmpty(void)
{
	if (g_TailNode.pPrev == &g_HeadNode)
		return 1;

	return 0;
}

void InitList(void)
{
	g_HeadNode.pNext = &g_TailNode;
	g_TailNode.pPrev = &g_HeadNode;
}
