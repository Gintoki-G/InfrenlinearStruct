#include "ui.h"

void AddNewData() {

	int age;
	char name[50];
	char phone[20];
	char answer;
	char save_answer;
	char* pszKey = NULL;
	int offset = -1;
	/*
	scanf_s("형식지정자", 변수주소, 입력받을 데이터의 크기);

	int a;
	scanf_s("%d", &a);

	char c;
	scanf_s(" %c", &c, 1);

	char str[20];
	scanf_s("%s", str, 20);
	*/
	printf("Input age : ");
	scanf_s("%d", &age);

	printf("Input Name : ");
	scanf_s("%s", name, (unsigned)_countof(name));

	printf("Input phone : ");
	scanf_s("%s", phone, (unsigned)_countof(phone));

	printf("choose key Name?");
	scanf_s(" %c", &answer, 1);

	/*
		strcpy_s(목적지, 목적지_크기, 원본);


	*/
	if (answer == 'y' || answer == 'Y') {
		pszKey = name;
	}
	else {
		pszKey = phone;
	}

	//AddNewNode(bool bNew, char* pszKey, 
	// void* pData, const int sizeData, const int offset)
	USERDATA user = { 0 };
	user.Name = name;
	user.phone = phone;
	user.age = age;
	AddNewNode(true, pszKey, &user, sizeof(USERDATA), offset);

	printf("Do you want to save to File?");
	scanf_s(" %c", &save_answer, 1);

	if (save_answer == 'y' || save_answer == 'Y') {
		SaveNewDataToFile();
	}
	else {
		return;
	}


}

void PrintAllList() {
	MYNODE* pTmp = g_HeadNode.pNext;
	if (IsEmpty() == 1) {
		printf("there is no list");
	}
	else {
		if (pTmp->pDataCache == NULL) {
			printf("NoData");
		}
		else {
			while (pTmp != &g_TailNode) {
				USERDATA* pData = (USERDATA*)pTmp->pDataCache;

				printf("[%p] age: %d, name: %s, phone: %s [%p]\n",
					pTmp,
					pData->age,
					pData->Name,
					pData->phone,
					pData);

				pTmp = pTmp->pNext;
			}
		}
	}
}