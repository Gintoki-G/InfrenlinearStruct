#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

typedef struct USERDATA
{
	int age;
	char name[32];
	char phone[32];
	struct USERDATA* pPrev;
	struct USERDATA* pNext;
} USERDATA;

USERDATA g_HeadNode = { 0, "__Dummy Head__" };
USERDATA g_TailNode = { 0, "__Dummy Tail__" };


USERDATA* AddNewNodeAtTail(int age, const char* pszName, const char* pszPhone)
{
	USERDATA* pNewNode = calloc(1, sizeof(USERDATA));
	pNewNode->age = age;
	strcpy_s(pNewNode->name, sizeof(pNewNode->name), pszName);
	strcpy_s(pNewNode->phone, sizeof(pNewNode->phone), pszPhone);

	//pNewNode 정리
	USERDATA* pPrevNode = g_TailNode.pPrev;
	pNewNode->pPrev = g_TailNode.pPrev;
	pNewNode->pNext = &g_TailNode;

	//pNewNode 앞뒤 정리
	pPrevNode->pNext = pNewNode;
	g_TailNode.pPrev = pNewNode;

	return pNewNode;
}

USERDATA * AddNewNodeAtHead(int age, const char* pszName, const char* pszPhone)
{
	USERDATA* pNewNode = calloc(1, sizeof(USERDATA));
	pNewNode->age = age;
	strcpy_s(pNewNode->name, sizeof(pNewNode->name), pszName);
	strcpy_s(pNewNode->phone, sizeof(pNewNode->phone), pszPhone);

	//pNewNode 정리
	USERDATA* pNextNode = g_HeadNode.pNext;
	pNewNode->pPrev = &g_HeadNode;
	pNewNode->pNext = pNextNode;

	//pNewNode 앞뒤 정리
	pNextNode->pPrev = pNewNode;
	g_HeadNode.pNext = pNewNode;

	return pNewNode;
}

// 리스트 초기화
void InitList(void)
{
	g_HeadNode.pNext = &g_TailNode;
	g_TailNode.pPrev = &g_HeadNode;
}

void ReleaseList(void)
{
	USERDATA* pTmp = g_HeadNode.pNext;
	USERDATA* pDelete;
	while (pTmp != NULL && pTmp != &g_TailNode)
	{
		pDelete = pTmp;
		pTmp = pTmp->pNext;

		printf("Delete: [%p] %d, %s, %s [%p]\n", pDelete,
			pDelete->age, pDelete->name, pDelete->phone, pDelete->pNext);

		free(pDelete);
	}

	InitList();
}

void InitDummyData(void)
{
	//Input
	AddNewNodeAtTail(10, "Hoon", "010-1111-1111");
	AddNewNodeAtTail(11, "Choi", "010-2222-2222");
	AddNewNodeAtTail(13, "Jang", "010-3333-3333");
}

// 노드를 반환함 
USERDATA* SearchByName(const char* pszName)
{
	USERDATA* pTmp = g_HeadNode.pNext;
	while (pTmp != NULL)
	{
		if (strcmp(pTmp->name, pszName) == 0)
		{
			printf("\"%s\": Found\n", pszName);
			return pTmp;
		}

		pTmp = pTmp->pNext;
	}

	printf("\"%s\": Not found\n", pszName);
	return NULL;
}

/*
RemoveNode 사용법
	USERDATA* p = SearchByName("Choi");
	if(p)
		RemoveNode(p);
*/
void RemoveNode(USERDATA* pRemove)
{
	USERDATA* pPrev = pRemove->pPrev;
	USERDATA* pNext = pRemove->pNext;

	pPrev->pNext = pRemove->pNext;
	pNext->pPrev = pRemove->pPrev;

	printf("RemoveNode(): %s\n", pRemove->name);
	free(pRemove);
}

//내가 만든 함수
int RemoveByName(const char* pszName)
{
	USERDATA* pNode = SearchByName(pszName);

	if (pNode == NULL)
		return 0;

	RemoveNode(pNode);
	return 1;
}

void PrintList(void)
{
	USERDATA* pTmp = &g_HeadNode;
	while (pTmp != NULL)
	{
		printf("[%p] %d, %s, %s [%p]\n", pTmp,
			pTmp->age, pTmp->name, pTmp->phone, pTmp->pNext);
		pTmp = pTmp->pNext;
	}
	putchar('\n');
}

void PrintListReverse(void)
{
	USERDATA* pTmp = g_TailNode.pPrev;
	while (pTmp != NULL)
	{
		printf("[%p] %d, %s, %s [%p]\n", pTmp,
			pTmp->age, pTmp->name, pTmp->phone, pTmp->pNext);
		pTmp = pTmp->pPrev;
	}
	putchar('\n');
}

