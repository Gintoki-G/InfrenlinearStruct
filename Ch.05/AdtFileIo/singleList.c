#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "singleList.h"

MYNODE g_HeadNode = { 0 };
MYNODE g_TailNode = { 0 };
static unsigned int g_listCount = 0;

void InitList(void) {
	ReleaseList();
	g_HeadNode.pNext = &g_TailNode;
	g_TailNode.pPrev = &g_HeadNode;
	g_listCount = 0;
}

int IsEmpty(void) {
	if (g_HeadNode.pNext == &g_TailNode || g_HeadNode.pNext == NULL) {
		return 1;
	}
	return 0;
}

void ReleaseList(void) {
	if (IsEmpty()) {
		return;
	}

	MYNODE* pTmp = g_HeadNode.pNext;
	MYNODE* pDelete;
	while (pTmp != &g_TailNode) {
		pDelete = pTmp;
		pTmp = pTmp->pNext;
		
		//노드 안에 있는 동적 메모리도 같이 해제
		free(pDelete->pszKey);
		if (pDelete->pDataCache != NULL) {
			free(pDelete->pDataCache);
		}

		//노드 메모리 해제
		free(pDelete);
	}
	g_HeadNode.pNext = &g_TailNode;
	g_TailNode.pPrev = &g_HeadNode;
	g_listCount = 0;
}

char* GetKey(void* thisPointer) {
	USERDATA* pUser = (USERDATA*)thisPointer;
	return pUser->name;
	//return pUser->phone;
}

void AddNewNode(const char* pszKey, const void* pData, int sizeData,
	bool bNew, unsigned int offset) {

	MYNODE* pNewNode = calloc(1, sizeof(MYNODE));

	// lenOfKey : 키의 길이
	// 동적할당을 할 때 lenOfkey +1인 이유 
	// : 문자열은 항상 NULL을 넣는다 
	int lenOfKey = (int)strlen(pszKey);
	pNewNode->pszKey = malloc(lenOfKey + 1);
	strcpy_s(pNewNode->pszKey, lenOfKey + 1, pszKey);

	// Node의 Data가 pNewData이다.
	// 데이터 복사
	if (pData != NULL && sizeData > 0) {
		// sizeData : pNewData의 크기
		// 'void *'라서 어떤 데이터이든 가능하다
		void* pNewData = malloc(sizeData);

		//memcpy로 바이트 단위 복사
		memcpy(pNewData, pData, sizeData);
		pNewNode->pDataCache = pNewData;
		pNewNode->sizeofData = sizeData;
	}

	// 새 데이터 여부
	pNewNode->bNew = bNew;
	// 위치 정보
	pNewNode->offset = offset;

	// 새로운 노드가 추가되어서 노드 교통정리
	MYNODE* pPrevNode = g_TailNode.pPrev;
	pPrevNode->pNext = pNewNode;
	pNewNode->pPrev = pPrevNode;
	pNewNode->pNext = &g_TailNode;
	g_TailNode.pPrev = pNewNode;

	++g_listCount;
}

//키로 노드를 찾고, 없으면 파일에서 데이터를 읽어서 캐시에 채워주는 함수
void* SearchListByName(char* pszKey) {
	MYNODE* pTmp = g_HeadNode.pNext;
	while (pTmp != &g_TailNode) {

		//문자열 비교해서 같은 노드 찾기 : 같으면 0 반환 → 찾은 것
		if (strcmp(pTmp->pszKey, pszKey) == 0) {

			// 이미 캐시에 데이터가 있다면?
			// >> 이미 메모리에 데이터 있음 → 바로 반환
			// >> 파일 접근 안 함 → 빠름
			if (pTmp->pDataCache != NULL) {
				return pTmp;
			}


			// 캐시에 없으면 파일에서 읽기
			FILE* fp = NULL;
			fopen_s(&fp, "listData.dat", "rb");
			
			// offset : 몇번째 데이터인지
			/*
				int fseek(FILE* stream, long offset, int origin)
				: 파일에서 읽거나 쓸 위치(포인터)를 원하는 곳으로
				  이동시키는 함수
				- origin : 어디를 기준으로 움직일지 
				         - SEEK_SET : 파일 시작 기준
						 - SEEL_CUR : 현재 위치 기준
						 - SEEL_END : 파일 끝 기준
			*/
			fseek(fp, pTmp->offset * sizeof(USERDATA), SEEK_SET);

			// 파일에서 구조체 하나 읽기
			USERDATA user = { 0 };
			if (fread(&user, sizeof(USERDATA),1, fp) > 0) {

				// 파일에서 읽은 데이터를 노드안에 캐싱
				pTmp->pDataCache = malloc(sizeof(USERDATA));
				memcpy(pTmp->pDataCache, &user, sizeof(USERDATA));
				pTmp->sizeofData = sizeof(USERDATA);
				fclose(fp);
				return pTmp;
			}
			fclose(fp);
		}
		pTmp = pTmp->pNext;
	}
	return NULL;
}


