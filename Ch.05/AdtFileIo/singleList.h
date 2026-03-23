#pragma once
#include <stdbool.h>
typedef struct USERDATA {
	int age;
	char name[32];
	char phone[32];
}USERDATA;

typedef struct MYNODE {
	bool bNew;
	char* pszKey;
	/*
		void* pDataCache;
		실제 데이터를 가리키는 포인터 (타입 미정)
		void*라서 어떤 데이터든 저장 가능
		- 구조체
		- 배열
		- 파일 데이터 등
		>> 대신 사용할 때는 형변환 필요
			ex) (int*)pDataCache
	*/
	void* pDataCache;

	//sizeofData : pDataCache가 가리키는 데이터의 크기
	unsigned int sizeofData;
	
	//데이터의 위치정보
	// 파일에서 몇번째 위치인지
	unsigned int offset;

	struct MYNODE* pNext;
	struct MYNODE* pPrev;
}MYNODE;

extern MYNODE g_HeadNode;
extern MYNODE g_TailNode;

void InitList(void);
void ReleaseList(void);
void AddNewNode(const char* pszKey, const void* pData, int sizeData,
	bool bNew, unsigned int offset);
void* SearchListByName(char* pszKey);
void SortListByAge(void);
int LoadListFromFile(void);
int SaveNewDataToFile(void);
int SaveNodeToFile(MYNODE* pNode);