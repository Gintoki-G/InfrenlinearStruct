#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "singleList.h"
#include "ui.h"

int main(void)
{
	InitList();
	LoadListFromFile();

	EventLoopRun();

	SaveNewDataToFile();
	ReleaseList();
	return 0;
}
