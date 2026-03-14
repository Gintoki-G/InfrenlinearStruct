#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

typedef struct USERDATA
{
	int age;
	char name[32];
	char phone[32];
	struct USERDATA* pNext;
} USERDATA;

//USERDATA* g_pHeadNode = NULL;
USERDATA g_HeadNode = { 0, "_DummyHead_" };


typedef enum MY_MENU { EXIT, NEW, SEARCH, PRINT, REMOVE } MY_MENU;

MY_MENU PrintMenu(void)
{
	MY_MENU input = 0;

	system("cls");
	printf("[1]New\t[2]Search\t[3]Print\t[4]Remove\t[0]Exit\n");
	scanf_s("%d%*c", &input);
	return input;
}

void PrintList(int wait)
{
	USERDATA* pTmp = g_HeadNode.pNext;
	while (pTmp != NULL)
	{
		printf("[%p] %d, %s, %s [%p]\n",
			pTmp,
			pTmp->age, pTmp->name, pTmp->phone,
			pTmp->pNext);
		pTmp = pTmp->pNext;
	}

	if (wait)
		_getch();
}

void AddNewNode(int age, char* pszName, char* pszPhone)
{
	USERDATA* pNewNode = calloc(1, sizeof(USERDATA));
	pNewNode->age = age;
	strcpy_s(pNewNode->name, sizeof(pNewNode->name), pszName);
	strcpy_s(pNewNode->phone, sizeof(pNewNode->phone), pszPhone);

	/*
	더미 노드가 없을 때는 HeadNode가 NULL인지 아닌지 
	검사하는 코드가 있었다
		if (g_pHeadNode == NULL)
		g_pHeadNode = pNewNode;
	*/
	USERDATA* pTmp = &g_HeadNode;
	while (pTmp->pNext != NULL)
		pTmp = pTmp->pNext;

	pTmp->pNext = pNewNode;

}

void AddNewUser(void)
{
	int age = 0;
	char name[32] = { 0 };
	char phone[32] = { 0 };

	printf("age: ");
	scanf_s("%d%*c", &age);
	printf("name: ");
	gets_s(name, sizeof(name));
	printf("phone: ");
	gets_s(phone, sizeof(phone));

	AddNewNode(age, name, phone);
}

int SearchListByName(USERDATA* pUser, char* pszName)
{

	// HeadNode가 없을 때는 pTmp = g_pHeadNode;
	USERDATA* pTmp = g_HeadNode.pNext;
	while (pTmp != NULL)
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
	while (pTmp != NULL)
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

void SearchByName(void)
{
	char name[32] = { 0 };

	printf("name: ");
	gets_s(name, sizeof(name));

	USERDATA user = { 0 };
	if (SearchListByName(&user, name) > 0)
		printf("Found: %d, %s, %s\n",
			user.age, user.name, user.phone);
	else
		puts("Not found");

	_getch();
}

void SearchByPhone(void)
{
	char phone[32] = { 0 };

	printf("phone: ");
	gets_s(phone, sizeof(phone));

	USERDATA user = { 0 };
	if (SearchListByPhone(&user, phone) > 0)
		printf("Found: %d, %s, %s\n",
			user.age, user.name, user.phone);
	else
		puts("Not found");

	_getch();
}


int RemoveByName(char* pszName)
{

	/*
	*			Delete
	*   pTmp    pNextNode  pNextNode->pNext
	*    |        |            |
	* [Dummy] -> [2] ->       [3] -> [4] -> NULL
	*
	*
	*/

	USERDATA* pTmp = &g_HeadNode;
	USERDATA* pNextNode;
	while (pTmp->pNext != NULL)
	{
		pNextNode = pTmp->pNext;
		if (strcmp(pNextNode->name, pszName) == 0)
		{
			pTmp->pNext = pNextNode->pNext;
			free(pNextNode);
			return 1;
		}

		pTmp = pTmp->pNext;
	}

	return 0;
}


void SearchByNameToRemove(void)
{
	char name[32] = { 0 };

	printf("name: ");
	gets_s(name, sizeof(name));

	USERDATA* pPrev = NULL;

	if (RemoveByName(name))
		puts("Complete");
	else
		puts("Not found");

	_getch();
}

void EventLoopRun(void)
{
	MY_MENU menu = 0;

	while ((menu = PrintMenu()) != 0)
	{
		switch (menu)
		{
		case NEW:
			AddNewUser();
			break;

		case SEARCH:
			SearchByName();
			//SearchByPhone();
			break;

		case PRINT:
			PrintList(1);
			break;

		case REMOVE:
			SearchByNameToRemove();
			break;

		default:
			break;
		}
	}
	puts("Bye~!");
}


void ReleaseList(void)
{
	USERDATA* pTmp = g_HeadNode.pNext;
	USERDATA* pDelete;
	while (pTmp != NULL)
	{
		pDelete = pTmp;
		pTmp = pTmp->pNext;

		printf("Delete: %d, %s, %s\n",
			pDelete->age, pDelete->name, pDelete->phone);

		free(pDelete);
	}

	g_HeadNode.pNext = NULL;
}


void InitDummyData(void)
{
	AddNewNode(5, "Hong", "010-1234-1111");
	AddNewNode(6, "Hoon", "010-1234-2222");
	AddNewNode(7, "Choi", "010-1234-3333");
	AddNewNode(8, "Jang", "010-1234-4444");
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

int main(void)
{
	TestStep01();
	TestStep02();
	TestStep03();
	return 0;
}
