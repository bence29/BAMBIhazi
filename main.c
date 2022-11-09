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

typedef struct //structban NEM lehet kezdõ értéket adni sadge
{
	uint8_t body[37];
	uint8_t size;
	enum direction { up , down , left , right, upleft, downleft, upright, downright, rightdown, leftdown, rightup, leftup } dir;
	enum direction prevdir;
	bool isAlive;
} snake;

snake SnakeInit(snake mysnake){
	for(uint8_t i=0; i<37; i++)
		mysnake.body[i]=0;
	mysnake.body[15]=1; //itt indul a snake
	mysnake.size=1;   //1 méretû
	mysnake.dir=right; //persze jobbra néz
	mysnake.isAlive=true; //és él
	return mysnake;
}

uint8_t PlaceFood(snake mysnake) {  //random helyre rakja a kaját ahol nincs a kígyó
	uint8_t food;
	do{
		food=rand()%36;
	} while(mysnake.body[food]!=0);
	return food;
}

snake MoveSnake(snake mysnake) {
	uint8_t head;
	bool errorbit=true;
	for(uint8_t i=0; i<36; i++)
		if(mysnake.body[i]==mysnake.size){
			head=i;
			errorbit=false;
		}
	if(errorbit) //hibakeresés, mivel térjen vissza?
		return -1;
	if((head<6)&&(mysnake.dir==right)) { //az összes jobbra tartó eset
		if(mysnake.body[head+1]!=0)
			mysnake.isAlive=false;
		mysnake.body[head+1]=mysnake.size+1;
		head=37;
	}
	else if((head==6)&&(mysnake.dir==right)) {
		if(mysnake.body[0]!=0)
			mysnake.isAlive=false;
		mysnake.body[0]=mysnake.size+1;
		head=37;
	}
	else if((head<14)&&(mysnake.dir==upright)) {
		if(mysnake.body[head-7]!=0)
			mysnake.isAlive=false;
		mysnake.body[head-7]=mysnake.size+1;
		head=37;
	}
	else if((head==14)&&(mysnake.dir==upright)) {
		if(mysnake.body[0]!=0)
			mysnake.isAlive=false;
		mysnake.body[0]=mysnake.size+1;
		head=37;
	}
	else if((head<14)&&(mysnake.dir==downright)) {
		if(mysnake.body[head+8]!=0)
			mysnake.isAlive=false;
		mysnake.body[head+8]=mysnake.size+1;
		head=37;
	}
	else if((head==14)&&(mysnake.dir==downright)) {
		if(mysnake.body[15]!=0)
			mysnake.isAlive=false;
		mysnake.body[15]=mysnake.size+1;
		head=37;
	}
	else if((head<21)&&(mysnake.dir==right)) {
		if(mysnake.body[head+1]!=0)
			mysnake.isAlive=false;
		mysnake.body[head+1]=mysnake.size+1;
		head=37;
	}
	else if((head==21)&&(mysnake.dir==right)) {
		if(mysnake.body[15]!=0)
			mysnake.isAlive=false;
		mysnake.body[15]=mysnake.size+1;
		head=37;
	}
	else if((head<29)&&(mysnake.dir==upright)) { //a kígyó oldalról fel és le is tud jobbra menni
		if(mysnake.body[head-7]!=0)
			mysnake.isAlive=false;
		mysnake.body[head-7]=mysnake.size+1;
		head=37;
	}
	else if((head==29)&&(mysnake.dir==upright)) {
		if(mysnake.body[15]!=0)
			mysnake.isAlive=false;
		mysnake.body[15]=mysnake.size+1;
		head=37;
	}
	else if((head<29)&&(mysnake.dir==downright)) {
		if(mysnake.body[head+8]!=0)
			mysnake.isAlive=false;
		mysnake.body[head+8]=mysnake.size+1;
		head=37;
	}
	else if((head==29)&&(mysnake.dir==downright)) {
		if(mysnake.body[30]!=0)
			mysnake.isAlive=false;
		mysnake.body[30]=mysnake.size+1;
		head=37;
	}
	else if((head<36)&&(mysnake.dir==right)) {
		if(mysnake.body[head+1]!=0)
			mysnake.isAlive=false;
		mysnake.body[head+1]=mysnake.size+1;
		head=37;
	}
	else if((head==36)&&(mysnake.dir==right)) {
		if(mysnake.body[30]!=0)
			mysnake.isAlive=false;
		mysnake.body[30]=mysnake.size+1;
		head=37;
	}



	else if((head<=6)&&(head>0)&&(mysnake.dir==left)) { //az összes balra tartó eset
		if(mysnake.body[head-1]!=0)
			mysnake.isAlive=false;
		mysnake.body[head-1]=mysnake.size+1;
		head=37;
	}
	else if((head==0)&&(mysnake.dir==left)) {
		if(mysnake.body[6]!=0)
			mysnake.isAlive=false;
		mysnake.body[6]=mysnake.size+1;
		head=37;
	}
	else if((head<=14)&&(head>7)&&(mysnake.dir==upleft)) {
		if(mysnake.body[head-8]!=0)
			mysnake.isAlive=false;
		mysnake.body[head-8]=mysnake.size+1;
		head=37;
	}
	else if((head==7)&&(mysnake.dir==upleft)) {
		if(mysnake.body[6]!=0)
			mysnake.isAlive=false;
		mysnake.body[6]=mysnake.size+1;
		head=37;
	}
	else if((head<=14)&&(head>7)&&(mysnake.dir==downleft)) {
		if(mysnake.body[head+7]!=0)
			mysnake.isAlive=false;
		mysnake.body[head+7]=mysnake.size+1;
		head=37;
	}
	else if((head==7)&&(mysnake.dir==downleft)) {
		if(mysnake.body[21]!=0)
			mysnake.isAlive=false;
		mysnake.body[21]=mysnake.size+1;
		head=37;
	}
	else if((head<=21)&&(head>15)&&(mysnake.dir==left)) {
		if(mysnake.body[head-1]!=0)
			mysnake.isAlive=false;
		mysnake.body[head-1]=mysnake.size+1;
		head=37;
	}
	else if((head==15)&&(mysnake.dir==left)) {
		if(mysnake.body[21]!=0)
			mysnake.isAlive=false;
		mysnake.body[21]=mysnake.size+1;
		head=37;
	}
	else if((head<=29)&&(head>22)&&(mysnake.dir==upleft)) {
		if(mysnake.body[head-8]!=0)
			mysnake.isAlive=false;
		mysnake.body[head-8]=mysnake.size+1;
		head=37;
	}
	else if((head==22)&&(mysnake.dir==upleft)) {
		if(mysnake.body[21]!=0)
			mysnake.isAlive=false;
		mysnake.body[21]=mysnake.size+1;
		head=37;
	}
	else if((head<=29)&&(head>22)&&(mysnake.dir==downleft)) {
		if(mysnake.body[head+7]!=0)
			mysnake.isAlive=false;
		mysnake.body[head+7]=mysnake.size+1;
		head=37;
	}
	else if((head==22)&&(mysnake.dir==downleft)) {
		if(mysnake.body[36]!=0)
			mysnake.isAlive=false;
		mysnake.body[36]=mysnake.size+1;
		head=37;
	}
	else if((head<=36)&&(head>30)&&(mysnake.dir==left)) {
		if(mysnake.body[head-1]!=0)
			mysnake.isAlive=false;
		mysnake.body[head-1]=mysnake.size+1;
		head=37;
	}
	else if((head==30)&&(mysnake.dir==left)) {
		if(mysnake.body[36]!=0)
			mysnake.isAlive=false;
		mysnake.body[36]=mysnake.size+1;
		head=37;
	}

	//up és down-okkal folytatni!!4!!!44!

}

