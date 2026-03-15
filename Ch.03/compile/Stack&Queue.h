#ifndef _H__STACKQUEUE__
#define _H__STACKQUEUE__
#include "list.h"

/* --------------------Stack-------------------------*/
USERDATA* PushTail(USERDATA* Node);

USERDATA* PushHead(USERDATA* Node);

// 리스트에서 특정 노드를 제거하지만 free는 하지 않음
// Pop() 같은 함수에서 노드를 반환하기 위해 사용
USERDATA* RemoveNodeFromPop(USERDATA* pNode);

USERDATA* PopHead(void);

USERDATA* PopTail(void);

/*----------------------------Queue-----------------------------------------*/
USERDATA* Dequeue(void);

void Enqueue(USERDATA* pUser);

#endif
