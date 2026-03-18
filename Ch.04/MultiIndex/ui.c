#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "singleList.h"
#include "ui.h"


MY_MENU PrintMenu(void)
{
	MY_MENU input = 0;

	system("cls");
	printf("[1]New\t[2]Search\t[3]Search age\t[4]Print\t[5]Remove\t[0]Exit\n");
	scanf_s("%d%*c", &input);
	return input;
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

void PrintListByIndex(int wait)
{
	if (IsEmpty())
		return;

	unsigned int cnt = GetListCount();

	for (int i = 0; i < cnt; ++i)
		printf("%d, %s, %s\n",
			g_idxListAge[i]->age,
			g_idxListAge[i]->name, g_idxListAge[i]->phone);
	putchar('\n');

	for (int i = 0; i < cnt; ++i)
		printf("%d, %s, %s\n",
			g_idxListName[i]->age, g_idxListName[i]->name,
			g_idxListName[i]->phone);

	putchar('\n');

	if (wait)
		_getch();
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

void SearchByAge(void)
{
	int min = 0, max = 1, cnt = 0;
	printf("MIN MAX age: ");
	scanf_s("%d%d%*c", &min, &max);

	SortListByAge();

	void** pResult = SearchByAgeRange(min, max, &cnt);
	USERDATA* pTmp = NULL;
	for (int i = 0; i < cnt; ++i)
	{
		pTmp = (USERDATA*)pResult[i];
		printf("%d, %s, %s\n", pTmp->age, pTmp->name, pTmp->phone);
	}

	free(pResult);
	_getch();
}

void SearchByAgeIndex(void)
{
	int min = 0, max = 1;
	unsigned int cnt = 0;
	printf("[Index search] MIN MAX age: ");
	scanf_s("%d%d%*c", &min, &max);

	USERDATA** pResult = (USERDATA**)SearchByIndexAgeRange(min, max, &cnt);
	USERDATA* pTmp = NULL;
	for (unsigned int i = 0; i < cnt; ++i)
	{
		pTmp = (USERDATA*)pResult[i];
		printf("%d, %s, %s\n", pTmp->age, pTmp->name, pTmp->phone);
	}

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

		case SEARCH_RANGE:
			//SearchByAge();
			SearchByAgeIndex();
			break;

		case PRINT:
			//PrintList(1);
			PrintListByIndex(1);
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
