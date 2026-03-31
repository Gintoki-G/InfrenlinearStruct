#include <stdio.h>
#include "singleList.h"
#include <string.h>
#include <stdlib.h>

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
	MYNODE* pDelete = NULL;

	while (pTmp != &g_TailNode)
	{
		pDelete = pTmp;
		pTmp = pTmp->pNext;

		// 1. 노드의 키(pszKey) 메모리 해제
		if (pDelete->pszKey != NULL) {
			free(pDelete->pszKey);
		}

		// 2. pDataCache(USERDATA) 내부의 동적 할당 메모리 해제
		if (pDelete->pDataCache != NULL) {
			USERDATA* pData = (USERDATA*)pDelete->pDataCache;

			// 🔥 추가: USERDATA 내부에 malloc된 Name과 phone을 먼저 free
			if (pData->Name != NULL) free(pData->Name);
			if (pData->phone != NULL) free(pData->phone);

			// USERDATA 구조체 자체를 free
			free(pData);
		}

		// 3. 노드 자체를 해제
		free(pDelete);
	}

	// 리스트 연결 상태 초기화
	g_HeadNode.pNext = &g_TailNode;
	g_TailNode.pPrev = &g_HeadNode;
	g_listCount = 0;
}

MYNODE* AddNewNode(bool bNew, char* pszKey, void* pData, const int sizeData, const int offset) {
	MYNODE* pNewNode = calloc(1, sizeof(MYNODE));
	int lenOfKey = (int)strlen(pszKey);
	pNewNode->pszKey = malloc(lenOfKey + 1);
	strcpy_s(pNewNode->pszKey, lenOfKey + 1, pszKey);
	pNewNode->offset = offset;

	if (bNew == true) {
		if (pData != NULL && sizeData > 0) {

			USERDATA* src = (USERDATA*)pData;
			USERDATA* dst = malloc(sizeof(USERDATA));

			dst->age = src->age;

			// 🔥 Name deep copy
			dst->Name = malloc(strlen(src->Name) + 1);
			strcpy_s(dst->Name, strlen(src->Name) + 1, src->Name);

			// 🔥 phone deep copy
			dst->phone = malloc(strlen(src->phone) + 1);
			strcpy_s(dst->phone, strlen(src->phone) + 1, src->phone);

			pNewNode->pDataCache = dst;
		}
	}

	pNewNode->bNew = bNew;
	pNewNode->sizeofData = sizeData;
	MYNODE* pPrevNode = g_TailNode.pPrev;
	pPrevNode->pNext = pNewNode;
	pNewNode->pPrev = pPrevNode;
	pNewNode->pNext = &g_TailNode;
	g_TailNode.pPrev = pNewNode;

	g_listCount++;

	return pNewNode;
}

int SaveNewDataToFile() {
	MYNODE* pTmp = g_HeadNode.pNext;
	FILE* fp = NULL;

	fopen_s(&fp, "listData.dat", "ab+");
	if (fp == NULL) return 0;

	int savedCount = 0;

	while (pTmp != &g_TailNode) {
		if (pTmp->bNew == true && pTmp->pDataCache != NULL) {

			USERDATA* pData = (USERDATA*)pTmp->pDataCache;

			int nameLen = (int)strlen(pData->Name) + 1;
			int phoneLen = (int)strlen(pData->phone) + 1;

			// offset 저장용
			fseek(fp, 0, SEEK_END);
			long pos = ftell(fp);

			// 1. age
			fwrite(&pData->age, sizeof(int), 1, fp);

			// 2. name
			fwrite(&nameLen, sizeof(int), 1, fp);
			fwrite(pData->Name, nameLen, 1, fp);

			// 3. phone
			fwrite(&phoneLen, sizeof(int), 1, fp);
			fwrite(pData->phone, phoneLen, 1, fp);

			// 저장 완료 처리
			pTmp->bNew = false;
			pTmp->offset = pos;   // 이제 구조체 단위 아님 (byte 기준)

			savedCount++;
		}

		pTmp = pTmp->pNext;
	}

	fclose(fp);
	return savedCount;
}