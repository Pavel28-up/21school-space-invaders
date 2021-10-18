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
int life = 5;

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
    
        life--;
        system("color 4F");
        Sleep(500);
        CreateWave(wav);
        if (life == 0) {
            ScreensaverGameOver();
            wav = 1;
            life = 5;
        }
        system("color 0F");
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
                if (   (pul[i].cType == '.') 
                    && (pul[i].vertSpeed < 0)
                    && (pul[0].y >= (moving[0].y + moving[0].height)) ) {

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
                    if (moving[j].cType == 'o') {
                    score += 450;
                    DeleteMoving(j);
                    j--;
                    DeletePul(i);
                    i--;
                    continue;
                    }
                }
            }
            if ((movingLength == (j >= 0)) == (pul[0].cType == '.') ) {
                for (int j = 0; j < movingLength; j++) 

                    wav++;

                    if (wav > 4) {
                         
                        //ScreensaverFinish();
                        wav = 0;
                        life = 5;
                    } else {
                       // ScreensaverWav();
                        CreateWave(wav);
                    }
            }
        }
    }
}

void HorizonObject(TObject *obj) {

    obj[0].x +=obj[0].horizSpeed;

    for (int i = 0; i < brickLength; i++)  {
        if (IsCollision(obj[0], brick[i])) {
            obj[0].x -= obj[0].horizSpeed;
            obj[0].horizSpeed = -obj[0].horizSpeed;
            return;
        }
    }

    for (int j = 0; j < 79; j++)  {
        if (   ((obj[0].cType == '!') && (obj[0].cType == '+') && (obj[0].cType == '?') && (obj[0].cType == '*'))
            || (obj[0].x == j++)
            || (obj[0].x == j--) ) 

            InitObject(GetMewPul(), obj[0].x, obj[0].y+1, 1, 1, '.');

     }
}

