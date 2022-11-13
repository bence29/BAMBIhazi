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
#include "timer.h"

extern volatile int UARTvalue;
extern volatile bool UARTflag;
extern volatile bool TIMERflag;
volatile bool UARThappened;

int main(void)
{
  /* Chip errata */
  CHIP_Init();
  UartInit();
  TimerInit();
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
	  snake mysnake;
	  SegmentLCD_LowerCharSegments_TypeDef mydisplay[7];
	  mysnake=SnakeInit(mysnake);  //próba, hogy megy-e a kirajzolás stb.
	  uint8_t food=16;  //random helyre most lerakok egy kaját

	  if(UARTflag) {
		//UARTvalue=USART_RxDataGet(UART0); //itt NEM szabad kiolvasni
	  	UARTflag=false;
	  	USART_Tx(UART0, UARTvalue);
	  	UARThappened=true;
	  }
	  if(TIMERflag) {
		if(UARThappened)
			mysnake=NextDirUART(mysnake, UARTvalue);
		else
			mysnake=NextDirNoUART(mysnake);
		UARThappened=false;
	  	TIMERflag=false;
	  	mysnake=MoveSnake(mysnake, &food);
	  	*mydisplay=SnakeandFoodtoLCD(mysnake, food, mydisplay);
		SegmentLCD_LowerSegments(mydisplay);
	  }
  }
}
