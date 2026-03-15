#include "list.h"
#include "Test.h"
#include "Stack&Queue.h"
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