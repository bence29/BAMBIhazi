#pragma once
#include "segmentlcd.h"
#include "segmentlcd_individual.h"
#include "stdlib.h"
#include <time.h>

typedef struct //structban NEM lehet kezdõ értéket adni sadge
{
	uint8_t body[37];
	uint8_t size;
	enum direction { upup , downdown , leftleft , rightright, upleft, downleft, upright, downright, rightdown, leftdown, rightup, leftup } dir;
// dir elsõ tagja a múltbeli irány, hátsó tagja pedig a következõ írány, ez egyértelmûen maghatározza a kígyó irányát
	bool isAlive;
} snake;

volatile time_t t;

snake SnakeInit(snake mysnake);
uint8_t PlaceFood(snake mysnake);
snake NextDirUART(snake mysnake, char uartdir);
snake NextDirNoUART(snake mysnake);
snake MoveSnake(snake mysnake, uint8_t *fruit);
SegmentLCD_LowerCharSegments_TypeDef SnakeandFoodtoLCD(snake thesnake, uint8_t food, SegmentLCD_LowerCharSegments_TypeDef *disp);
