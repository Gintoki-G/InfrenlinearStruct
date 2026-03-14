#include <stdio.h>
#include <string.h>

/*
fopen_s:  파일을 여는 함수
	   - 왜 주소를 주냐면
		 → fopen_s가 파일을 열고 그 결과를 fp에 저장해야 하기 때문
	   - errno_t fopen_s(FILE** pFile, const char* filename, const char* mode);
		- pFile : 파일의 주소 -ex) &fp
		- filename : 열 파일의 이름 -ex) "2022"
		- mode : 파일을 열때의 모드 -ex) "rb"
*/

/*
fread : 파일을 읽어서 저장하는 함수
	 - size_t fread(void *buffer, size_t size, size_t count, FILE *stream);
			- buffer : 읽어온 데이터를 저장할 메모리 주소
			- size : 한번에 읽을 데이터의 크기ㅁ
			- count : 몇개를 읽은 것인지
			- fp : 읽어올 파일 포인터
*/
typedef struct USERDATA {
	int age;
	char name[32];
	char phone[32];
	struct USERDATA* pNext;
}USERDATA;

void LoadList() {
	FILE* fp = NULL;
	fopen_s(&fp, "listData.dat", "rb");
	if (fp == NULL) {
		puts("Error : failed to open listData.dat");
		return;
	}

	USERDATA user = { 0 };

	while (fread(&user, sizeof(USERDATA), 1, fp) > 0) {
		printf("%d, %s,%s\n", user.age, user.name, user.phone);
		memset(&user, 0, sizeof(USERDATA));
	}
	fclose(fp);
}
int main(void) {
	USERDATA aList[4] = {
	{20, "Tester01", "010-1234-1111", NULL},
	{21, "Tester02", "010-1234-2222", NULL},
	{22, "Tester03", "010-1234-3333", NULL},
	{23, "Tester04", "010-1234-4444", NULL}
	};

	aList[0].pNext = &aList[1];
	aList[1].pNext = &aList[2];
	aList[2].pNext = &aList[3];
	aList[3].pNext = NULL;

	FILE* fp = NULL;
	fopen_s(&fp, "listData.dat", "wb");
	if (fp == NULL) {
		puts("Error : failed to open listData.dat");
		return;
	}

	USERDATA* pUser = &aList[0];
	while (pUser != NULL) {
		fwrite(pUser, sizeof(USERDATA), 1, fp);
		pUser = pUser->pNext;
	}
	fclose(fp);
	LoadList();
	return 0;
}
