#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

#define mapWidth 80
#define mapHeight 25

typedef struct {
    float x, y;
    float width, height;
    float horizSpeed;
    float vertSpeed;
    char cType;
    BOOL IsFiy;
}TObject;

char map[mapHeight][mapWidth+1];
TObject fighter;
TObject *brick;
int brickLength;

TObject *moving;
int movingLength;

TObject *pul;
int pulLength;

int wav = 1;
int score;
int maxWav;


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
    (*obj).horizSpeed = 0.2;
    (*obj).vertSpeed = 0;
}

void CreateWave(int wav);
void ScreensaverGameOver();

void PlayerDead() {

    system("color 4F");
    Sleep(500);
    ScreensaverGameOver();
    CreateWave(wav);
    system("color 07");
}

BOOL IsCollision(TObject o1, TObject o2);

void VertMoveObject(TObject *obj) {

    (*obj).IsFiy = TRUE;
    (*obj).vertSpeed += 0.05;
    SetObjectPos(obj, (*obj).x, (*obj).y + (*obj).vertSpeed);
    if (IsCollision(*obj, moving[0])) {

        (*obj).IsFiy = FALSE;
    }
}

TObject *GetMewMoving();
TObject *GetMewPul();

void DeleteMoving(int j) {

    movingLength--;
    moving[j] = moving[movingLength];
    moving = realloc(moving, sizeof(*moving) *movingLength);
}

void DeletePul(int i) {

    pulLength--;
    pul[i] = pul[pulLength];
    pul = realloc(pul, sizeof(*pul) *pulLength);
}

void  ScreensaverWav();
void ScreensaverFinish();

void FighterCollision() {

    for (int i = 0; i < pulLength; i++) {
        if (IsCollision(fighter, pul[i])) 
                
            PlayerDead();
    }
    for (int i = 0; i < pulLength; i++) {
     for (int j = 0; j < movingLength; j++) {
        if (IsCollision(pul[i], moving[j])) {
                if (   (pul[0].cType == '.') 
                    && (pul[0].vertSpeed < 0)
                    && ((pul[0].y >= (moving[j].y + moving[j].height)) 
                    || (pul[0].x == (moving[j].x + moving[j].width))) ) {

                    if (moving[j].cType == '!') {
                    score += 350;
                    DeleteMoving(j);
                    j--;
                    DeletePul(i);
                    i--;
                    continue;
                    }
                    if (moving[j].cType == '+') {
                    score += 250;
                    DeleteMoving(j);
                    j--;
                    DeletePul(i);
                    i--;
                    continue;
                    }
                    if (moving[j].cType == '?') {
                    score += 150;
                    DeleteMoving(j);
                    j--;
                    DeletePul(i);
                    i--;
                    continue;
                    }
                    if (moving[j].cType == '*') {
                    score += 50;
                    DeleteMoving(j);
                    j--;
                    DeletePul(i);
                    i--;
                    continue;
                    }
                }
            }
            if ( (movingLength == (j >= 0)) == (pul[0].cType == '.') ) {
                for (int j = 0; j < movingLength; j++) 
                   
                   wav++;
                if (wav > maxWav) ScreensaverFinish();;

                    
                    ScreensaverWav();
                    CreateWave(wav);
            } 
        }
    }
}

void HorizonObject(TObject *obj) {

    obj[0].x +=obj[0].horizSpeed;

    for (int i = 0; i < brickLength; i++) 
        if (IsCollision(obj[0], brick[i])) {
            obj[0].x -= obj[0].horizSpeed;
            obj[0].horizSpeed = -obj[0].horizSpeed;
            return;
    }

    for (int j = 0; j < 79; j++)  {
        if (   (obj[0].x == j++)
            || (obj[0].x == j--) ) 

            InitObject(GetMewPul(), obj[0].x, obj[0].y+1, 1, 1, '.');

     }
}

BOOL IsPosInMap(int x, int y) {

    return ((x >= 0) && (x < mapWidth) && (y >= 0) && (y < mapHeight));
}

void PutObjectOnMap(TObject obj) {

    int ix = (int)round(obj.x);
    int iy = (int)round(obj.y);
    int iWidth = (int)round(obj.width);
    int iHeight = (int)round(obj.height);

    for (int i = ix; i < (ix + iWidth); i++)
        for (int j = iy; j < (iy + iHeight); j++)
            if (IsPosInMap(i, j))
                map[j][i] = obj.cType;
}