void HorizonMoving(TObject *obj) {

    for (int k = 0; k < movingLength; k++) {
    obj[0].x +=obj[0].horizSpeed;
    obj[0].cType == 'o';

        if (IsCollision(obj[0], moving[k])) {
            for (int k = 0; k < movingLength; k++)
            obj[0].x -= obj[0].horizSpeed;
            obj[0].horizSpeed = -obj[0].horizSpeed;
            return;
        }
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

void WavOnMap() {

    char t[50];
    sprintf(t, "Score: %d   Wav: %d  life: %d", score, wav, life);
    int len = strlen(t);
    for (int i = 0; i < len; i++) {

        map[0][i + 3] = t[i];
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
    if (wav == 4) {
        InitObject(GetMewBrick(), 0, 0, 1, 25, '#');
        InitObject(GetMewBrick(), 79, 0, 1, 25, '#');

            InitObject(GetMewMoving(), 34, 2, 2, 1, '!');
        InitObject(GetMewMoving(), 36, 2, 2, 1, 'o');
        InitObject(GetMewMoving(), 38, 2, 2, 1, 'o');
        InitObject(GetMewMoving(), 40, 2, 2, 1, 'o');
        InitObject(GetMewMoving(), 42, 2, 2, 1, 'o');
            InitObject(GetMewMoving(), 44, 2, 2, 1, '!');

            InitObject(GetMewMoving(), 32, 3, 2, 1, '+');
        InitObject(GetMewMoving(), 34, 3, 2, 1, 'o');
            InitObject(GetMewMoving(), 36, 3, 2, 1, '+');
        InitObject(GetMewMoving(), 38, 3, 2, 1, 'o');
        InitObject(GetMewMoving(), 40, 3, 2, 1, 'o');
            InitObject(GetMewMoving(), 42, 3, 2, 1, '+');
        InitObject(GetMewMoving(), 44, 3, 2, 1, 'o');
            InitObject(GetMewMoving(), 46, 3, 2, 1, '+');

            InitObject(GetMewMoving(), 30, 4, 2, 1, '?');
        InitObject(GetMewMoving(), 32, 4, 2, 1, 'o');
        InitObject(GetMewMoving(), 34, 4, 2, 1, 'o');
            InitObject(GetMewMoving(), 36, 4, 2, 1, '?');
        InitObject(GetMewMoving(), 38, 4, 2, 1, 'o');
        InitObject(GetMewMoving(), 40, 4, 2, 1, 'o');
            InitObject(GetMewMoving(), 42, 4, 2, 1, '?');
        InitObject(GetMewMoving(), 44, 4, 2, 1, 'o');
        InitObject(GetMewMoving(), 46, 4, 2, 1, 'o');
            InitObject(GetMewMoving(), 48, 4, 2, 1, '?');

            InitObject(GetMewMoving(), 28, 5, 2, 1, '*');
        InitObject(GetMewMoving(), 30, 5, 2, 1, 'o');
        InitObject(GetMewMoving(), 32, 5, 2, 1, 'o');
            InitObject(GetMewMoving(), 34, 5, 2, 1, '*');
        InitObject(GetMewMoving(), 36, 5, 2, 1, 'o');
        InitObject(GetMewMoving(), 38, 5, 2, 1, 'o');
        InitObject(GetMewMoving(), 40, 5, 2, 1, 'o');
        InitObject(GetMewMoving(), 42, 5, 2, 1, 'o');
            InitObject(GetMewMoving(), 44, 5, 2, 1, '*');
        InitObject(GetMewMoving(), 46, 5, 2, 1, 'o');
        InitObject(GetMewMoving(), 48, 5, 2, 1, 'o');
            InitObject(GetMewMoving(), 50, 5, 2, 1, '*');

            InitObject(GetMewMoving(), 28, 6, 2, 1, '*');
        InitObject(GetMewMoving(), 30, 6, 2, 1, 'o');
        InitObject(GetMewMoving(), 32, 6, 2, 1, 'o');
            InitObject(GetMewMoving(), 34, 6, 2, 1, '*');
        InitObject(GetMewMoving(), 36, 6, 2, 1, 'o');
        InitObject(GetMewMoving(), 38, 6, 2, 1, 'o');
        InitObject(GetMewMoving(), 40, 6, 2, 1, 'o');
        InitObject(GetMewMoving(), 42, 6, 2, 1, 'o');
            InitObject(GetMewMoving(), 44, 6, 2, 1, '*');
        InitObject(GetMewMoving(), 46, 6, 2, 1, 'o');
        InitObject(GetMewMoving(), 48, 6, 2, 1, 'o');
            InitObject(GetMewMoving(), 50, 6, 2, 1, '*');

            InitObject(GetMewMoving(), 28, 7, 2, 1, '*');
        InitObject(GetMewMoving(), 30, 7, 2, 1, 'o');
        InitObject(GetMewMoving(), 32, 7, 2, 1, 'o');
            InitObject(GetMewMoving(), 34, 7, 2, 1, '*');
        InitObject(GetMewMoving(), 36, 7, 2, 1, 'o');
        InitObject(GetMewMoving(), 38, 7, 2, 1, 'o');
        InitObject(GetMewMoving(), 40, 7, 2, 1, 'o');
        InitObject(GetMewMoving(), 42, 7, 2, 1, 'o');
            InitObject(GetMewMoving(), 44, 7, 2, 1, '*');
        InitObject(GetMewMoving(), 46, 7, 2, 1, 'o');
        InitObject(GetMewMoving(), 48, 7, 2, 1, 'o');
            InitObject(GetMewMoving(), 50, 7, 2, 1, '*');

            InitObject(GetMewMoving(), 30, 8, 2, 1, '?');
        InitObject(GetMewMoving(), 32, 8, 2, 1, 'o');
        InitObject(GetMewMoving(), 34, 8, 2, 1, 'o');
            InitObject(GetMewMoving(), 36, 8, 2, 1, '?');
        InitObject(GetMewMoving(), 38, 8, 2, 1, 'o');
        InitObject(GetMewMoving(), 40, 8, 2, 1, 'o');
            InitObject(GetMewMoving(), 42, 8, 2, 1, '?');
        InitObject(GetMewMoving(), 44, 8, 2, 1, 'o');
        InitObject(GetMewMoving(), 46, 8, 2, 1, 'o');
            InitObject(GetMewMoving(), 48, 8, 2, 1, '?');

            InitObject(GetMewMoving(), 32, 9, 2, 1, '+');
        InitObject(GetMewMoving(), 34, 9, 2, 1, 'o');
            InitObject(GetMewMoving(), 36, 9, 2, 1, '+');
        InitObject(GetMewMoving(), 38, 9, 2, 1, 'o');
        InitObject(GetMewMoving(), 40, 9, 2, 1, 'o');
            InitObject(GetMewMoving(), 42, 9, 2, 1, '+');
        InitObject(GetMewMoving(), 44, 9, 2, 1, 'o');
            InitObject(GetMewMoving(), 46, 9, 2, 1, '+');

        InitObject(GetMewMoving(), 34, 10, 2, 1, '!');
        InitObject(GetMewMoving(), 36, 10, 2, 1, 'o');
        InitObject(GetMewMoving(), 38, 10, 2, 1, 'o');
        InitObject(GetMewMoving(), 40, 10, 2, 1, 'o');
        InitObject(GetMewMoving(), 42, 10, 2, 1, 'o');
        InitObject(GetMewMoving(), 44, 10, 2, 1, '!');
    }

    maxWav = 4;
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
    printf("\n\n\n\n\n\n\n \t\t  Congratulations, you have defended the earth. \n\n \t Pavel Sergeyevich Vavilov, Lead Developer \n\n \t Developer-Creator of Levels Smychkova Anna Evgenievna \n\n \t Developer Vladimir Sergeyevich Panov");
    Sleep(10000);
    system("cls");
}

void ScreensaverLogo() {

    system("cls");
    system("color F4");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n \t\t\t\t  VELES  COMPANY  PRESENT");
    Sleep(10000);
    system("color 0F");
    system("cls");
}

int main() {

    CreateWave(wav);
    // ScreensaverLogo();
    // ScreensaverStart();
    // ScreensaverWav();

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
        WavOnMap();

        setCur(0, 0);
        ShowMap();
        
        Sleep(10);
    
    }
    while (GetKeyState(VK_ESCAPE) >= 0);
    

   return 0;
}