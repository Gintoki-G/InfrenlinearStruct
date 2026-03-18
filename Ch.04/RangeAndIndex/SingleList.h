#pragma once

typedef struct USERDATA
{
	int age;
	char name[32];
	char phone[32];
	struct USERDATA* pNext;
	struct USERDATA* pPrev;
} USERDATA;

extern USERDATA g_HeadNode;
extern USERDATA g_TailNode;

void InitList(void);
void ReleaseList(void);
int IsEmpty(void);
int RemoveByName(char* pszName);
int SearchListByPhone(USERDATA* pUser, char* pszPhone);
int SearchListByName(USERDATA* pUser, char* pszName);
void SortListByName(void);
void SortListByAge(void);
void AddNewNode(int age, char* pszName, char* pszPhone);
void** SearchByAgeRange(int min, int max, int* pCount);
unsigned int GetListCount(void);
unsigned int RecalcListCount(void);
void** MakeIndexAge(int* pCnt);
void** SearchByIndexAgeRange(int min, int max, unsigned int* pCount);