void TestStep01(void)
{
	puts("TestStep01()----------------------------");
	AddNewNodeAtTail(10, "Choi", "010-2222-2222");
	AddNewNodeAtTail(10, "Hoon", "010-1111-1111");
	AddNewNodeAtTail(13, "Jang", "010-3333-3333");
	PrintList();

	USERDATA* pPrev = SearchByName("Choi");
	if (pPrev != NULL)
		RemoveNode(pPrev);

	PrintListReverse();
	ReleaseList();
	putchar('\n');
}

void TestStep02(void)
{
	puts("TestStep02()----------------------------");
	AddNewNodeAtTail(10, "Choi", "010-2222-2222");
	AddNewNodeAtTail(10, "Hoon", "010-1111-1111");
	AddNewNodeAtTail(13, "Jang", "010-3333-3333");
	PrintList();

	USERDATA* pPrev = SearchByName("Hoon");
	if (pPrev != NULL)
		RemoveNode(pPrev);
	ReleaseList();
	putchar('\n');
}

void TestStep03(void)
{
	puts("TestStep03()----------------------------");
	AddNewNodeAtTail(10, "Choi", "010-2222-2222");
	AddNewNodeAtTail(10, "Hoon", "010-1111-1111");
	AddNewNodeAtTail(13, "Jang", "010-3333-3333");
	PrintList();

	USERDATA* pPrev = SearchByName("Jang");
	if (pPrev != NULL)
		RemoveNode(pPrev);
	ReleaseList();
	putchar('\n');
}

int IsEmpty(void)
{
	if (g_TailNode.pPrev == &g_HeadNode)
		return 1;

	return 0;
}
/* --------------------Stack-------------------------*/
USERDATA* PushTail(USERDATA* Node) {
	USERDATA* pPush;
	pPush = AddNewNodeAtTail(Node->age, Node->name, Node->phone);
	return pPush;
}

USERDATA* PushHead(USERDATA* Node) {
	USERDATA* pPush;
	pPush = AddNewNodeAtHead(Node->age, Node->name, Node->phone);
	return pPush;
}

// 리스트에서 특정 노드를 제거하지만 free는 하지 않음
// Pop() 같은 함수에서 노드를 반환하기 위해 사용
USERDATA* RemoveNodeFromPop(USERDATA* pNode)
{
	// 삭제할 노드의 앞노드
	USERDATA* pPrev = pNode->pPrev;

	// 삭제할 노드의 뒤 노드
	USERDATA* pNext = pNode->pNext;

	// 앞 노드가 삭제 노드의 다음을 가리키도록
	pPrev->pNext = pNext;

	// 뒤 노드가 삭제 노드의 이전을 가리키도록
	pNext->pPrev = pPrev;

	// 삭제된 노드는 리스트와 완전히 분리
	pNode->pPrev = NULL;
	pNode->pNext = NULL;

	// 제거된 노드를 반환
	return pNode;
}


/*
	       PopHead
			  |
DummyHead -> [1] -> [2] -> Tail
	
*/
// 리스트의 가장 앞 노드를 제거
USERDATA* PopHead(void)
{
	if (IsEmpty())
		return NULL;
	// Head 다음 노드가 실제 첫 번째 데이터 노드
	USERDATA* pPop = g_HeadNode.pNext;
	return RemoveNodeFromPop(g_HeadNode.pNext);
}


/*
					   PopTail
					      |
	D.H -> [1] -> [2] -> [3] -> D.T
*/
// 리스트의 가장 뒤 노드를 제거
USERDATA* PopTail(void)
{
	if (IsEmpty())
		return NULL;

	// Tail 이전 노드가 실제 마지막 데이터 노드
	USERDATA* pPop = g_TailNode.pPrev;
	return RemoveNodeFromPop(g_TailNode.pPrev);
}

/*----------------------------Queue-----------------------------------------*/
USERDATA* Dequeue(void)
{
	return PopHead();
}

void Enqueue(USERDATA* pUser)
{
	AddNewNodeAtTail(pUser->age, pUser->name, pUser->phone);
}
int main(void)
{
	InitList();

	USERDATA user = { 10, "Test01", };
	//Push(&user);
	Enqueue(&user);
	user.age = 11;
	strcpy_s(user.name, sizeof(user.name), "Test02");
	//Push(&user);
	Enqueue(&user);
	user.age = 12;
	strcpy_s(user.name, sizeof(user.name), "Test03");
	//Push(&user);
	Enqueue(&user);

	PrintList();

	for (int i = 0; i < 3; ++i)
	{
		//USERDATA* pUser = Pop();
		USERDATA* pUser = Dequeue();
		printf("Pop: %d, %s\n", pUser->age, pUser->name);
		free(pUser);
		PrintList();
	}

	ReleaseList();
	return 0;

	//TestStep01();
	//TestStep02();
	//TestStep03();
}
