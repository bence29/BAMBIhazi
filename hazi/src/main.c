#include "em_device.h"
#include "em_chip.h"

#include "em_cmu.h"
#include "bsp.h"
#include "bsp_trace.h"
#include <stdint.h>
#include <stdbool.h>

#include "segmentlcd.h"
#include "segmentlcd_individual.h"

#include "stdlib.h" //v�letlen kaja hely gener�l�shoz

#include "uart.h"
#include "snake.h"
#include "timer.h"


extern volatile int UARTvalue;
extern volatile bool UARTflag;
extern volatile bool TIMERflag;
volatile bool UARThappened; //m�shogy kell mozogni att�l f�gg�en, hogy az UART �ltal beolvasott j/b mikor j�tt be
extern volatile time_t t;

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
  /* Infinite loop */
  SegmentLCD_AllOff();

  srand((unsigned) time(&t)); //random sz�m gener�tor seedj�nek be�ll�t�sa
  snake mysnake; //snake p�ld�ny l�trehoz�sa
  SegmentLCD_LowerCharSegments_TypeDef mydisplay[7]; //kijelz� l�trehoz�sa
  bool SymbolFlip=false;

  mysnake=SnakeInit(mysnake);  //snake inicializ�l�s
  uint8_t food=PlaceFood(mysnake);  //�tel inicializ�l�s
  while (1) {
	  if(UARTflag) {
	  	UARTflag=false;
	  	USART_Tx(UART0, UARTvalue);
	  	UARThappened=true;
	  }

	  if(TIMERflag) {
		 TIMERflag=false;
		 if(mysnake.isAlive) {
			if(UARThappened)
				mysnake=NextDirUART(mysnake, UARTvalue);
			else
				mysnake=NextDirNoUART(mysnake);
			UARThappened=false;
	  		mysnake=MoveSnake(mysnake, &food);
	  		*mydisplay=SnakeandFoodtoLCD(mysnake, food, mydisplay);
			SegmentLCD_LowerSegments(mydisplay);
		 }
		 else {
			 SegmentLCD_AllOff();
			 if(SymbolFlip) {
				 SegmentLCD_Symbol(LCD_SYMBOL_DP2, 1);
				 SegmentLCD_Symbol(LCD_SYMBOL_DP3, 1);
				 SegmentLCD_Symbol(LCD_SYMBOL_DP4, 1);
				 SegmentLCD_Symbol(LCD_SYMBOL_DP5, 1);
				 SegmentLCD_Symbol(LCD_SYMBOL_DP6, 1);
				 SymbolFlip=false;
			 }
			 else {
				 SegmentLCD_Symbol(LCD_SYMBOL_DP2, 0);
				 SegmentLCD_Symbol(LCD_SYMBOL_DP3, 0);
				 SegmentLCD_Symbol(LCD_SYMBOL_DP4, 0);
				 SegmentLCD_Symbol(LCD_SYMBOL_DP5, 0);
				 SegmentLCD_Symbol(LCD_SYMBOL_DP6, 0);
				 SymbolFlip=true;
			 }
		 }
	  }
	  SegmentLCD_Number(mysnake.size);
  }
}
