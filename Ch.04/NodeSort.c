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
	struct USERDATA* pPrev;
} USERDATA;

USERDATA g_HeadNode = { 0, "_DummyHead_" };
USERDATA g_TailNode = { 0, "_DummyTail_" };


typedef enum MY_MENU { EXIT, NEW, SEARCH, PRINT, REMOVE } MY_MENU;

MY_MENU PrintMenu(void)
{
	MY_MENU input = 0;

	system("cls");
	printf("[1]New\t[2]Search\t[3]Print\t[4]Remove\t[0]Exit\n");
	scanf_s("%d%*c", &input);
	return input;
}

int IsEmpty(void)
{
	if (g_HeadNode.pNext == &g_TailNode)
		return 1;

	return 0;
}

void NodeDataCopy(USERDATA* pLeft, USERDATA* pRight) {
	pLeft->age = pRight->age;
	strcpy_s(pLeft->name, sizeof(pLeft->name), pRight->name);
	strcpy_s(pLeft->phone, sizeof(pLeft->phone), pRight->phone);
}

void SwapNode(USERDATA* pLeft, USERDATA* pRight) {
	USERDATA tmp = *pLeft;
	NodeDataCopy(pLeft, pRight);
	NodeDataCopy(pRight, &tmp);
}

void SortListByName(void) {
	if (IsEmpty()) {
		puts("error : empty");
		return;
	}

	//pTmp: 현재 정렬할 위치
	USERDATA* pTmp = g_HeadNode.pNext;
	USERDATA* pSelected = NULL;
	USERDATA* pCmp = NULL;

	/*
			  정렬 위치
			      |	
		[D.H] -> [1] -> [2] -> [3] -> [D.T]
	*/
	while (pTmp != NULL && pTmp != g_TailNode.pPrev) {// 정렬 위치 이동
		pSelected = pTmp; // 처음에는 현재 노드를 최소값이라고 가정
		pCmp = pTmp->pNext;// 현재 노드의 다음 노드부터 비교


		/*
			  정렬 위치
				  |
		[D.H] -> [1] -> [2] -> [3] -> [D.T]
		[1], [2] 비교 pCmp = pCmp->pNext
		[1], [3] 비교 pCmp = pCmp->pNext
		[1], [4] 비교 pCmp = pCmp->pNext

		*/
		while (pCmp != NULL && pCmp != &g_TailNode) { // 최솟값 찾기

			/*
				strcmp의 반환값 > 0 : 앞 문자열이 큼
				strcmp의 반환값 = 0 : 같음
				strcmp의 반환값 < 0 : 뒤 문자열이 큼
			*/
			if (strcmp(pSelected->name, pCmp->name) > 0) { // 더 작은 이름 발견 
				pSelected = pCmp; // 최소값 갱신
			}
			pCmp = pCmp->pNext; // 현재 노드의 다음 노드 비교
		}

		// 현재 위치와 최소값이 다르면 노드 위치 교환
		if (pTmp != pSelected) {
			SwapNode(pTmp, pSelected);
		}
		pSelected = NULL;
		pTmp = pTmp->pNext;
	}
}


void PrintList(int wait)
{
	USERDATA* pTmp = &g_HeadNode;
	while (pTmp != NULL)
	{
		printf("[%p] %d, %s, %s [%p]\n",
			pTmp,
			pTmp->age, pTmp->name, pTmp->phone,
			pTmp->pNext);
		pTmp = pTmp->pNext;
	}
	putchar('\n');

	if (wait)
		_getch();
}

void AddNewNode(int age, char* pszName, char* pszPhone)
{
	USERDATA* pNewNode = calloc(1, sizeof(USERDATA));
	pNewNode->age = age;
	strcpy_s(pNewNode->name, sizeof(pNewNode->name), pszName);
	strcpy_s(pNewNode->phone, sizeof(pNewNode->phone), pszPhone);

	USERDATA* pPrevNode = g_TailNode.pPrev;
	pPrevNode->pNext = pNewNode;
	pNewNode->pPrev = pPrevNode;
	pNewNode->pNext = &g_TailNode;
	g_TailNode.pPrev = pNewNode;
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
	USERDATA* pTmp = g_HeadNode.pNext;
	while (pTmp != &g_TailNode)
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
	USERDATA* pCur = g_HeadNode.pNext;
	USERDATA* pNextNode;
	USERDATA* pPrevNode;
	while (pCur != NULL && pCur != &g_TailNode)
	{
		if (strcmp(pCur->name, pszName) == 0)
		{
			pNextNode = pCur->pNext;
			pPrevNode = pCur->pPrev;

			pNextNode->pPrev = pCur->pPrev;
			pPrevNode->pNext = pCur->pNext;

			free(pCur);
			return 1;
		}

		pCur = pCur->pNext;
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
	while (pTmp != &g_TailNode)
	{
		pDelete = pTmp;
		pTmp = pTmp->pNext;

		printf("Delete: %d, %s, %s\n",
			pDelete->age, pDelete->name, pDelete->phone);

		free(pDelete);
	}

	g_HeadNode.pNext = &g_TailNode;
	g_TailNode.pPrev = &g_HeadNode;
}


void InitDummyData(void)
{
	AddNewNode(5, "Kim", "010-1234-1111");
	AddNewNode(6, "Hoon", "010-1234-2222");
	AddNewNode(7, "Choi", "010-1234-3333");
	AddNewNode(8, "Jang", "010-1234-4444");
}


void RemoveForTest(char* pszName)
{
	USERDATA* pPrev = NULL;
	RemoveByName(pszName);
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

int main(void)
{
	g_HeadNode.pNext = &g_TailNode;
	g_TailNode.pPrev = &g_HeadNode;

	TestSortList01();
	_getch();
	TestSortList02();
	_getch();
	TestSortList03();
	_getch();
	TestSortList04();
	_getch();


	return 0;
}
