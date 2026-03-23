#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "singleList.h"
#include "ui.h"


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
	MYNODE* pTmp = &g_HeadNode;
	while (pTmp != NULL)// && pTmp != &g_TailNode)
	{
		printf("[%p] %s, new: %d, offset: %d, ",
			pTmp,
			(const char*)pTmp->pszKey,
			pTmp->bNew,
			pTmp->offset);

		if (pTmp->pDataCache != NULL)
		{
			USERDATA* pUser = pTmp->pDataCache;
			printf("%d, %s [%p]\n",
				pUser->age, pUser->phone,
				pTmp->pNext);
		}
		else
			printf("[%p]\n", pTmp->pNext);

		pTmp = pTmp->pNext;
	}
	putchar('\n');

	if (wait)
		_getch();
}

void AddNewUser(void)
{
	USERDATA user = { 0 };

	printf("age: ");
	scanf_s("%d%*c", &user.age);
	printf("name: ");
	gets_s(user.name, sizeof(user.name));
	printf("phone: ");
	gets_s(user.phone, sizeof(user.phone));

	AddNewNode(user.name, &user, sizeof(user), true, 0);
}

void EditUserData(MYNODE* pNode)
{
	USERDATA* pUser = pNode->pDataCache;
	printf("New phone number: ");
	char szBuffer[32] = { 0 };
	gets_s(szBuffer, sizeof(szBuffer));
	if (strlen(szBuffer) > 0)
	{
		memcpy(pUser->phone, szBuffer, sizeof(pUser->phone));

		printf("Do you want to commit now?(y/n)\n");
		char ch = _getche();
		if (ch == 'y' || ch == 'Y')
			SaveNodeToFile(pNode);
	}
}

void SearchByName(void)
{
	char name[32] = { 0 };

	printf("name: ");
	gets_s(name, sizeof(name));

	MYNODE* pNode = SearchListByName(name);
	if (pNode != NULL)
	{
		USERDATA* pUser = pNode->pDataCache;
		if (pUser != NULL)
		{
			printf("Found: %d, %s, %s\n",
				pUser->age, pUser->name, pUser->phone);

			printf("Do you want to edit? (y/n)\n");
			char ch = _getch();
			if (ch == 'y' || ch == 'Y')
				EditUserData(pNode);
		}
		else
			puts("Cache data not found.");
	}
	else
		puts("Not found.");

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
			break;

		case PRINT:
			PrintList(1);
			break;

		case REMOVE:
			break;

		default:
			break;
		}
	}
	puts("Bye~!");
}