// 파일에 저장된 데이터를 읽어서 "리스트 구조만 먼저 만들어 두는"함수 
// 실제 데이터는 안가져오고 key와 offset만 저장하는 초기 로딩 단계
// 파일 -> 연결리스트로 변환
int LoadListFromFile(void) {
	
	// ReleaseList() : 기존에 있던 노드 전부 삭제
	// 이유 : 새로 파일에서 다시 읽어오기 때문에 초기화 필요
	ReleaseList();


	FILE* fp = NULL;
	fopen_s(&fp, "listData.dat", "rb");

	//파일 읽기에 실패
	if (fp == NULL) {
		return 0;
	}

	// 읽기용 변수 준비
	USERDATA user = { 0 };
	unsigned int offset = 0;
	
	// File 끝까지 반복
	while (fread(&user, sizeof(USERDATA), 1, fp) > 0) {

		// user.name : key 로 사용
		// pData = NULL : 데이터 안넣음
		// sizeData = 0 : 데이터 없음
		// offset : 파일 위치 저장
		AddNewNode(user.name, NULL, 0, false, offset);

		// 다음을 읽기 위해 구조체 초기화
		memset(&user, 0, sizeof(USERDATA));
		++offset;
	}
	fclose(fp);
	return 1;
}

// 새로 추가된 데이터(bNew = true)만 파일에 이어서 저장하는 함수
// 연결리스트를 순회하면서 새 데이터만 파일 끝에 저장
int SaveNewDataToFile(void) {
	FILE* fp = NULL;

	// ab+의 의미
	// a : append(파일 끝에 추가)
	// b : binary 모드 
	// + : 읽기/쓰기 가능
	// >> 퍄일 끝에만 데이터가 추가됨
	fopen_s(&fp, "listData.dat", "ab+");
	if (fp == NULL) { // 파일 읽기 실패
		return 0;
	}

	MYNODE* pTmp = g_HeadNode.pNext;
	while (pTmp != NULL && pTmp != &g_TailNode) {

		if (pTmp->bNew == true) {
			fwrite(pTmp->pDataCache, pTmp->sizeofData, 1, fp);
		}

		pTmp = pTmp->pNext;

	}

	fclose(fp);
	return 1;
}

int SaveNodeToFile(MYNODE* pNode) {

	// 저장할 데이터가 없으면 실패
	if (pNode->pDataCache == NULL || pNode->sizeofData == 0) {
		return 0;
	}

	FILE* fp = NULL;
	/*
		rb+의 의미
		- r : 기존 파일 읽기
		- b : 바이너리 
		- + : 읽기/쓰기 가능
	*/
	fopen_s(&fp, "listData.dat", "rb+");
	if (fp == NULL) {
		return 0;
	}
	/*
		int fseek(FILE* stream, long offset, int origin)
		: 파일에서 읽거나 쓸 위치(포인터)를 원하는 곳으로
		  이동시키는 함수
			- origin : 어디를 기준으로 움직일지
			- SEEK_SET : 파일 시작 기준
			- SEEL_CUR : 현재 위치 기준
			- SEEL_END : 파일 끝 기준
	*/
	fseek(fp, pNode->offset * sizeof(USERDATA), SEEK_SET);

	// 해당 위치에 데이터 저장 : 기존 데이터 → 새 데이터로 교체
	// append 아님, overwrite임
	if (fwrite(pNode->pDataCache, sizeof(USERDATA), 1, fp) > 0) {
		fclose(fp);
		return 1;
	}

	return 0;
}