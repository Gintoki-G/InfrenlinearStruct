#include "Stack&Queue.h"
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
