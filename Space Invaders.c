#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

#define mapWidth 80
#define mapHeight 25

typedef struct {
    float x, y;
    float width, height;
    char cType;
}TObject;

char map[mapHeight][mapWidth+1];
TObject fighter;
TObject *moving;
int movingLength;


void clearMap() {

    for (int i = 0; i < mapWidth; i++)
        map[0][i] = ' ';
    for (int j = 0; j < mapHeight; j++)
        sprintf(map[j], map[0]);
}

void ShowMap() {

    for (int j = 0; j < mapHeight; j++)
        printf("%s\n", map[j]);
}

void SetObjectPos(TObject *obj, float xPos, float yPos) {

    (*obj).x = xPos;
    (*obj).y = yPos;
}

void InitObject(TObject *obj, float xPos, float yPos, float oWidth, float oHeight, char inType) {

    SetObjectPos(obj, xPos, yPos);
    (*obj).width = oWidth;
    (*obj).height = oHeight;
    (*obj).cType = inType;
}

void PutObjectOnMap(TObject obj) {

    int ix = (int)round(obj.x);
    int iy = (int)round(obj.y);
    int iWidth = (int)round(obj.width);
    int iHeight = (int)round(obj.height);

    for (int i = ix; i < (ix + iWidth); i++)
        for (int j = iy; j < (iy + iHeight); j++)
            map[j][i] = obj.cType;
}

void setCur(int x, int y) {

    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

TObject *GetMewMoving() {

    movingLength++;
    moving = realloc(moving, sizeof(*moving) *movingLength);
    return moving + movingLength -1;
}

void CreateWave() {

    movingLength = 0; 
    moving = realloc(moving, 0);

    InitObject(&fighter, 39, 24, 2, 1, '@');

    InitObject(GetMewMoving(), 36, 1, 2, 1, '!');
    InitObject(GetMewMoving(), 40, 1, 2, 1, '!');
    InitObject(GetMewMoving(), 44, 1, 2, 1, '!');
    InitObject(GetMewMoving(), 48, 1, 2, 1, '!');

    InitObject(GetMewMoving(), 32, 3, 2, 1, '+');
    InitObject(GetMewMoving(), 36, 3, 2, 1, '+');
    InitObject(GetMewMoving(), 40, 3, 2, 1, '+');
    InitObject(GetMewMoving(), 44, 3, 2, 1, '+');
    InitObject(GetMewMoving(), 48, 3, 2, 1, '+');
    InitObject(GetMewMoving(), 52, 3, 2, 1, '+');

    InitObject(GetMewMoving(), 28, 5, 2, 1, '?');
    InitObject(GetMewMoving(), 32, 5, 2, 1, '?');
    InitObject(GetMewMoving(), 36, 5, 2, 1, '?');
    InitObject(GetMewMoving(), 40, 5, 2, 1, '?');
    InitObject(GetMewMoving(), 44, 5, 2, 1, '?');
    InitObject(GetMewMoving(), 48, 5, 2, 1, '?');
    InitObject(GetMewMoving(), 52, 5, 2, 1, '?');
    InitObject(GetMewMoving(), 56, 5, 2, 1, '?');

    InitObject(GetMewMoving(), 24, 7, 2, 1, '*');
    InitObject(GetMewMoving(), 28, 7, 2, 1, '*');
    InitObject(GetMewMoving(), 32, 7, 2, 1, '*');
    InitObject(GetMewMoving(), 36, 7, 2, 1, '*');
    InitObject(GetMewMoving(), 40, 7, 2, 1, '*');
    InitObject(GetMewMoving(), 44, 7, 2, 1, '*');
    InitObject(GetMewMoving(), 48, 7, 2, 1, '*');
    InitObject(GetMewMoving(), 52, 7, 2, 1, '*');
    InitObject(GetMewMoving(), 56, 7, 2, 1, '*');
    InitObject(GetMewMoving(), 60, 7, 2, 1, '*');

    InitObject(GetMewMoving(), 24, 9, 2, 1, '*');
    InitObject(GetMewMoving(), 28, 9, 2, 1, '*');
    InitObject(GetMewMoving(), 32, 9, 2, 1, '*');
    InitObject(GetMewMoving(), 36, 9, 2, 1, '*');
    InitObject(GetMewMoving(), 40, 9, 2, 1, '*');
    InitObject(GetMewMoving(), 44, 9, 2, 1, '*');
    InitObject(GetMewMoving(), 48, 9, 2, 1, '*');
    InitObject(GetMewMoving(), 52, 9, 2, 1, '*');
    InitObject(GetMewMoving(), 56, 9, 2, 1, '*');
    InitObject(GetMewMoving(), 60, 9, 2, 1, '*');

    InitObject(GetMewMoving(), 24, 11, 2, 1, '*');
    InitObject(GetMewMoving(), 28, 11, 2, 1, '*');
    InitObject(GetMewMoving(), 32, 11, 2, 1, '*');
    InitObject(GetMewMoving(), 36, 11, 2, 1, '*');
    InitObject(GetMewMoving(), 40, 11, 2, 1, '*');
    InitObject(GetMewMoving(), 44, 11, 2, 1, '*');
    InitObject(GetMewMoving(), 48, 11, 2, 1, '*');
    InitObject(GetMewMoving(), 52, 11, 2, 1, '*');
    InitObject(GetMewMoving(), 56, 11, 2, 1, '*');
    InitObject(GetMewMoving(), 60, 11, 2, 1, '*');
}

int main() {

    CreateWave();

    do {

        clearMap();

        for (int i = 0; i < movingLength; i++)
            PutObjectOnMap(moving[i]);
        PutObjectOnMap(fighter);

        setCur(0, 0);
        ShowMap(10);

        Sleep(10);
    
    }
    while (GetKeyState(VK_ESCAPE) >= 0);
    

   return 0;
}