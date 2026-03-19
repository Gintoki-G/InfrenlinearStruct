#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "singleList.h"
#include "ui.h"

int main(void)
{
	InitList();
	AddNewNode(15, "Kim", "010-1234-1111");
	AddNewNode(5, "Hoon", "010-1234-2222");
	AddNewNode(10, "Jang", "010-1234-4444");
	AddNewNode(7, "Choi", "010-1234-3333");

	EventLoopRun();

	ReleaseList();
	return 0;
}
