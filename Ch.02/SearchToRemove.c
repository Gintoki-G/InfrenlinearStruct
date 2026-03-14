#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

typedef struct USERDATA
{
	int age;
	char name[32];
	char phone[32];
	struct USERDATA* pNext;
} USERDATA;

USERDATA* g_pHeadNode = NULL;


void AddNewNode(int age, const char* pszName, const char* pszPhone)
{
	USERDATA* pNewNode = (USERDATA*)malloc(sizeof(USERDATA));
	pNewNode->age = age;
	strcpy_s(pNewNode->name, sizeof(pNewNode->name), pszName);
	strcpy_s(pNewNode->phone, sizeof(pNewNode->phone), pszPhone);
	pNewNode->pNext = NULL;

	if (g_pHeadNode == NULL)
		g_pHeadNode = pNewNode;
	else
	{
		USERDATA* pTail = g_pHeadNode;
		while (pTail->pNext != NULL)
			pTail = pTail->pNext;

		pTail->pNext = pNewNode;

		//Stack
		//pNewNode->pNext = g_pHeadNode;
		//g_pHeadNode = pNewNode;
	}
}

void ReleaseList(void)
{
	USERDATA* pTmp = g_pHeadNode;
	USERDATA* pDelete;
	while (pTmp != NULL)
	{
		pDelete = pTmp;
		pTmp = pTmp->pNext;

		printf("Delete: [%p] %d, %s, %s [%p]\n", pDelete,
			pDelete->age, pDelete->name, pDelete->phone, pDelete->pNext);

		free(pDelete);
	}

	g_pHeadNode = NULL;
}

void InitDummyData(void)
{
	//Input
	AddNewNode(10, "Hoon", "010-1111-1111");
	AddNewNode(11, "Choi", "010-2222-2222");
	AddNewNode(13, "Jang", "010-3333-3333");
}

USERDATA* SearchByName(const char* pszName)
{
	USERDATA* pTmp = g_pHeadNode;
	while (pTmp != NULL)
	{
		if (strcmp(pTmp->name, pszName) == 0)
		{
			printf("\"%s\": Found", pszName);
			return pTmp;
		}

		pTmp = pTmp->pNext;
	}

	printf("\"%s\": Not found", pszName);
	return NULL;
}

USERDATA* SearchToRemove(USERDATA** ppPrev, const char* pszName)
{
	// ppPrev : 포인터에 대한 포인터
	// 삭제할 노드의 이전 노드를 항상 백업해둬야 
	// 삭제할 노드의 이후 노드와 연결한다.
	// ex) [1] -> [2] -> [3] -> [4] -> NULL
	//			          pPrev   pCur
	//			            |      |
	// 삭제 이전    [1] -> [2] -> [3] -> [4] -> NULL
	// 
	// 삭제 이후    [1] -> [2] -> [4] -> NULL
	// pPrev.pNext 가 [4]의 주소를 가르켜야 함
	USERDATA* pCurrent = g_pHeadNode;
	USERDATA* pPrev = NULL;
	while (pCurrent != NULL)
	{
		if (strcmp(pCurrent->name, pszName) == 0)
		{
			*ppPrev = pPrev;
			return pCurrent;
		}
		pPrev = pCurrent;
		pCurrent = pCurrent->pNext;
	}
	return NULL;
}

void RemoveNode(USERDATA* pPrev)
{
	// 삭제할 노드의 이전 노드를 항상 백업해둬야 
	// 삭제할 노드의 이후 노드와 연결한다.
	// ex) [1] -> [2] -> [3] -> [4] -> NULL
	//			          pPrev   pCur
	//			            |      |
	// 삭제 이전    [1] -> [2] -> [3] -> [4] -> NULL
	// 
	// 삭제 이후    [1] -> [2] -> [4] -> NULL
	// pPrev.pNext 가 [4]의 주소를 가르켜야 함
	USERDATA* pRemove = NULL;
	if (pPrev == NULL) // 삭제할 노드가 헤드노드일 경우
	{
		if (g_pHeadNode == NULL)// 삭제할 노드가 없을 경우
			return;
		else
		{
			pRemove = g_pHeadNode;
			g_pHeadNode = pRemove->pNext;
			printf("RemoveNode(): %s\n", pRemove->name);
			free(pRemove);
		}

		return;
	}
	// 원래부터 prev 노드의 다음 노드를 삭제할 계획
	pRemove = pPrev->pNext; 
	pPrev->pNext = pRemove->pNext;
	printf("RemoveNode(): %s\n", pRemove->name);
	free(pRemove);
}

void PrintList(void)
{
	USERDATA* pTmp = g_pHeadNode;
	while (pTmp != NULL)
	{
		printf("[%p] %d, %s, %s [%p]\n", pTmp,
			pTmp->age, pTmp->name, pTmp->phone, pTmp->pNext);
		pTmp = pTmp->pNext;
	}
	putchar('\n');
}

void TestStep01(void)
{
	puts("TestStep01()----------------------------");
	AddNewNode(10, "Choi", "010-2222-2222");
	AddNewNode(10, "Hoon", "010-1111-1111");
	AddNewNode(13, "Jang", "010-3333-3333");
	PrintList();

	USERDATA* pPrev = NULL;
	if (SearchToRemove(&pPrev, "Choi") != NULL)
		RemoveNode(pPrev);
	ReleaseList();
	putchar('\n');
}

void TestStep02(void)
{
	puts("TestStep02()----------------------------");
	AddNewNode(10, "Choi", "010-2222-2222");
	AddNewNode(10, "Hoon", "010-1111-1111");
	AddNewNode(13, "Jang", "010-3333-3333");
	PrintList();

	USERDATA* pPrev = NULL;
	if (SearchToRemove(&pPrev, "Hoon") != NULL)
		RemoveNode(pPrev);
	ReleaseList();
	putchar('\n');
}

void TestStep03(void)
{
	puts("TestStep03()----------------------------");
	AddNewNode(10, "Choi", "010-2222-2222");
	AddNewNode(10, "Hoon", "010-1111-1111");
	AddNewNode(13, "Jang", "010-3333-3333");
	PrintList();

	USERDATA* pPrev = NULL;
	if (SearchToRemove(&pPrev, "Jang") != NULL)
		RemoveNode(pPrev);
	ReleaseList();
	putchar('\n');
}

int main(void)
{
	TestStep01();
	TestStep02();
	TestStep03();
	return 0;
}

