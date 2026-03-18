#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "singleList.h"

USERDATA g_HeadNode = { 0, "_DummyHead_" };
USERDATA g_TailNode = { 0, "_DummyTail_" };
static unsigned int g_listCount = 0;

// 인덱스에 대한 리스트를 미리 정의해둔다.
USERDATA** g_idxListAge = NULL;
USERDATA** g_idxListName = NULL;

/*-----------------ListCount------------------*/
unsigned int GetListCount(void) {
	return g_listCount;
}

unsigned int RecalcListCount(void) {
	unsigned int cnt = 0;
	USERDATA* pTmp = g_HeadNode.pNext;
	while (pTmp != &g_TailNode) {
		++cnt;
		pTmp = pTmp->pNext;
	}
	g_listCount = cnt;

	return g_listCount;
}
/*---------------------------------------------*/

void InitList() {
	ReleaseList();
	g_HeadNode.pNext = &g_TailNode;
	g_TailNode.pPrev = &g_HeadNode;
	g_listCount = 0;

	g_idxListAge = NULL;
	g_idxListName = NULL;
}

int IsEmpty(void) {
	if (g_HeadNode.pNext == &g_TailNode ||
		g_HeadNode.pNext == NULL) {
		return 1;
	}
	return 0;
}

void ReleaseList(void) {
	if (IsEmpty()) {
		return;
	}

	USERDATA* pTmp = g_HeadNode.pNext;
	USERDATA* pDelete;
	while (pTmp != &g_TailNode) {
		pDelete = pTmp;
		pTmp = pTmp->pNext;

		printf("Delete: %d, %s, %s\n",
			pDelete->age, pDelete->name, pDelete->phone);
		
		free(pDelete);
	}

	// 노드 초기화
	g_HeadNode.pNext = &g_TailNode;
	g_TailNode.pPrev = &g_HeadNode;
	g_listCount = 0;

	// 인덱스 초기화
	if (g_idxListAge != NULL) {
		free(g_idxListAge);
		g_idxListAge = NULL;
	}

	if (g_idxListName != NULL) {
		free(g_idxListName);
		g_idxListName = NULL;
	}

}

/*-----------------NodeCopy & Swap---------------------*/
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
/*------------------------------------------------------*/

/*------------------SortListByName/age------------------*/
void SortListByName(void) {
	if (IsEmpty()) {
		return;
	}

	USERDATA* pTmp = g_HeadNode.pNext;
	USERDATA* pSelected = NULL;
	USERDATA* pCmp = NULL;

	// 선택 정렬
	while (pTmp != NULL && pTmp != g_TailNode.pPrev) {
		pSelected = pTmp;
		pCmp = pTmp->pNext;
		while (pCmp != NULL && pCmp != &g_TailNode) {
			if (strcmp(pSelected->name, pCmp->name) > 0) {
				/*
				리스트 구조는 그대로,데이터도 그대로
				그냥 pSelected가 “더 작은 이름을 가진 노드를
				가리키게 된 것
				*/
				pSelected = pCmp;
			}

			pCmp = pCmp->pNext;
		}

		if (pTmp != pSelected) {
			/// 실제로 그 자리로 바꿔주는 것
			SwapNode(pTmp, pSelected);
		}
		// 새로운 정렬 위치를 넣기 위해
		pSelected = NULL;
		pTmp = pTmp->pNext;
	}
}

void SortListByAge(void)
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
			if (pSelected->age > pCmp->age)
				pSelected = pCmp;

			pCmp = pCmp->pNext;
		}

		if (pTmp != pSelected)
			SwapNode(pTmp, pSelected);

		pSelected = NULL;
		pTmp = pTmp->pNext;
	}
}
/*------------------------------------------------------*/


/*--------------SearchListByName/Phone/Age------------------*/
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
/*------------------------------------------------------------*/

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
			--g_listCount;
			UpdateIndexAll();
			return 1;
		}

		pCur = pCur->pNext;
	}

	return 0;
}

void AddNewNode(int age, char* pszName, char* pszPhone) {
	USERDATA* pNewNode = calloc(1, sizeof(USERDATA));
	pNewNode->age = age;
	strcpy_s(pNewNode->name, sizeof(pNewNode->name), pszName);
	strcpy_s(pNewNode->phone, sizeof(pNewNode->phone), pszPhone);

	USERDATA* pPrevNode = g_TailNode.pPrev;
	pPrevNode->pNext = pNewNode;
	pNewNode->pPrev = pPrevNode;
	pNewNode->pNext = &g_TailNode;
	g_TailNode.pPrev = pNewNode;

	++g_listCount;
	UpdateIndexAll();
}

/*---------------------------------------------------------*/
//SearchByAgeRange
// 리스트가 age 기준으로 정렬되어 있다고 가정한다.(중요)
// 자료 그 자체를 정렬!!
// min 이상이 처음 나오는 노드(pMin)와
// max 이하인 마지막 노드(pMax)를 찾아
// 그 구간의 노드 포인터들을 배열에 담아 반환한다.
void** SearchByAgeRange(int min, int max, int* pCount) {
	*pCount = 0;
	USERDATA* pMin = NULL;
	USERDATA* pMax = NULL;
	USERDATA* pTmp = g_HeadNode.pNext;

	while (pTmp != &g_TailNode) {
		if (pTmp->age >= min) {
			pMin = pTmp;
			break;
		}
		pTmp = pTmp->pNext;
	}

	if (pMin != NULL) {
		pTmp = pMin->pNext;
	}
	else {
		pTmp = g_HeadNode.pNext;
	}

	//pMax를 찾다가 넘어가는 pMax를 찾으면 바로 while문 중단
	while (pTmp != &g_TailNode) {
		if (pTmp->age <= max) {
			pMax = pTmp;
		}
		else if (pTmp->age > max) {
			break;
		}
		pTmp = pTmp->pNext;
	}

	if (pMin != NULL && pMax != NULL) {
		USERDATA* pTmp = pMin;
		int cnt = 1;
		while (pTmp != pMax) {
			++cnt;
			pTmp = pTmp->pNext;
		}
		*pCount = cnt;
		
		// 자료형이 void*인 배열의 주소는 pNOdePtrList
		void** pNodePtrList = malloc(sizeof(void*) * cnt);

		// pMin~pMax를 pNodePtrList에 넣음
		pTmp = pMin;
		int i = 0;

		for (; pTmp != pMax; ++i)
		{
			pNodePtrList[i] = pTmp;
			pTmp = pTmp->pNext;
		}
		pNodePtrList[i] = pMax;

		return pNodePtrList;
	}
	return NULL;
}

