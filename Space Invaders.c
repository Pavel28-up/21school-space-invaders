#include <stdio.h>

#define mapWidth 80
#define mapHeight 25

char map[mapHeight][mapWidth+1];

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

int main() {

    clearMap();
    ShowMap();

   return 0;
}