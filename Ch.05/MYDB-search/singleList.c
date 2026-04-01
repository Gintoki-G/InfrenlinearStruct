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

MYNODE* SearchList(char* Key) {
	char pszkey[20];
	strcpy_s(pszkey, sizeof(pszkey), Key);
	
	MYNODE* pTmp = g_HeadNode.pNext;
	while (pTmp != &g_TailNode) {
		if (strcmp(pszkey, pTmp->pszKey) == 0) {
			return pTmp;
		}
		pTmp = pTmp->pNext;
	}
	return NULL;
}

USERDATA getData(MYNODE* pNode) {
	USERDATA user = { 0 };
	if (pNode == NULL) return user;

	// 🔹 이미 캐시 있음
	if (pNode->pDataCache != NULL) {
		USERDATA* pData = (USERDATA*)pNode->pDataCache;

		user.age = pData->age;

		user.Name = (char*)malloc(strlen(pData->Name) + 1);
		strcpy_s(user.Name, strlen(pData->Name) + 1, pData->Name);

		user.phone = (char*)malloc(strlen(pData->phone) + 1);
		strcpy_s(user.phone, strlen(pData->phone) + 1, pData->phone);

		return user;
	}

	// 🔹 파일에서 로딩
	FILE* fp = NULL;
	fopen_s(&fp, "listData.dat", "rb");
	if (fp == NULL) return user;

	USERDATA* loaded = LoadDataFromFile(fp, pNode->offset);
	fclose(fp);

	if (loaded == NULL) return user;

	// 🔥 캐싱 : 같은 데이터인데 매번 파일에 접근하는 것을 막기 위해서
	pNode->pDataCache = CreateCache(loaded);

	// 🔹 반환용 복사
	user.age = loaded->age;
	user.Name = loaded->Name;
	user.phone = loaded->phone;

	free(loaded->Name);
	free(loaded->phone);
	free(loaded); // 구조체만 free (Name, phone은 user가 들고 있음)

	return user;
}
// 파일 구조 [age] [nameLen] [name] [phoneLen] [phone]
// offset은 age의 시작 위치
// 가변 데이터 읽는 방법
// 길이 먼저 읽고 → 그 길이만큼 malloc → fread
USERDATA* LoadDataFromFile(FILE* fp, int offset) {
	if (fp == NULL) return NULL;

	fseek(fp, offset, SEEK_SET);

	int age = 0;
	int nameLen = 0;
	int phoneLen = 0;

	fread(&age, sizeof(int), 1, fp);

	fread(&nameLen, sizeof(int), 1, fp);
	char* name = (char*)malloc(nameLen);
	fread(name, nameLen, 1, fp);

	fread(&phoneLen, sizeof(int), 1, fp);
	char* phone = (char*)malloc(phoneLen);
	fread(phone, phoneLen, 1, fp);

	USERDATA* data = (USERDATA*)malloc(sizeof(USERDATA));
	data->age = age;
	data->Name = name;
	data->phone = phone;

	return data;
}

USERDATA* CreateCache(USERDATA* src) {
	if (src == NULL) return NULL;

	USERDATA* dst = (USERDATA*)malloc(sizeof(USERDATA));
	dst->age = src->age;

	dst->Name = (char*)malloc(strlen(src->Name) + 1);
	strcpy_s(dst->Name, strlen(src->Name) + 1, src->Name);

	dst->phone = (char*)malloc(strlen(src->phone) + 1);
	strcpy_s(dst->phone, strlen(src->phone) + 1, src->phone);

	return dst;
}