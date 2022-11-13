#pragma once
#include "em_device.h"
#include "em_chip.h"

#include "em_cmu.h"
#include "bsp.h"
#include "bsp_trace.h"
#include <stdint.h>
#include <stdbool.h>

#include "segmentlcd.h"
#include "segmentlcd_individual.h"

#include "stdlib.h" //véletlen kaja hely generáláshoz

#include "uart.h"
#include "snake.h"

extern volatile int UARTvalue;
extern volatile bool UARTflag;

volatile uint32_t msTicks; /* counts 1ms timeTicks */

/* Local prototypes */
void Delay(uint32_t dlyTicks);

/***************************************************************************//**
 * @brief SysTick_Handler
 *   Interrupt Service Routine for system tick counter
 * @note
 *   No wrap around protection
 ******************************************************************************/
void SysTick_Handler(void)
{
  msTicks++;       /* increment counter necessary in Delay()*/
}

/***************************************************************************//**
 * @brief Delays number of msTick Systicks (typically 1 ms)
 * @param dlyTicks Number of ticks to delay
 ******************************************************************************/
void Delay(uint32_t dlyTicks)
{
  uint32_t curTicks;
  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks) ;
}

int main(void)
{
  /* Chip errata */
  CHIP_Init();
  uartinit();
  /* Enable LCD without voltage boost */
  SegmentLCD_Init(false);
  /* If first word of user data page is non-zero, enable Energy Profiler trace */
  BSP_TraceProfilerSetup();
  /* Setup SysTick Timer for 1 msec interrupts  */
  if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) {
	  while (1) ;
  }
    // 7+5*6=37 mezõ van. uint8_t-ben elég tárolni azt, hogy a mezõ a kígyó hanyadik eleme (az 1. elem a feje)
    /*		Kijelzõ kódolása:
    *    --------- 0,a --------
    *
    *   |     \7,h  |8,j  /    |
    *   |5,f   \    |    /9,k  |1,b
    *   |       \   |   /      |
    *
    *    --- 6,g --   -- 10,m --
    *
    *   |      /    |    \11,n |
    *   |4,e  /13,q |12,p \    |2,c
    *   |    /      |      \   |
    *
    *    --------- 3,d --------
    */
  /* Infinite loop */
  while (1) {
	  SegmentLCD_AllOff();
	  Delay(1000);
	  snake mysnake;
	  SegmentLCD_LowerCharSegments_TypeDef mydisplay[7];
	  mysnake=SnakeInit(mysnake);  //próba, hogy megy-e a kirajzolás stb.
	  uint8_t food=PlaceFood(mysnake);  //random helyre most lerakok egy kaját
	  *mydisplay=SnakeandFoodtoLCD(mysnake, food, mydisplay);
	  SegmentLCD_LowerSegments(mydisplay);
	  Delay(1000);
	  if(UARTflag) {
		//UARTvalue=USART_RxDataGet(UART0); //itt NEM szabad kiolvasni
	  	UARTflag = false;
	  	USART_Tx(UART0, UARTvalue);
	  	mysnake=NextDir(mysnake, UARTvalue);
	  }
	  mysnake=MoveSnake(mysnake, &food);
	  *mydisplay=SnakeandFoodtoLCD(mysnake, food, mydisplay);
	  SegmentLCD_LowerSegments(mydisplay);
	  Delay(1000);
	  //"mozduljon el" a snake
//	  mysnake.body[15]=0;
//	  mysnake.body[16]=1;
//	  food=PlaceFood(mysnake);
//	  *mydisplay=SnakeandFoodtoLCD(mysnake, food, mydisplay);
//	  SegmentLCD_LowerSegments(mydisplay);
//	  Delay(1000);
//	  mysnake.body[16]=0;
//	  mysnake.body[14]=1;
//	  mysnake.body[29]=1;
//	  mysnake.body[36]=1;
//	  food=PlaceFood(mysnake);
//	  *mydisplay=SnakeandFoodtoLCD(mysnake, food, mydisplay);
//	  SegmentLCD_LowerSegments(mydisplay);
//	  Delay(1000);
  }
}