void setCur(int x, int y) {

    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void HorizonFighter(float dx) {

    fighter.x -= dx;
    for (int i = 0; i < brickLength; i++)
        if (IsCollision(fighter, brick[i])) {

            fighter.x += dx;
            return;
        }
    fighter.x -= dx;
}

TObject *GetMewBrick() {

    brickLength++;
    brick = realloc(brick, sizeof(*brick) *brickLength);
    return brick + brickLength -1;
}

TObject *GetMewMoving() {

    movingLength++;
    moving = realloc(moving, sizeof(*moving) *movingLength);
    return moving + movingLength -1;
}

 TObject *GetMewPul() {

     pulLength++;
     pul = realloc(pul, sizeof(*pul) *pulLength);
     return pul + pulLength -1;
 }

 void pulFighter(TObject *obj) {
        
    if ((obj == &fighter)) {
        InitObject(GetMewPul(1), fighter.x +1, fighter.y -1, 1, 1, '.');
        pul[pulLength -1].vertSpeed = -1.7;
    }
 }

void PutScoreOnMap() {

    char c[50];
    sprintf(c, "Score: %d", score);
    int len = strlen(c);
    for (int i = 0; i < len; i++) {

        map[0][i + 5] = c[i];
    }
}

void WavOnMap() {

    char t[50];
    sprintf(t, "Wav: %d", wav);
    int len = strlen(t);
    for (int i = 0; i < len; i++) {

        map[0][i + 20] = t[i];
    }
}

void CreateWave(int wav) {

    brickLength = 0; 
    brick = realloc(brick, 0);
    movingLength = 0; 
    moving = realloc(moving, 0);

    InitObject(&fighter, 39, 24, 3, 1, '@');
    score = 0;

    if (wav == 1) {
        InitObject(GetMewBrick(), 0, 0, 1, 25, '#');
        InitObject(GetMewBrick(), 79, 0, 1, 25, '#');

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
    if (wav == 2) {
        InitObject(GetMewBrick(), 0, 0, 1, 25, '#');
        InitObject(GetMewBrick(), 79, 0, 1, 25, '#');

        InitObject(GetMewMoving(), 36, 1, 2, 1, '*');
        InitObject(GetMewMoving(), 40, 1, 2, 1, '*');
        InitObject(GetMewMoving(), 44, 1, 2, 1, '*');
        InitObject(GetMewMoving(), 48, 1, 2, 1, '*');

        InitObject(GetMewMoving(), 32, 3, 2, 1, '*');
        InitObject(GetMewMoving(), 36, 3, 2, 1, '*');
        InitObject(GetMewMoving(), 40, 3, 2, 1, '*');
        InitObject(GetMewMoving(), 44, 3, 2, 1, '*');
        InitObject(GetMewMoving(), 48, 3, 2, 1, '*');
        InitObject(GetMewMoving(), 52, 3, 2, 1, '*');

        InitObject(GetMewMoving(), 28, 5, 2, 1, '*');
        InitObject(GetMewMoving(), 32, 5, 2, 1, '*');
        InitObject(GetMewMoving(), 36, 5, 2, 1, '*');
        InitObject(GetMewMoving(), 40, 5, 2, 1, '*');
        InitObject(GetMewMoving(), 44, 5, 2, 1, '*');
        InitObject(GetMewMoving(), 48, 5, 2, 1, '*');
        InitObject(GetMewMoving(), 52, 5, 2, 1, '*');
        InitObject(GetMewMoving(), 56, 5, 2, 1, '*');

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
        for (int i = 0; i < movingLength; i++)
                 moving[i].horizSpeed += 1;
    }

    if (wav == 3) {
        InitObject(GetMewBrick(), 0, 0, 1, 25, '#');
        InitObject(GetMewBrick(), 79, 0, 1, 25, '#');

        InitObject(GetMewMoving(), 36, 1, 2, 1, '+');
        InitObject(GetMewMoving(), 40, 1, 2, 1, '+');
        InitObject(GetMewMoving(), 44, 1, 2, 1, '+');
        InitObject(GetMewMoving(), 48, 1, 2, 1, '+');

        InitObject(GetMewMoving(), 32, 3, 2, 1, '+');
        InitObject(GetMewMoving(), 36, 3, 2, 1, '+');
        InitObject(GetMewMoving(), 40, 3, 2, 1, '+');
        InitObject(GetMewMoving(), 44, 3, 2, 1, '+');
        InitObject(GetMewMoving(), 48, 3, 2, 1, '+');
        InitObject(GetMewMoving(), 52, 3, 2, 1, '+');

        InitObject(GetMewMoving(), 28, 5, 2, 1, '+');
        InitObject(GetMewMoving(), 32, 5, 2, 1, '+');
        InitObject(GetMewMoving(), 36, 5, 2, 1, '+');
        InitObject(GetMewMoving(), 40, 5, 2, 1, '+');
        InitObject(GetMewMoving(), 44, 5, 2, 1, '+');
        InitObject(GetMewMoving(), 48, 5, 2, 1, '+');
        InitObject(GetMewMoving(), 52, 5, 2, 1, '+');
        InitObject(GetMewMoving(), 56, 5, 2, 1, '+');

        InitObject(GetMewMoving(), 24, 7, 2, 1, '+');
        InitObject(GetMewMoving(), 28, 7, 2, 1, '+');
        InitObject(GetMewMoving(), 32, 7, 2, 1, '+');
        InitObject(GetMewMoving(), 36, 7, 2, 1, '+');
        InitObject(GetMewMoving(), 40, 7, 2, 1, '+');
        InitObject(GetMewMoving(), 44, 7, 2, 1, '+');
        InitObject(GetMewMoving(), 48, 7, 2, 1, '+');
        InitObject(GetMewMoving(), 52, 7, 2, 1, '+');
        InitObject(GetMewMoving(), 56, 7, 2, 1, '+');
        InitObject(GetMewMoving(), 60, 7, 2, 1, '+');

        InitObject(GetMewMoving(), 24, 9, 2, 1, '+');
        InitObject(GetMewMoving(), 28, 9, 2, 1, '+');
        InitObject(GetMewMoving(), 32, 9, 2, 1, '+');
        InitObject(GetMewMoving(), 36, 9, 2, 1, '+');
        InitObject(GetMewMoving(), 40, 9, 2, 1, '+');
        InitObject(GetMewMoving(), 44, 9, 2, 1, '+');
        InitObject(GetMewMoving(), 48, 9, 2, 1, '+');
        InitObject(GetMewMoving(), 52, 9, 2, 1, '+');
        InitObject(GetMewMoving(), 56, 9, 2, 1, '+');
        InitObject(GetMewMoving(), 60, 9, 2, 1, '*');

        InitObject(GetMewMoving(), 24, 11, 2, 1, '+');
        InitObject(GetMewMoving(), 28, 11, 2, 1, '+');
        InitObject(GetMewMoving(), 32, 11, 2, 1, '+');
        InitObject(GetMewMoving(), 36, 11, 2, 1, '+');
        InitObject(GetMewMoving(), 40, 11, 2, 1, '+');
        InitObject(GetMewMoving(), 44, 11, 2, 1, '+');
        InitObject(GetMewMoving(), 48, 11, 2, 1, '+');
        InitObject(GetMewMoving(), 52, 11, 2, 1, '+');
        InitObject(GetMewMoving(), 56, 11, 2, 1, '+');
        InitObject(GetMewMoving(), 60, 11, 2, 1, '+');
    }

    maxWav = 3;
}

BOOL IsCollision(TObject o1, TObject o2) {

    return ((o1.x + o1.width) > o2.x) && (o1.x < (o2.x + o2.width)) &&
           ((o1.y + o1.height) > o2.y) && (o1.y < (o2.y + o2.height));
}

void ScreensaverStart() {

    system("cls");
    printf("\n\n\n\n\n \t\t\t\t  Space Invaders \n\n \t\t\t\t  Left A \n\n \t\t\t\t  Right D \n\n \t\t\t\t  Shoot SPACE \n\n \t\t\t\t  Exit ESCAPE");
    Sleep(10000);
    system("cls");
}

void ScreensaverWav() {

    system("cls");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n \t\t\t\t  Wav: %d", wav);
    Sleep(3000);
    system("cls");
}

void ScreensaverGameOver() {

    system("cls");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n \t\t\t\t  GAME\n \t\t\t\t  OVER");
    Sleep(10000);
    system("cls");
}

void ScreensaverFinish() {

    system("cls");
    printf("\n\n\n\n\n\n\n \t\t  Congratulations, you have defended the earth. \n\n \t Pavel Sergeyevich Vavilov, Lead Developer \n\n \t Developer-Creator of Levels Smychkova Anna Evgenievna \n\n \t Developer");
    Sleep(10000);
    system("cls");
}

int main() {

    CreateWave(wav);
    ScreensaverStart();
    ScreensaverWav();
    
    do {

        clearMap();
        

        if ((&fighter) && GetKeyState(VK_SPACE) < 0) pulFighter(&fighter);
                                            
              
        if (GetKeyState('A') < 0) HorizonFighter(1);
        if (GetKeyState('D') < 0) HorizonFighter(-1);

        FighterCollision();

        for (int i = 0; i < brickLength; i++) {
            PutObjectOnMap(brick[i]);
        }
        for (int i = 0; i < movingLength; i++) {

            HorizonObject(moving +i);
            PutObjectOnMap(moving[i]);
        }
        for (int i = 0; i < pulLength; i++) {

            VertMoveObject(pul +i);
            if ((pul[i].y > mapHeight) || (pul[i].y < 0)) {

                DeletePul(i);
                i--;
                continue;
            }
            PutObjectOnMap(pul[i]);
        }
        PutObjectOnMap(fighter);
        PutScoreOnMap();
        WavOnMap();

        setCur(0, 0);
        ShowMap(10);

        Sleep(10);
    
    }
    while (GetKeyState(VK_ESCAPE) >= 0);
    

   return 0;
}