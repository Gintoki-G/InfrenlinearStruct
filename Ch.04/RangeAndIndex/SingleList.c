#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "singleList.h"

USERDATA g_HeadNode = { 0, "_DummyHead_" };
USERDATA g_TailNode = { 0, "_DummyTail_" };
static unsigned int g_listCount = 0;

unsigned int GetListCount(void) {
	return g_listCount;
}

// 리스트의 개수를 반환하는 함수
unsigned int RecalListCount(void) {
	unsigned int cnt = 0;
	USERDATA* pTmp = g_HeadNode.pNext;

	while (pTmp != &g_TailNode) {
		++cnt;
		pTmp = pTmp->pNext;
	}
	g_listCount = cnt;
	return g_listCount;
}

// 초기화에 ReleaseList()를 넣었다.
void InitList(void) {
	ReleaseList();
	g_HeadNode.pNext = &g_TailNode;
	g_TailNode.pPrev = &g_HeadNode;
	g_listCount = 0;
}

int IsEmpty(void) {
	if (g_HeadNode.pNext == &g_TailNode
		|| g_HeadNode.pNext == NULL) {
		return 1;
	}
	return 0;
}

// 전체 노드를 헤드 노드의 다음 노드부터 다 해제한다.
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
	g_HeadNode.pNext = &g_TailNode;
	g_TailNode.pPrev = &g_HeadNode;
	g_listCount = 0;

}

// 오른쪽의 노드를 왼쪽에 복사한다.
void NodeDataCopy(USERDATA* pLeft, USERDATA* pRight) {
	pLeft->age = pRight->age;
	strcpy_s(pLeft->name, sizeof(pLeft->name), pRight->name);
	strcpy_s(pLeft->phone, sizeof(pLeft->phone), pRight->phone);
}

// 왼쪽 노드와 오른쪽 노드를 swap 한다.
void SwapNode(USERDATA* pLeft, USERDATA* pRight) {
	USERDATA tmp = *pLeft;
	NodeDataCopy(pLeft, pRight);
	NodeDataCopy(pRight, &tmp);
}

