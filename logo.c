#include <stdio.h>
#include <windows.h>

#include "logo.h"

void ScreensaverLogo() {

    system("cls");
    system("color F4");
    printf("\n\n\n\n    \t\t\t\t            __                  ____");
    printf("\n  \t\t\t\t \\     /       |              |");
    printf("\n  \t\t\t\t  _____   ___  |         ____ |____");
    printf("\n  \t\t\t\t  \\   /        |        |___       |");
    printf("\n  \t\t\t\t   \\ / ______  |_____   |____  ____|");
    printf("\n\n\n  \t\t\t\t          COMPANY  PRESENTS");
    Sleep(4000);
    system("color 0F");
    system("cls");
}

