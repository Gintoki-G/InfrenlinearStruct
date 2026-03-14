#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct USERDATA
{
	int age;
	char name[32];
	char phone[32];
	struct USERDATA* pNext;
} USERDATA;

// 전역변수 헤드 포인터
USERDATA *g_pHeadNode = NULL;

// 노드 전체를 free 하는 함수
// head Node부터 pNext로 갱신해 가며 노드를 삭제
// 방향 : ->
void ReleaseList(void)
{
	USERDATA *pTmp = g_pHeadNode;
	USERDATA* pDelete;
	while (pTmp != NULL)
	{
		// pTmp를 갱신하기 전에 삭제할 노드를 백업해둔다
		// 그래야 주소를 삭제할 노드의 다음 노드의 주소를 잃지 않는다.
		pDelete = pTmp;
		pTmp = pTmp->pNext;

		printf("Delete: %d, %s, %s\n",
			pDelete->age, pDelete->name, pDelete->phone);

		free(pDelete);
	}
}


/*
void* calloc(size_t count, size_t size)
- 동적 메모리를 할당하는 함수
- count : 몇개를 만들 것인지 
- size : 하나의 크기
- malloc 과의 차이점 - malloc - 메모리만 할당(쓰레기값 있음)
					 - calloc - 메모리 할당 + 0으로 초기화

*/
void AddNewNode(int age, char* pszName, char* pszPhone)
{
	USERDATA* pNewNode = calloc(1, sizeof(USERDATA));
	pNewNode->age = age;
	strcpy_s(pNewNode->name, sizeof(pNewNode->name), pszName);
	strcpy_s(pNewNode->phone, sizeof(pNewNode->phone), pszPhone);

	/*
	<노드의 머리에 새로운 노드를 추가하는 방식>
		if (g_pHeadNode != NULL)
		pNewNode->pNext = g_pHeadNode;
		g_pHeadNode = pNewNode;
	*/
	
	// 노드의 꼬리에 새로운 노드를 추가하는 방식
	if (g_pHeadNode == NULL) // 기존의 노드가 없을 경우
		g_pHeadNode = pNewNode;
	else
	{
		USERDATA* pTmp = g_pHeadNode;

		//pTmp의 pNext가 NULL인 곳을 찾았어 그 뒤에 새로운 노드를 추가
		/*
		*         {pTmp가 3번 자리에 왔을때 새 노드 추가}
		*					   |
		 기존 - [1] -> [2] -> [3] -> NULL
		 추가후 - [1] -> [2] -> [3] -> [4] -> NULL
		*/
		while (pTmp->pNext != NULL)
			pTmp = pTmp->pNext;

		pTmp->pNext = pNewNode;
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

	ReleaseList();
	return 0;
}
