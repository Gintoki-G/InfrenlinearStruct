#include <stdio.h>
#include "ui.h"
#include "singleList.h"

int main() {
	InitList();
	AddNewData();
	PrintAllList();
	AddNewData();
	PrintAllList();
	findData();
	return 0;
}