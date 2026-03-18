#pragma once

typedef enum MY_MENU
{
	EXIT, NEW, SEARCH, SEARCH_RANGE, PRINT, REMOVE
} MY_MENU;


MY_MENU PrintMenu(void);
void PrintList(int wait);
void EventLoopRun(void);
void AddNewUser(void); 
