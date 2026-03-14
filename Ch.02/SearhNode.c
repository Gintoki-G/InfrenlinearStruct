#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>


/*
 void* memcpy(void* dest, const void* src, size_t size);
 : 메모리에 있는 데이터를 그대로 목하는 함수
 - dest : 데이터가 복사될 목적지
 - src : 복사할 원본 데이터
 - size : 얼마만큼 복사할 것인지
*/

/*
SearchByName ,SearchByPhone : 사용자 UI
SearchListByName , SearchListByPhone : 내부 코드
*/
typedef struct USERDATA
{
	int age;
	char name[32];
	char phone[32];
	struct USERDATA* pNext;
} USERDATA;

USERDATA* g_pHeadNode = NULL;


typedef enum MY_MENU { EXIT, NEW, SEARCH, PRINT, REMOVE } MY_MENU;

MY_MENU PrintMenu(void)
{
	MY_MENU input = 0;

	system("cls");
	printf("[1]New\t[2]Search\t[3]Print\t[4]Remove\t[0]Exit\n");
	scanf_s("%d%*c", &input);
	return input;
}

void PrintList(void)
{
	USERDATA* pTmp = g_pHeadNode;
	while (pTmp != NULL)
	{
		printf("[%p] %d, %s, %s [%p]\n",
			pTmp,
			pTmp->age, pTmp->name, pTmp->phone,
			pTmp->pNext);
		pTmp = pTmp->pNext;
	}

	_getch();
}

void AddNewNode(int age, char* pszName, char* pszPhone)
{
	USERDATA* pNewNode = calloc(1, sizeof(USERDATA));
	pNewNode->age = age;
	strcpy_s(pNewNode->name, sizeof(pNewNode->name), pszName);
	strcpy_s(pNewNode->phone, sizeof(pNewNode->phone), pszPhone);

	//if (g_pHeadNode != NULL)
	//	pNewNode->pNext = g_pHeadNode;

	//g_pHeadNode = pNewNode;

	if (g_pHeadNode == NULL)
		g_pHeadNode = pNewNode;
	else
	{
		USERDATA* pTmp = g_pHeadNode;
		while (pTmp->pNext != NULL)
			pTmp = pTmp->pNext;

		pTmp->pNext = pNewNode;
	}
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

int SearchListByName(USERDATA* pUser, char *pszName)
{
	// Head Node부터 데이터를 찾음
	// 문자열 비교 함수 : strcmp
	// return 잘 쓰기
	 
	USERDATA* pTmp = g_pHeadNode;
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
	// Head Node부터 데이터를 찾음
	// 문자열 비교 함수 : strcmp
	// return 잘 쓰기

	USERDATA* pTmp = g_pHeadNode;
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
			//SearchByName();
			SearchByPhone();
			break;

		case PRINT:
			PrintList();
			break;

		case REMOVE:
			break;

		default:
			break;
		}
	}
	puts("Bye~!");
}


void ReleaseList(void)
{
	USERDATA* pTmp = g_pHeadNode;
	USERDATA* pDelete;
	while (pTmp != NULL)
	{
		pDelete = pTmp;
		pTmp = pTmp->pNext;

		printf("Delete: %d, %s, %s\n",
			pDelete->age, pDelete->name, pDelete->phone);

		free(pDelete);
	}
}


void InitDummyData(void)
{
	AddNewNode(5, "Hong", "010-1234-1111");
	AddNewNode(6, "Hoon", "010-1234-2222");
	AddNewNode(7, "Choi", "010-1234-3333");
	AddNewNode(8, "Jang", "010-1234-4444");
}


int main(void)
{
	InitDummyData();
	EventLoopRun();
	ReleaseList();
	return 0;
}
