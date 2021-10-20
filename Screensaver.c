#include <stdio.h>
#include <windows.h>

void ScreensaverStart() {

    system("cls");
    printf("\n\n\n\n\n \t\t\t\t  Space Invaders \n\n \t\t\t\t  Left A \n\n \t\t\t\t  Right D \n\n \t\t\t\t  Shoot SPACE \n\n \t\t\t\t  Exit ESCAPE");
    Sleep(5000);
    system("cls");
}

void ScreensaverGameOver() {

    system("cls");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n \t\t\t\t  GAME\n \t\t\t\t  OVER");
    Sleep(5000);
    system("cls");
}

void ScreensaverFinish() {

    system("cls");
    printf("\n\n\n\n\n\n\n \t\t  Congratulations, you have defended the earth. \n\n \t Pavel Sergeyevich Vavilov, Lead Developer \n\n \t Developer-Creator of Levels Smychkova Anna Evgenievna \n\n \t Developer Vladimir Sergeyevich Panov");
    Sleep(5000);
    system("cls");
}