void SortListByName(void) {
	if (IsEmpty()) {
		return;
	}

	USERDATA* pTmp = g_HeadNode.pNext;
	USERDATA* pSelected = NULL;
	USERDATA* pCmp = NULL;

	/*
		첫 정렬 : [Charlie] [Alice] [Bob]
		1. pTmp = Chaelie (pSeleted 이기도 함)
		2. 최소 찾기 : Alice {if (strcmp(pSelected->name, pCmp->name) > 0)}
		3. swap [Alice] [Charlie] [Bob]
		
	*/ 

	// 선택 정렬을 한다
	while (pTmp != NULL && pTmp != g_TailNode.pPrev) {
		// pSeleted : 정렬중인 위치의 노드 , 바깥 while문을
		//			  돌 때마다 다음 노드로 넘어간다.
		// 
		// pCmp : 정렬중인 위치의 다음 노드로 안의 while문을
		//        돌 때마다 다음 노드로 넘어간다.
		pSelected = pTmp;
		pCmp = pTmp->pNext;

		while (pCmp != NULL && pCmp != &g_TailNode) {

			// if문 : 이름을 알파벳 순으로 정렬
			// pSeleted 안의 이름이 pCmp 안의 이름보다 빠르면 
			// pSeleted의 노드 전체에 pCmp의 노드 전체를 넣는다.
			if (strcmp(pSelected->name, pCmp->name) > 0) {
				pSelected = pCmp;
			}
			pCmp = pCmp->pNext;
		}

		if (pTmp != pSelected) {
			SwapNode(pTmp, pSelected);
		}

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


/*----------------SearchList----------------------------------*/
// 선형 탐색 + 메모리Copy
int SearchListByName(USERDATA* pUser, char* pszName) {
	USERDATA* pTmp = g_HeadNode.pNext;
	while (pTmp != &g_TailNode) {
		if (strcmp(pTmp->name, pszName) == 0) {
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

int SearchListByAge(int age) {
	USERDATA* pTmp = g_HeadNode.pNext;
	while (pTmp != &g_TailNode) {
		if (pTmp->age == age) {
			return 1;
		}
		pTmp = pTmp->pNext;
	}
	return 0;
}
/*------------------------------------------------------------*/

int RemoveByName(char* pszName) {
	USERDATA* pCur = g_HeadNode.pNext;
	USERDATA* pNextNode;
	USERDATA* pPrevNode;
	
	while (pCur != NULL && pCur != &g_TailNode) {
		
		if (strcmp(pCur->name, pszName) == 0) {
			pNextNode = pCur->pNext;
			pPrevNode = pCur->pPrev;

			pNextNode->pPrev = pCur->pPrev;
			pPrevNode->pNext = pCur->pNext;

			free(pCur);
			--g_listCount;
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

	// D.T에 새로운 노드 추가
	USERDATA* pPrevNode = g_TailNode.pPrev;
	pPrevNode->pNext = pNewNode;
	pNewNode->pPrev = pPrevNode;
	pNewNode->pNext = &g_TailNode;
	g_TailNode.pPrev = pNewNode;

	++g_listCount;
}

void** SearchByAgeRange(int min, int max, int* pCount) {
	*pCount = 0;
	USERDATA* pMin = NULL;
	USERDATA* pMax = NULL; 
	USERDATA* pTmp = g_HeadNode.pNext;

	//최소값 위치 찾기 (min 이상 첫 노드)
	//min 이상 처음 등장하는 노드 = pMin
	while (pTmp != &g_TailNode) {
		if (pTmp->age >= min) {
			pMin = pTmp;
			break;
		}
		pTmp = pTmp->pNext;
	}

	if (pMin != NULL) {
		//pMin 찾았으면 → 그 다음부터 탐색
		pTmp = pMin->pNext;
	}
	else {
		//없으면 → 처음부터 다시
		pTmp = g_HeadNode.pNext;
	}

	//max보다 작거나 같은 동안 계속 가다가, 
	//처음으로 max를 넘는 순간 멈춘다
	//max 이하인 마지막 노드(pMax)를 찾는 핵심 코드
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

		//pMin ~ pMax까지 노드 개수 계산
		while (pTmp != pMax) {
			++cnt;
			pTmp = pTmp->pNext;
		}

		*pCount = cnt;
		// 결과를 담을 배열 생성(노드 "복사"가 아니라 "주소" 저장)
		void** pNodePtrList = malloc(sizeof(void*) * cnt);
		
		//pMin부터 pMax까지 순서대로 저장
		pTmp = pMin;
		int i = 0;
		for (; pTmp != pMax; i++) {
			pNodePtrList[i] = pTmp;
			pTmp = pTmp->pNext;
		}
		pNodePtrList[i] = pMax;
		return pNodePtrList;
	}

	return NULL;
}

// 정렬되는건 aList 배열이지 리스트가 아니다
void** MakeIndexAge(int* pCnt) {
	*pCnt = 0;
	if (IsEmpty()) {
		return NULL;
	}
	// USERDATA** → 노드 "포인터 배열"
	// 노드를 복사하는 게 아니라 주소
	USERDATA** aList;
	aList = malloc(sizeof(USERDATA*) * GetListCount());
	memset(aList, 0, sizeof(USERDATA*) * GetListCount());
	*pCnt = GetListCount();

	//리스트 → 배열 복사
	USERDATA* pTmp = g_HeadNode.pNext;
	for (int i = 0; pTmp != &g_TailNode; ++i) {
		aList[i] = pTmp;
		pTmp = pTmp->pNext;
	}

	// aList 버블 정렬
	for (unsigned int i = 0; i < GetListCount() - 1; ++i) {
		for (unsigned int j = i + 1; j < GetListCount(); ++j) {
			if (aList[i]->age > aList[i]->age) {
				USERDATA* pTmp = aList[i];
				aList[i] = aList[j];
				aList[j] = pTmp;
			}
		}
	}

	return aList;
}

void** SearchByIndexAgeRange(int min, int max, unsigned int* pCount) {
	
	/*
	MakeIndexAge() :나이를 기준으로 정렬된 USERDATA 포인터 배열 생성
	cntTotal : 전체 데이터 개수
	aList : 정렬된 USERDATA* 배열의 주소
	*/
	*pCount = 0;
	unsigned int cntTotal = 0;
	USERDATA** aList = (USERDATA**)MakeIndexAge(&cntTotal);

	int idxMin = -1, idxMax = 0;
	unsigned int i = 0;

	for (i = 0; i < cntTotal; i++) {

		//min <= age <= max
		// 처음 만족하는 위치를 찾으면
		// idxMin = 시작 인덱스
		// idxMax = 일단 같은 값으로 시작
		// >> 범위에 들어오는 첫번째 위치 찾기
		if (aList[i]->age >= min && aList[i]->age <= max) {
			idxMin = i;
			idxMax = i;
			break;
		}
	}

	if (idxMin >= 0) {
		for (; i < cntTotal; ++i) {
			/*
			이미 정렬되어 있기 때문에
			age <= max : 계속 범위안
			age > max : 범위 끝
			>> 연속된 구간의 끝을 찾는 구조
			*/
			if (aList[i]->age <= max) {
				idxMax = i;
			}
			else if (aList[i]->age > max) {
				break;
			}
		}

		/*
		length : 선택된 데이터 개수
		aSelected : 결과 배열
		memcpy : 해당 범위만 복사
		*/
		int length = idxMax - idxMin + 1;
		USERDATA ** aSelected = malloc(sizeof(void*) * length);
		memcpy(aSelected, aList + idxMin, sizeof(void*) * length);
		
		//기존 전체 배열 aList는 해제
		free(aList);

		*pCount = length;
		return aSelected;

	}

	//조건 만족하는 데이터가 하나도 없으면 NULL 반환
	free(aList);
	return NULL;
}