#include <stdio.h>
#include <stdlib.h>

typedef enum MY_MENU {EXIT, NEW, SEARCH, PRINT, REMOVE} MY_MENU;

MY_MENU PrintMenu(void)
{
    MY_MENU input = 0;

    system("cls");
    printf("[1]New\t[2]Search\t[3]Print\t[4]Remove\t[0]Exit\n");
    scanf_s("%d%*c", &input);
    return input;
}


int main(void)
{
    MY_MENU menu = 0;

    while ((menu = PrintMenu()) != 0)
    {
        switch (menu)
        {
        case NEW:
            break;

        case SEARCH:
            break;

        case PRINT:
            break;

        case REMOVE:
            break;

        default:
            break;
        }
    }
    puts("Bye~!");
    return 0;
}

