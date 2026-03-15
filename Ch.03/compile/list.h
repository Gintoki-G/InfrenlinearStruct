#ifndef _H__LIST__
#define _H__LIST__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

typedef struct USERDATA
{
	int age;
	char name[32];
	char phone[32];
	struct USERDATA* pPrev;
	struct USERDATA* pNext;
} USERDATA;

USERDATA g_HeadNode;
USERDATA g_TailNode;


USERDATA* AddNewNodeAtTail(int age, const char* pszName, const char* pszPhone);
USERDATA* AddNewNodeAtHead(int age, const char* pszName, const char* pszPhone);
void InitList(void);
void ReleaseList(void);
USERDATA* SearchByName(const char* pszName);

void RemoveNode(USERDATA* pRemove);
int RemoveByName(const char* pszName);
void PrintList(void);
void PrintListReverse(void);
int IsEmpty(void);
#endif