SegmentLCD_LowerCharSegments_TypeDef SnakeandFoodtoLCD(snake thesnake, uint8_t food, SegmentLCD_LowerCharSegments_TypeDef *disp){
    for(uint8_t j=0; j<7; j++) //disp nullázás
    	disp[j].raw=0;
    thesnake.body[food]=1; //az ételt úgy rajzoljuk ki, hogy berakjuk a snake mezõjébe
	for(uint8_t i=0; i<37; i++) { //snake konvertálása a dispre
		if(thesnake.body[i]!=0) {
			switch(i) {
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
				disp[i].a=1;
				break;
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
			case 12:
			case 13:
				disp[i-7].f=1;
				break;
			case 14:
				disp[6].b=1;
				break;
			case 15:
			case 16:
			case 17:
			case 18:
			case 19:
			case 20:
			case 21:
				disp[i-15].g=1;
				disp[i-15].m=1;
				break;
			case 22:
			case 23:
			case 24:
			case 25:
			case 26:
			case 27:
			case 28:
				disp[i-22].e=1;
				break;
			case 29:
				disp[6].c=1;
				break;
			case 30:
			case 31:
			case 32:
			case 33:
			case 34:
			case 35:
			case 36:
				disp[i-30].d=1;
				break;
			default:
				break;
			}
		}
	}
	return *disp;
};

