#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "singleList.h"


USERDATA g_HeadNode = { 0, "_DummyHead_" };
USERDATA g_TailNode = { 0, "_DummyTail_" };


void InitList(void)
{
	ReleaseList();
	g_HeadNode.pNext = &g_TailNode;
	g_TailNode.pPrev = &g_HeadNode;
}

int IsEmpty(void)
{
	if (g_HeadNode.pNext == &g_TailNode ||
		g_HeadNode.pNext == NULL)
		return 1;

	return 0;
}

void ReleaseList(void)
{
	if (IsEmpty())
		return; 

	USERDATA* pTmp = g_HeadNode.pNext;
	USERDATA* pDelete;
	while (pTmp != &g_TailNode)
	{
		pDelete = pTmp;
		pTmp = pTmp->pNext;

		printf("Delete: %d, %s, %s\n",
			pDelete->age, pDelete->name, pDelete->phone);

		free(pDelete);
	}

	g_HeadNode.pNext = &g_TailNode;
	g_TailNode.pPrev = &g_HeadNode;
}


void NodeDataCopy(USERDATA* pLeft, USERDATA* pRight)
{
	pLeft->age = pRight->age;
	strcpy_s(pLeft->name, sizeof(pLeft->name), pRight->name);
	strcpy_s(pLeft->phone, sizeof(pLeft->phone), pRight->phone);
}

void SwapNode(USERDATA* pLeft, USERDATA* pRight)
{
	USERDATA tmp = *pLeft;
	NodeDataCopy(pLeft, pRight);
	NodeDataCopy(pRight, &tmp);
}

void SortListByName(void)
{
	if (IsEmpty())
		return;

	USERDATA* pTmp = g_HeadNode.pNext;
	USERDATA* pSelected = NULL;
	USERDATA* pCmp = NULL;
	while (pTmp != NULL && pTmp != g_TailNode.pPrev)
	{
		pSelected = pTmp;
		pCmp = pTmp->pNext;
		while (pCmp != NULL && pCmp != &g_TailNode)
		{
			if (strcmp(pSelected->name, pCmp->name) > 0)
				pSelected = pCmp;

			pCmp = pCmp->pNext;
		}

		if (pTmp != pSelected)
			SwapNode(pTmp, pSelected);

		pSelected = NULL;
		pTmp = pTmp->pNext;
	}
}

void SortListByAge(void)
{
	if (IsEmpty())
		return;

	//정렬 위치
	USERDATA* pTmp = g_HeadNode.pNext;
	USERDATA* pSelected = NULL;
	USERDATA* pCmp = NULL;

	// pTmp가 NULL이 아니고 노드가 있을 것
	while (pTmp != NULL && pTmp != g_TailNode.pPrev)
	{
		pSelected = pTmp;

		// 정렬할 위치의 다음부터 비교를 시작
		pCmp = pTmp->pNext;
		while (pCmp != NULL && pCmp != &g_TailNode)
		{
			// 정렬할 위치의 값과 다음의 값을 비교
			if (pSelected->age > pCmp->age)
				pSelected = pCmp;

			// 다음 노드로 옮김
			pCmp = pCmp->pNext;
		}

		// 정렬할 위치와 pSeleted의 위치가 다르다면
        // Swap한다.
		if (pTmp != pSelected)
			SwapNode(pTmp, pSelected);

		// 초기화
		pSelected = NULL;
		// 정렬할 위치를 다음으로 넘김
		pTmp = pTmp->pNext;
	}
}

int SearchListByName(USERDATA* pUser, char* pszName)
{
	USERDATA* pTmp = g_HeadNode.pNext;
	while (pTmp != &g_TailNode)
	{
		if (strcmp(pTmp->name, pszName) == 0)
		{
			memcpy(pUser, pTmp, sizeof(USERDATA));
			return 1;
		}

		pTmp = pTmp->pNext;
	}

	return 0;
}

int SearchListByPhone(USERDATA* pUser, char* pszPhone)
{
	USERDATA* pTmp = g_HeadNode.pNext;
	while (pTmp != &g_TailNode)
	{
		if (strcmp(pTmp->phone, pszPhone) == 0)
		{
			memcpy(pUser, pTmp, sizeof(USERDATA));
			return 1;
		}

		pTmp = pTmp->pNext;
	}

	return 0;
}

