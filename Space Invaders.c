#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

#define mapWidth 80
#define mapHeight 25

typedef struct {
    float x, y;
    float width, height;
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

void InitObject(TObject *obj, float xPos, float yPos, float oWidth, float oHeight) {

    SetObjectPos(obj, xPos, yPos);
    (*obj).width = oWidth;
    (*obj).height= oHeight;
}

void PutObjectOnMap(TObject obj) {

    int ix = (int)round(obj.x);
    int iy = (int)round(obj.y);
    int iWidth = (int)round(obj.width);
    int iHeight = (int)round(obj.height);

    for (int i = ix; i < (ix + iWidth); i++)
        for (int j = iy; j < (iy + iHeight); j++)
            map[j][i] = '@';
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

    InitObject(&fighter, 39, 23, 3, 2);

    InitObject(GetMewMoving(), 37, 3 , 1, 1);
    InitObject(GetMewMoving(), 39, 3 , 1, 1);
    InitObject(GetMewMoving(), 41, 3 , 1, 1);
    InitObject(GetMewMoving(), 43, 3 , 1, 1);
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