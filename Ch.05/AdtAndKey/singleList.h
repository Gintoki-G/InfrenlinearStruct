#pragma once
// 데이터와 자료구조를 나눈다.
typedef struct USERDATA
{
	int age;
	char name[32];
	char phone[32];
} USERDATA;
/*
1. struct MYNODE {...} -> 구조체 "정의중"
2. 이 시점에서는 아직 MYNODE라는 typedef이름이 완전히 만들어지기 전
3. 그래서 내부에서는 반드시 struct MYNODE라고 써야한다.
--------------------------------------------------------------------
MYNODE* pPrev : 아직 MYNODE 정의가 안끝나서 이렇게 쓰면 안됌!
--------------------------------------------------------------------
strcut MYNODE* : 항상 써도 괜찮다
MYNODE * : 정의 중에는 쓰면 안된다!
*/

/*
GetKey → 정렬/검색용 함수 포인터
- char * : 문자열을 반환하는 함수
- (*GetKey) : 함수 "포인터 변수" 이름
- (void * thisPointer) : 매개변수
*/
typedef struct MYNODE
{
	void* pData;
	struct MYNODE* pNext;
	struct MYNODE* pPrev;
	char* (*GetKey)(void* thisPointer);
} MYNODE;

/*
extern : 이 변수는 여기서 정의된 게 아니라,
		 다른 파일에 이미 만들어져 있다.
-----------------------------------------------------------------
왜 이렇게 나눌까?
: 여러 파일에서 같은 전역변수를 쓰기 위해서
'MYNODE g_HeadNode;' 를 쓰면 헤더 포함할 때마다 변수 복사된다.
>> 중복 정의 에러 발생한다.
-----------------------------------------------------------------
extern : 다른 파일에 있는 변수다
헤더 : 선언만
소스 : 실제 생성
*/

extern MYNODE g_HeadNode;
extern MYNODE g_TailNode;

void InitList(void);
void ReleaseList(void);
void AddNewNode(int age, char* pszName, char* pszPhone);
void SortListByAge(void);