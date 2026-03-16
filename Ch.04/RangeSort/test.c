#include <stdio.h>
#include "singleList.h"
#include "ui.h"

void InitDummyData(void)
{
	AddNewNode(7, "Park", "010-1234-3333");
	AddNewNode(5, "Kim", "010-1234-1111");
	AddNewNode(8, "Chang", "010-1234-7777");
	AddNewNode(6, "Hoon", "010-1234-2222");
	AddNewNode(8, "Jang", "010-1234-6666");
	AddNewNode(8, "Jung", "010-1234-8888");
	AddNewNode(7, "Choi", "010-1234-4444");
	AddNewNode(7, "Chae", "010-1234-5555");
}

void RemoveForTest(char* pszName)
{
	USERDATA* pPrev = NULL;
	RemoveByName(pszName);
}

void TestStep01(void)
{
	puts("---------------------------------");
	AddNewNode(5, "Hong", "010-1234-1111");
	PrintList(0);
	RemoveForTest("Hong");
	PrintList(0);
	puts("TestStep01 - Complete");
}

void TestStep02(void)
{
	puts("---------------------------------");
	InitDummyData();
	RemoveForTest("Hong");
	RemoveForTest("Jang");
	PrintList(0);
	puts("TestStep02 - Complete");
	ReleaseList();
}


void TestStep03(void)
{
	puts("---------------------------------");
	InitDummyData();
	PrintList(0);
	putchar('\n');
	RemoveForTest("Choi");
	RemoveForTest("Hoon");
	PrintList(0);
	puts("TestStep03 - Complete");
	ReleaseList();
}

void TestSortList01(void)
{
	puts("TestSortList01");
	AddNewNode(5, "Hoon", "010-1234-1111");
	PrintList(0);
	SortListByName();
	PrintList(0);
	ReleaseList();
	puts("---------------------------------");
}

void TestSortList02(void)
{
	puts("TestSortList02");
	AddNewNode(5, "Hoon", "010-1234-2222");
	AddNewNode(6, "Hong", "010-1234-1111");
	PrintList(0);
	SortListByName();
	PrintList(0);
	ReleaseList();
	puts("---------------------------------");
}

void TestSortList03(void)
{
	puts("TestSortList03");
	AddNewNode(6, "Hong01", "010-1234-1111");
	AddNewNode(5, "Hong", "010-1234-1111");
	PrintList(0);
	SortListByName();
	PrintList(0);
	ReleaseList();
	puts("---------------------------------");
}

void TestSortList04(void)
{
	puts("TestSortList04");
	InitDummyData();
	PrintList(0);
	SortListByName();
	PrintList(0);
	ReleaseList();
	puts("---------------------------------");
}
