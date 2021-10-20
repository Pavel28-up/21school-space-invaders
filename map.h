#include <stdio.h>


#ifndef MAP_H
#define MAP_H

#define mapWidth 80
#define mapHeight 25

char map[mapHeight][mapWidth+1];

void clearMap();
void ShowMap();
void WavOnMap();


#endif

