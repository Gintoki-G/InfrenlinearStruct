#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "singleList.h"
#include "ui.h"
#include "test.h"

int main(void)
{
	InitList();
	InitDummyData();
	//SortListByName();

	EventLoopRun();

	ReleaseList();
	return 0;
}
