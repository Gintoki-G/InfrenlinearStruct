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
	printf("[1]New\t[2]Search\t[3]Sort\t[4]Print\t[5]Remove\t[0]Exit\n");
	scanf_s("%d%*c", &input);
	return input;
}

void PrintList(int wait)
{
	MYNODE* pTmp = g_HeadNode.pNext;
	USERDATA* pUser = NULL;
	while (pTmp != NULL && pTmp != &g_TailNode)
	{
		pUser = pTmp->pData;
		printf("[%p] %d, %s, %s [%p]\n",
			pTmp,
			pUser->age,
			pTmp->GetKey(pTmp->pData),
			pUser->phone,
			pTmp->pNext);
		pTmp = pTmp->pNext;
	}
	putchar('\n');

	if (wait)
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
			break;

		case SEARCH:
			break;

		case SORTLIST:
			SortListByAge();
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
