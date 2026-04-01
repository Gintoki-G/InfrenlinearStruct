#pragma once
#include <stdio.h>
#include <stdbool.h>

typedef struct MYNODE {

	bool bNew;
	char* pszKey;
	void* pDataCache;
	unsigned int sizeofData;
	unsigned int offset;
	struct MYNODE* pPrev;
	struct MYNODE* pNext;
}MYNODE;

typedef struct USERDATA {
	int age;
	char* Name;
	char* phone;
}USERDATA;

extern MYNODE g_HeadNode;
extern MYNODE g_TailNode;

void InitList(void);
int IsEmpty(void);
void ReleaseList(void);
MYNODE* AddNewNode(bool bNew, char* pszKey, void* pData, const int sizeData, const int offset);
int SaveNewDataToFile();
MYNODE* SearchList(char* Key);
USERDATA getData(MYNODE* pNode);
USERDATA* LoadDataFromFile(FILE* fp, int offset);
USERDATA* CreateCache(USERDATA* src);