/*-----------------------------------------------------*/
/*
	SearcchByAgeRange : 리스트를 직접 순회해서 찾음
	SearchByIndexAgeRange : 정렬된 배열(인덱스)을 만들어서 찾음
*/

/*
목표: 나이가 min 이상 max 이하인 노드들만 뽑아 배열로 반환

반환값: USERDATA* 포인터 배열 (void* 배열)

pCount: 몇 개 찾았는지 알려주는 출력 변수
*/
void** SearchByIndexAgeRange(int min, int max, unsigned int* pCount) {
	*pCount = 0;
	unsigned int cntTotal = 0;
	USERDATA ** aList = (USERDATA**)MakeIndexAge(&cntTotal);

	int idxMin = -1, idxMax = 0;
	unsigned int i = 0;


	for (i = 0; i < cntTotal; ++i) {
		//혹시 첫 노드가 max를 초과하면 범위 안에 없음 
		// → 바로 idxMin을 찾지 않도록
		// aList[i]-> age <= max 라는 조건을 넣었다
		if (aList[i]->age >= min && aList[i]->age <= max) {
			idxMin = i;
			idxMax = i;
			break;
		}
	}

	if (idxMin >= 0) {
		//idxMax 찾기
		for (; i < cntTotal; i++) {
			if (aList[i]->age <= max) {
				idxMax = i;
			}
			else if(aList[i]->age > max) {
				break;
			}
		}
		int length = idxMax - idxMin + 1;
		USERDATA** aSelected = malloc(sizeof(void*) * length);

		//aList:   [0] [1] [2] [3] [4]
		//내용:    N0  N1  N2  N3  N4

		//	idxMin = 2
		//	aList + idxMin -> & aList[2]->N2
		//	memcpy(aSelected, aList + idxMin, 2 * sizeof(void*))
		//	->aSelected[0] = N2, aSelected[1] = N3
		memcpy(aSelected, aList + idxMin, sizeof(void*) * length);
		
		//aList는 임시 배열 → 필요 없으므로 해제
		free(aList);

		*pCount = length;
		return aSelected;
	}
	//aList는 임시 배열 → 필요 없으므로 해제
	free(aList);
	return NULL;
}


// 인덱스 생성 및 인덱스 정렬
void** MakeIndexName(int* pCnt) {
	*pCnt = 0;
	if (IsEmpty()) {
		return NULL;
	}

	/*인덱스 생성하기(동적할당)*/
	USERDATA** aList;
	aList = malloc(sizeof(USERDATA*) * GetListCount());
	memset(aList, 0, sizeof(USERDATA*) * GetListCount());
	*pCnt = GetListCount();

	USERDATA* pTmp = g_HeadNode.pNext;
	for (int i = 0; pTmp != &g_TailNode; ++i) {
		aList[i] = pTmp;
		pTmp = pTmp->pNext;
	}
	
	// 버블 정렬으로 인덱스를 정렬한다.
	for (unsigned int i = 0; i < GetListCount() - 1; ++i)
	{
		for (unsigned int j = i + 1; j < GetListCount(); ++j)
		{
			if (strcmp(aList[i]->name, aList[j]->name) > 0)
			{
				USERDATA* pTmp = aList[i];
				aList[i] = aList[j];
				aList[j] = pTmp;
			}
		}
	}
	return aList;
}

void** MakeIndexAge(int* pCnt)
{
	*pCnt = 0;
	if (IsEmpty())
		return NULL;

	USERDATA** aList;
	aList = malloc(sizeof(USERDATA*) * GetListCount());
	memset(aList, 0, sizeof(USERDATA*) * GetListCount());
	*pCnt = GetListCount();

	USERDATA* pTmp = g_HeadNode.pNext;
	for (int i = 0; pTmp != &g_TailNode; ++i)
	{
		aList[i] = pTmp;
		pTmp = pTmp->pNext;
	}

	for (unsigned int i = 0; i < GetListCount() - 1; ++i)
	{
		for (unsigned int j = i + 1; j < GetListCount(); ++j)
		{
			if (aList[i]->age > aList[j]->age)
			{
				USERDATA* pTmp = aList[i];
				aList[i] = aList[j];
				aList[j] = pTmp;
			}
		}
	}

	return aList;
}

// update시 인덱스 재정렬
void UpdateIndexAll(void) {
	if (g_idxListAge != NULL)
	{
		free(g_idxListAge);
		g_idxListAge = NULL;
	}

	if (g_idxListName != NULL)
	{
		free(g_idxListName);
		g_idxListName = NULL;
	}

	int cnt = 0;
	g_idxListAge = (USERDATA**)MakeIndexAge(&cnt);
	g_idxListName = (USERDATA**)MakeIndexName(&cnt);
}