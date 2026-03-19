#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "singleList.h"

MYNODE g_HeadNode = { 0 };
MYNODE g_TailNode = { 0 };
static unsigned int g_listCount = 0;


void InitList(void)
{
	ReleaseList();
	g_HeadNode.pNext = &g_TailNode;
	g_TailNode.pPrev = &g_HeadNode;
	g_listCount = 0;
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

	MYNODE* pTmp = g_HeadNode.pNext;
	MYNODE* pDelete;
	while (pTmp != &g_TailNode)
	{
		pDelete = pTmp;
		pTmp = pTmp->pNext;
		free(pDelete->pData);
		free(pDelete);
	}

	// 모든 노드의 메모리가 해제된 후 
	// 노드 초기화
	g_HeadNode.pNext = &g_TailNode;
	g_TailNode.pPrev = &g_HeadNode;
	g_listCount = 0;
}

// void*로 들어온 데이터를 USERDATA로
// 바꿔서 name을 꺼내서 반환한다
char* GetKey(void* thisPointer)
{
	USERDATA* pUser = (USERDATA*)thisPointer;
	return pUser->name;
	//return pUser->phone;
}

// USERDATA 데이터를 만들고 → 노드를 만들고 
// → 리스트 맨 뒤에 붙인다
void AddNewNode(int age, char* pszName, char* pszPhone)
{
	//데이터 생성
	//pNewData -> [ age | name | phone ]
	USERDATA* pNewData = calloc(1, sizeof(USERDATA));
	pNewData->age = age;
	strcpy_s(pNewData->name, sizeof(pNewData->name), pszName);
	strcpy_s(pNewData->phone, sizeof(pNewData->phone), pszPhone);

	//노드 생성
	// pNewNode -> [ pData | pPrev | pNext | GetKey ]
	//				   |
	//             [USERDATA]	
	MYNODE* pNewNode = calloc(1, sizeof(MYNODE));
	pNewNode->pData = pNewData;
	// 이 노드의 GetKey에다가, GetKey 함수 주소를 넣어라
	pNewNode->GetKey = GetKey;

	MYNODE* pPrevNode = g_TailNode.pPrev;
	pPrevNode->pNext = pNewNode;
	pNewNode->pPrev = pPrevNode;
	pNewNode->pNext = &g_TailNode;
	g_TailNode.pPrev = pNewNode;

	++g_listCount;
}


void SwapNode(MYNODE* pLeft, MYNODE* pRight)
{
	void* pTmp = pLeft->pData;
	pLeft->pData = pRight->pData;
	pRight->pData = pTmp;
}

void SortListByAge(void)
{
	if (IsEmpty())
		return;

	MYNODE* pTmp = g_HeadNode.pNext;
	MYNODE* pSelected = NULL;
	MYNODE* pCmp = NULL;
	while (pTmp != NULL && pTmp != g_TailNode.pPrev)
	{
		pSelected = pTmp;
		pCmp = pTmp->pNext;
		while (pCmp != NULL && pCmp != &g_TailNode)
		{
			if (strcmp(
					pSelected->GetKey(pSelected->pData),
					pCmp->GetKey(pCmp->pData)) > 0)
				pSelected = pCmp;

			pCmp = pCmp->pNext;
		}

		if (pTmp != pSelected)
			SwapNode(pTmp, pSelected);

		pSelected = NULL;
		pTmp = pTmp->pNext;
	}
}