int RemoveByName(char* pszName)
{
	USERDATA* pCur = g_HeadNode.pNext;
	USERDATA* pNextNode;
	USERDATA* pPrevNode;
	while (pCur != NULL && pCur != &g_TailNode)
	{
		if (strcmp(pCur->name, pszName) == 0)
		{
			pNextNode = pCur->pNext;
			pPrevNode = pCur->pPrev;

			pNextNode->pPrev = pCur->pPrev;
			pPrevNode->pNext = pCur->pNext;

			free(pCur);
			return 1;
		}

		pCur = pCur->pNext;
	}

	return 0;
}

void AddNewNode(int age, char* pszName, char* pszPhone)
{
	USERDATA* pNewNode = calloc(1, sizeof(USERDATA));
	pNewNode->age = age;
	strcpy_s(pNewNode->name, sizeof(pNewNode->name), pszName);
	strcpy_s(pNewNode->phone, sizeof(pNewNode->phone), pszPhone);

	USERDATA* pPrevNode = g_TailNode.pPrev;
	pPrevNode->pNext = pNewNode;
	pNewNode->pPrev = pPrevNode;
	pNewNode->pNext = &g_TailNode;
	g_TailNode.pPrev = pNewNode;
}

int SearchListByAge(int age)
{
	USERDATA* pTmp = g_HeadNode.pNext;
	while (pTmp != &g_TailNode)
	{
		if (pTmp->age == age)
			return 1;

		pTmp = pTmp->pNext;
	}

	return 0;
}

void** SearchByAgeRange(int min, int max, int* pCount)
{
	*pCount = 0;
	USERDATA* pMin = NULL;
	USERDATA* pMax = NULL;
	USERDATA* pTmp = g_HeadNode.pNext;
	while (pTmp != &g_TailNode)
	{
		/*
		 선형 탐색을 헤드 노드의 다음 노드부터 시작
		조건에 맞는 pTmp를 찾으면 바로 break를
		함으로써 min을 한참 넘어서는 pTmp를 가질 순 없다
		[D.H] -> [56] -> [59] -> [60] -> [60] - > [D.T]
		만약 min = 58 이면
		pTmp가 59를 만나는 순간 break가 된다.
		*/
		if (pTmp->age >= min)
		{
			pMin = pTmp;
			pMax = pTmp;
			break;
		}
		pTmp = pTmp->pNext;
	}

	if (pMin != NULL)
		pTmp = pMin->pNext;
	else
		// pMin을 찾지 못했다
		pTmp = g_HeadNode.pNext;

	while (pTmp != &g_TailNode)
	{
		if (pTmp->age >= min && pTmp->age <= max)
			pMax = pTmp;
		else if (pTmp->age > max)
			/*
			min과 마찬가지로 조건이 맞으면
			while문을 빠져 나온다

			[D.H] -> [54] -> [56] -> [70] -> [D.T]
			만약 max = 58이면 70을 만나자마가
			break를 한다.
			*/
			break;

		pTmp = pTmp->pNext;
	}

	if (pMin != NULL && pMax != NULL)
	{
		USERDATA* pTmp = pMin;
		int cnt = 1;

		// 조건에 맞는 노드가 몇개인지 세는 while문
		while (pTmp != pMax)
		{
			++cnt;

			// 이미 정렬이 되어있기에 선형 탐색이 가능
			pTmp = pTmp->pNext;
		}

		*pCount = cnt;

		// 자료형이 void *인 배열의 주소가
		// pNodePtrList이다. 
		// 그래서 배열에 min,max에 맞는 노드를
		// 넣을 수 있다.
		void** pNodePtrList = malloc(sizeof(void*) * cnt);

		pTmp = pMin;
		int i = 0;
		for (; pTmp != pMax; ++i)
		{
			pNodePtrList[i] = pTmp;
			// 정렬이 되어있기에 가능한 노드
			pTmp = pTmp->pNext;
		}
		pNodePtrList[i] = pMax;

		return pNodePtrList;
	}

	return NULL;
}