// 7+5*6=37 mezõ van. uint8_t-ben elég tárolni azt, hogy a mezõ a kígyó hanyadik eleme (az 1. elem a feje)

/*		Kijelzõ kódolása:						Snake kódolása:
*    --------- 0,a --------					 --------- 0,a --------
*
*   |     \7,h  |8,j  /    |				|     \XXX  |XXX  /    |
*   |5,f   \    |    /9,k  |1,b				|5,f   \    |    /XXX  |1,b
*   |       \   |   /      |				|       \   |   /      |
*
*    --- 6,g --   -- 10,m --				 --- 6,g --   -- 6,g --
*
*   |      /    |    \11,n |				|      /    |    \ XXX |
*   |4,e  /13,q |12,p \    |2,c				|4,e  /XXX  |XXX  \    |2,c
*   |    /      |      \   |				|    /      |      \   |
*
*    --------- 3,d --------					 --------- 3,d --------
*/

int main(void)
{
  /* Chip errata */
  CHIP_Init();

  /* Enable LCD without voltage boost */
  SegmentLCD_Init(false);
  /* If first word of user data page is non-zero, enable Energy Profiler trace */
  BSP_TraceProfilerSetup();
  /* Setup SysTick Timer for 1 msec interrupts  */
  if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) {
	  while (1) ;
  }



  SegmentLCD_LowerCharSegments_TypeDef disp[7];
  disp[0].raw=8192;
  disp[1].raw=4096;
  disp[2].raw=2048;
  disp[3].raw=1024;
  disp[4].raw=512;
  disp[5].raw=256;
  disp[6].raw=128;

  SegmentLCD_LowerCharSegments_TypeDef disp2[7];
    disp2[0].raw=64;
    disp2[1].raw=32;
    disp2[2].raw=16;
    disp2[3].raw=8;
    disp2[4].raw=4;
    disp2[5].raw=2;
    disp2[6].raw=1;
    SegmentLCD_LowerCharSegments_TypeDef disp3[7];
    for(int j=0; j<7; j++)
    	disp3[j].raw=0;

    // 7+5*6=37 mezõ van. uint8_t-ben elég tárolni azt, hogy a mezõ a kígyó hanyadik eleme (az 1. elem a feje)

    /*		Kijelzõ kódolása:						Snake kódolása:
    *    --------- 0,a --------					 --------- 0,a --------
    *
    *   |     \7,h  |8,j  /    |				|     \XXX  |XXX  /    |
    *   |5,f   \    |    /9,k  |1,b				|5,f   \    |    /XXX  |1,b
    *   |       \   |   /      |				|       \   |   /      |
    *
    *    --- 6,g --   -- 10,m --				 --- 6,g --   -- 6,g --
    *
    *   |      /    |    \11,n |				|      /    |    \ XXX |
    *   |4,e  /13,q |12,p \    |2,c				|4,e  /XXX  |XXX  \    |2,c
    *   |    /      |      \   |				|    /      |      \   |
    *
    *    --------- 3,d --------					 --------- 3,d --------
    */

  /* Infinite loop */
  while (1) {
	  SegmentLCD_AllOff();
	  snake mysnake;
	  mysnake=SnakeInit(mysnake);  //próba, hogy megy-e a kirajzolás stb.
	  uint8_t food=PlaceFood(mysnake);  //random helyre most lerakok egy kaját
	  SegmentLCD_LowerCharSegments_TypeDef mydisplay[7];
	  *mydisplay=SnakeandFoodtoLCD(mysnake, food, mydisplay);
	  SegmentLCD_LowerSegments(mydisplay);
	  Delay(1000);
	  //"mozduljon el" a snake
	  mysnake.body[15]=0;
	  mysnake.body[16]=1;
	  food=PlaceFood(mysnake);
	  *mydisplay=SnakeandFoodtoLCD(mysnake, food, mydisplay);
	  SegmentLCD_LowerSegments(mydisplay);
	  Delay(1000);
	  mysnake.body[16]=0;
	  mysnake.body[14]=1;
	  mysnake.body[29]=1;
	  mysnake.body[36]=1;
	  food=PlaceFood(mysnake);
	  *mydisplay=SnakeandFoodtoLCD(mysnake, food, mydisplay);
	  SegmentLCD_LowerSegments(mydisplay);
	  Delay(1000);
//	  Delay(1000);
//	  SegmentLCD_LowerSegments(disp);
//	  Delay(1000);
//	  SegmentLCD_LowerSegments(disp2);
//	  Delay(1000);
//	  SegmentLCD_LowerSegments(disp3);
//	  Delay(1000);
/*	  SegmentLCD_AllOff();
	  LCD_SegmentSet(0, 14, true);
	  Delay(2000);
	  LCD_SegmentSet(1, 14, true);
	  Delay(2000);
	  LCD_SegmentSet(2, 14, true);
	  Delay(2000);
	  LCD_SegmentSet(3, 14, true);
	  Delay(2000);
	  LCD_SegmentSet(4, 14, true);
	  Delay(2000);
	  LCD_SegmentSet(5, 14, true);
	  Delay(2000);
	  LCD_SegmentSet(6, 14, true);
	  Delay(2000);
	  LCD_SegmentSet(7, 14, true);
	  Delay(2000);
	  LCD_SegmentSet(8, 14, true);
	  Delay(2000);
	  LCD_SegmentSet(9, 14, true);
	  Delay(2000);
	  LCD_SegmentSet(10, 14, true);
	  Delay(2000);
	  LCD_SegmentSet(11, 14, true);
	  Delay(2000);
	  LCD_SegmentSet(12, 14, true);
	  Delay(2000);*/
	  /*{  1                                                    \
	          .com[0] = 1, .com[1] = 1, .com[2] = 5, .com[3] = 7,       \
	          .bit[0] = 13, .bit[1] = 14, .bit[2] = 14, .bit[3] = 14,   \
	          .com[4] = 7, .com[5] = 3, .com[6] = 4, .com[7] = 2,       \
	          .bit[4] = 13, .bit[5] = 13, .bit[6] = 13, .bit[7] = 13,   \
	          .com[8] = 3, .com[9] = 2, .com[10] = 4, .com[11] = 6,     \
	          .bit[8] = 14, .bit[9] = 14, .bit[10] = 14, .bit[11] = 14, \
	          .com[12] = 5, .com[13] = 6,                               \
	          .bit[12] = 13, .bit[13] = 13                              \
	        },
	  */
//	  LCD_SegmentSet(1, 13, true);
//	  Delay(1000);
//	  LCD_SegmentSet(1, 14, true);
//	  Delay(1000);
//	  LCD_SegmentSet(5, 14, true);
//	  Delay(1000);
//	  LCD_SegmentSet(7, 14, true);
//	  Delay(1000);
//	  LCD_SegmentSet(7, 13, true);
//	  Delay(1000);
//	  LCD_SegmentSet(3, 13, true);
//	  Delay(1000);
//	  LCD_SegmentSet(4, 13, true);
//	  LCD_SegmentSet(4, 14, true);
//	  Delay(1000);
//	  LCD_SegmentSet(2, 13, true);
//	  Delay(1000);
//	  LCD_SegmentSet(3, 14, true);
//	  Delay(1000);
//	  LCD_SegmentSet(2, 14, true);
//	  Delay(1000);
//	  LCD_SegmentSet(4, 14, true);
//	  Delay(1000);
//	  LCD_SegmentSet(6, 14, true);
//	  Delay(1000);
//	  LCD_SegmentSet(5, 13, true);
//	  Delay(1000);
//	  LCD_SegmentSet(6, 13, true);
//	  Delay(1000);
  }
}
