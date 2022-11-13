#include "snake.h"

typedef struct //structban NEM lehet kezdõ értéket adni sadge
{
	uint8_t body[37];
	uint8_t size;
	enum direction { upup , downdown , leftleft , rightright, upleft, downleft, upright, downright, rightdown, leftdown, rightup, leftup } dir;
// dir elsõ tagja a múltbeli irány, hátsó tagja pedig a következõ írány, ez egyértelmûen maghatározza a kígyó irányát
	bool isAlive;
} snake;

snake SnakeInit(snake mysnake){
	for(uint8_t i=0; i<37; i++)
		mysnake.body[i]=0;
	mysnake.body[15]=1; //itt indul a snake
	mysnake.size=1;   //1 méretû
	mysnake.dir=rightright; //persze jobbra néz
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

snake NextDir(snake mysnake, char uartdir) {
	if(uartdir=='j') //ha jobbra fordul
		switch(mysnake.dir) {
			case(rightright):
			case(upright):
			case(downright):
				mysnake.dir=rightdown;
				break;
			case(leftleft):
			case(upleft):
			case(downleft):
				mysnake.dir=leftup;
				break;
			case(rightdown):
			case(leftdown):
			case(downdown):
				mysnake.dir=downleft;
				break;
			case(rightup):
			case(leftup):
			case(upup):
				mysnake.dir=upright;
				break;
			default:
				mysnake.size=0;
				mysnake.dir=rightright;
				mysnake.isAlive=false;
				for(uint8_t i; i<36; i++)
					mysnake.body[i]=0;
				break;
		}
	else if(uartdir=='b') //ha balra fordul
		switch(mysnake.dir) {
			case(rightright):
			case(upright):
			case(downright):
				mysnake.dir=rightup;
				break;
			case(leftleft):
			case(upleft):
			case(downleft):
				mysnake.dir=leftdown;
				break;
			case(rightdown):
			case(leftdown):
			case(downdown):
				mysnake.dir=downright;
				break;
			case(rightup):
			case(leftup):
			case(upup):
				mysnake.dir=upleft;
				break;
			default:
				mysnake.size=0;
				mysnake.dir=rightright;
				mysnake.isAlive=false;
				for(uint8_t i; i<36; i++)
					mysnake.body[i]=0;
				break;
		}
	else {
		char kifele[16]="Rossz karakter!";
		for(uint8_t i=0;i<15;i++)
			USART_Tx(UART0, kifele[i]);
	}
	return mysnake;
}

snake MoveSnake(snake mysnake, uint8_t fruit) { //snake-et mozgatja, nézi a magábaharapást és a gyümölcsevést is, és lekezeli
	uint8_t head, headforfruit; //head lesz az, amit nézni fog az irány mellett ahhoz, hogy merre léptesse a kígyót, de a head a lépés után el lesz rontva, hogy átugorja a többi if else-t, a headforfruit megmarad, hogy a gyümölcs check-re megmaradjon a fej helyzete
	bool errorbit=true; //ha ez nem fordul false-ba és nem marad úgy, akkor size!=(legnagyobb mysnake.body[] tagjával, valami nagyon nem oké)
	for(uint8_t i=0; i<36; i++)
		if(mysnake.body[i]==mysnake.size){
			head=i;
			errorbit=false; //ha egyezik a size a body egyik tagjával, akkor vagy egyezik a size a body size-jával, vagy kisebb, de nem nagyobb legalább, nem lesz hiba
		}
	for(uint8_t i=0; i<36; i++)
		if(mysnake.body[i]>mysnake.size)
			errorbit=true; //így már biztos, hogy a size egyezik a body legnagyobb tagjával, ha nem akkor error van
	if(errorbit) {//hibakeresés, mivel térjen vissza?
		mysnake.isAlive=false;
		for(uint8_t i=0; i<37; i++)
			mysnake.body[i]=0;
		mysnake.size=0;
		mysnake.dir=rightright;
		return mysnake;
	}
	headforfruit=head;
	bool fruiteaten=true;
	if((mysnake.dir==upup)||mysnake.dir==downdown) {  //upup , downdown , leftleft , rightright, upleft, downleft, upright, downright, rightdown, leftdown, rightup, leftup
		if(head>=7&&head<=14) {
			if(mysnake.body[head+15]!=0)
				mysnake.isAlive=false;
			mysnake.body[head+15]=mysnake.size+1;
			head=37;
		}
		else if(head>=22&&head<=29) {
			if(mysnake.body[head-15]!=0)
				mysnake.isAlive=false;
			mysnake.body[head-15]=mysnake.size+1;
			head=37;
		}
		else {
			errorbit=true;
			head=37;
		}
	}
	else if(mysnake.dir==leftleft) {
		if((head>=1&&head<=6)||(head>=16&&head<=21)||(head>=31&&head<=36)) {
			if(mysnake.body[head-1]!=0)
				mysnake.isAlive=false;
			mysnake.body[head-1]=mysnake.size+1;
			head=37;
		}
		else if(head==0||head==15||head==30) {
			if(mysnake.body[head+6]!=0)
				mysnake.isAlive=false;
			mysnake.body[head+6]=mysnake.size+1;
			head=37;
		}
		else {
			errorbit=true;
			head=37;
		}
	}
	else if(mysnake.dir==rightright) {
		if((head>=0&&head<=5)||(head>=15&&head<=20)||(head>=30&&head<=35)) {
			if(mysnake.body[head+1]!=0)
				mysnake.isAlive=false;
			mysnake.body[head+1]=mysnake.size+1;
			head=37;
		}
		else if(head==6||head==21||head==36) {
			if(mysnake.body[head-6]!=0)
				mysnake.isAlive=false;
			mysnake.body[head-6]=mysnake.size+1;
			head=37;
		}
		else {
			errorbit=true;
			head=37;
		}
	}
	else if(mysnake.dir==upleft) {
		if((head>=8&&head<=14)||(head>=23&&head<=29)) {
			if(mysnake.body[head-8]!=0)
				mysnake.isAlive=false;
			mysnake.body[head-8]=mysnake.size+1;
			head=37;
		}
		else if(head==7||head==22) {
			if(mysnake.body[head-1]!=0)
				mysnake.isAlive=false;
			mysnake.body[head-1]=mysnake.size+1;
			head=37;
		}
		else {
			errorbit=true;
			head=37;
		}
	}
	else if(mysnake.dir==downleft) {
		if((head>=8&&head<=14)||(head>=23&&head<=29)) { //aaaa
			if(mysnake.body[head+7]!=0)
				mysnake.isAlive=false;
			mysnake.body[head+7]=mysnake.size+1;
			head=37;
		}
		else if(head==7||head==22) {
			if(mysnake.body[head+14]!=0)
				mysnake.isAlive=false;
			mysnake.body[head+14]=mysnake.size+1;
			head=37;
		}
		else {
			errorbit=true;
			head=37;
		}
	}
	else if(mysnake.dir==upright) {
		if((head>=7&&head<=13)||(head>=22&&head<=28)) {
			if(mysnake.body[head-7]!=0)
				mysnake.isAlive=false;
			mysnake.body[head-7]=mysnake.size+1;
			head=37;
		}
		else if(head==14||head==29) {
			if(mysnake.body[head-14]!=0)
				mysnake.isAlive=false;
			mysnake.body[head-14]=mysnake.size+1;
			head=37;
		}
		else {
			errorbit=true;
			head=37;
		}
	}
	else if(mysnake.dir==downright) {
		if((head>=7&&head<=13)||(head>=22&&head<=28)) {
			if(mysnake.body[head+8]!=0)
				mysnake.isAlive=false;
			mysnake.body[head+8]=mysnake.size+1;
			head=37;
		}
		else if(head==14||head==29) {
			if(mysnake.body[head+1]!=0)
				mysnake.isAlive=false;
			mysnake.body[head+1]=mysnake.size+1;
			head=37;
		}
		else {
			errorbit=true;
			head=37;
		}
	}
	else if(mysnake.dir==rightdown) {
		if((head>=0&&head<=6)||(head>=15&&head<=21)) {
			if(mysnake.body[head+8]!=0)
				mysnake.isAlive=false;
			mysnake.body[head+8]=mysnake.size+1;
			head=37;
		}
		else if(head>=30&&head<=36) {
			if(mysnake.body[head-22]!=0)
				mysnake.isAlive=false;
			mysnake.body[head-22]=mysnake.size+1;
			head=37;
		}
		else {
			errorbit=true;
			head=37;
		}
	}
	else if(mysnake.dir==leftdown) {
		if((head>=0&&head<=6)||(head>=15&&head<=21)) {
			if(mysnake.body[head+7]!=0)
				mysnake.isAlive=false;
			mysnake.body[head+7]=mysnake.size+1;
			head=37;
		}
		else if(head>=30&&head<=36) {
			if(mysnake.body[head-23]!=0)
				mysnake.isAlive=false;
			mysnake.body[head-23]=mysnake.size+1;
			head=37;
		}
		else {
			errorbit=true;
			head=37;
		}
	}
	else if(mysnake.dir==rightup) {
		if((head>=15&&head<=21)||(head>=30&&head<=36)) {
			if(mysnake.body[head-7]!=0)
				mysnake.isAlive=false;
			mysnake.body[head-7]=mysnake.size+1;
			head=37;
		}
		else if(head>=0&&head<=6) {
			if(mysnake.body[head+23]!=0)
				mysnake.isAlive=false;
			mysnake.body[head+23]=mysnake.size+1;
			head=37;
		}
		else {
			errorbit=true;
			head=37;
		}
	}
	else if(mysnake.dir==leftup) {
		if((head>=15&&head<=21)||(head>=30&&head<=36)) {
			if(mysnake.body[head-8]!=0)
				mysnake.isAlive=false;
			mysnake.body[head-8]=mysnake.size+1;
			head=37;
		}
		else if(head>=0&&head<=6) {
			if(mysnake.body[head+22]!=0)
				mysnake.isAlive=false;
			mysnake.body[head+22]=mysnake.size+1;
			head=37;
		}
		else {
			errorbit=true;
			head=37;
		}
	} //198 sornyi csoda
	if(errorbit) {//hibakeresés, mivel térjen vissza?
		mysnake.isAlive=false;
		for(uint8_t i=0; i<37; i++)
			mysnake.body[i]=0;
		mysnake.size=0;
		mysnake.dir=rightright;
		return mysnake;
	}
	mysnake.size++; //növeljük a size méretét, feltételezve, hogy megette a gyümölcsöt
	if(headforfruit!=*fruit) {//ha a fej helyzete a gyümölcsben van, akkor megette, tehát a kígyó méretét nem kell csökkenteni, amúgy meg igen, és...
		for(uint8_t i=0; i<36; i++)
			mysnake.body[i]--;
		mysnake.size--;   //mivel nem ette meg a gyümölcsöt, csökkentsük vissza a méretet
		fruiteaten=false;
	}
	if(fruiteaten)
		fruit=PlaceFood(mysnake);
	return mysnake; //még nem jó, le kell rakni új gyümölcsöt!!!!4!!!!4!
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
