#pragma once
#include "segmentlcd.h"
#include "segmentlcd_individual.h"

typedef struct{} snake;
snake SnakeInit(snake mysnake);
uint8_t PlaceFood(snake mysnake);
snake NextDir(snake mysnake, char uartdir);
snake MoveSnake(snake mysnake, uint8_t *fruit);
SegmentLCD_LowerCharSegments_TypeDef SnakeandFoodtoLCD(snake thesnake, uint8_t food, SegmentLCD_LowerCharSegments_TypeDef *disp);
