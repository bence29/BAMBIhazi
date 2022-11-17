#include "snake.h"

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

snake NextDirUART(snake mysnake, char uartdir) { //ha jött UART j/b, akkor a következõ irány e függvény szerint alakul
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
			default: //hiba esetén
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
			default: //hiba esetén
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

snake NextDirNoUART(snake mysnake) { //ha nem jött UART j/b, akkor a következõ irány e függvény szerint alakul
		switch(mysnake.dir) {
			case(rightright):
			case(upright):
			case(downright):
				mysnake.dir=rightright;
				break;
			case(leftleft):
			case(upleft):
			case(downleft):
				mysnake.dir=leftleft;
				break;
			case(rightdown):
			case(leftdown):
			case(downdown):
				mysnake.dir=downdown;
				break;
			case(rightup):
			case(leftup):
			case(upup):
				mysnake.dir=upup;
				break;
			default:
				mysnake.size=0;
				mysnake.dir=rightright;
				mysnake.isAlive=false;
				for(uint8_t i; i<36; i++)
					mysnake.body[i]=0;
				break;
		}
	return mysnake;
}

//if(uarthappened) { //valahol TimerIT-ben
//	NextDirUART
//}
//else {
//	NextDirNoUART
//}

snake MoveSnake(snake mysnake, uint8_t *fruit) { //snake-et mozgatja, nézi a magábaharapást és a gyümölcsevést is, és lekezeli
	uint8_t head, headforfruit=100; //head lesz az, amit nézni fog az irány mellett ahhoz, hogy merre léptesse a kígyót, de a head a lépés után el lesz rontva, hogy átugorja a többi if else-t, a headforfruit megmarad, hogy a gyümölcs check-re megmaradjon a fej helyzete
	bool errorbit=true; //ha ez nem fordul false-ba és nem marad úgy, akkor size!=(legnagyobb mysnake.body[] tagjával, valami nagyon nem oké)
	for(uint8_t i=0; i<37; i++)
		if(mysnake.body[i]==mysnake.size){
			head=i;
			errorbit=false; //ha egyezik a size a body egyik tagjával, akkor vagy egyezik a size a body size-jával, vagy kisebb, de nem nagyobb legalább, nem lesz hiba
		}
	for(uint8_t i=0; i<37; i++)
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
	bool fruiteaten=true;
	if((mysnake.dir==upup)||mysnake.dir==downdown) {  //upup , downdown , leftleft , rightright, upleft, downleft, upright, downright, rightdown, leftdown, rightup, leftup
		if(head>=7&&head<=14) {
			if(mysnake.body[head+15]>1) //magic cucc, azért nem jó az 1-sem, mert az mozgás után a csökkentett értéken kellene nézni, hogy beleharap-e magába, 1-1=0, tehát még jó
				mysnake.isAlive=false;
			mysnake.body[head+15]=mysnake.size+1;
			headforfruit=head+15;
			head=37;
		}
		else if(head>=22&&head<=29) {
			if(mysnake.body[head-15]>1)
				mysnake.isAlive=false;
			mysnake.body[head-15]=mysnake.size+1;
			headforfruit=head-15;
			head=37;
		}
		else {
			errorbit=true;
			head=37;
		}
	}
	else if(mysnake.dir==leftleft) {
		if((head>=1&&head<=6)||(head>=16&&head<=21)||(head>=31&&head<=36)) {
			if(mysnake.body[head-1]>1)
				mysnake.isAlive=false;
			mysnake.body[head-1]=mysnake.size+1;
			headforfruit=head-1;
			head=37;
		}
		else if(head==0||head==15||head==30) {
			if(mysnake.body[head+6]>1)
				mysnake.isAlive=false;
			mysnake.body[head+6]=mysnake.size+1;
			headforfruit=head+6;
			head=37;
		}
		else {
			errorbit=true;
			head=37;
		}
	}
	else if(mysnake.dir==rightright) {
		if((head>=0&&head<=5)||(head>=15&&head<=20)||(head>=30&&head<=35)) {
			if(mysnake.body[head+1]>1)
				mysnake.isAlive=false;
			mysnake.body[head+1]=mysnake.size+1;
			headforfruit=head+1;
			head=37;
		}
		else if(head==6||head==21||head==36) {
			if(mysnake.body[head-6]>1)
				mysnake.isAlive=false;
			mysnake.body[head-6]=mysnake.size+1;
			headforfruit=head-6;
			head=37;
		}
		else {
			errorbit=true;
			head=37;
		}
	}
	else if(mysnake.dir==upleft) {
		if((head>=8&&head<=14)||(head>=23&&head<=29)) {
			if(mysnake.body[head-8]>1)
				mysnake.isAlive=false;
			mysnake.body[head-8]=mysnake.size+1;
			headforfruit=head-8;
			head=37;
		}
		else if(head==7||head==22) {
			if(mysnake.body[head-1]>1)
				mysnake.isAlive=false;
			mysnake.body[head-1]=mysnake.size+1;
			headforfruit=head-1;
			head=37;
		}
		else {
			errorbit=true;
			head=37;
		}
	}
	else if(mysnake.dir==downleft) {
		if((head>=8&&head<=14)||(head>=23&&head<=29)) {
			if(mysnake.body[head+7]>1)
				mysnake.isAlive=false;
			mysnake.body[head+7]=mysnake.size+1;
			headforfruit=head+7;
			head=37;
		}
		else if(head==7||head==22) {
			if(mysnake.body[head+14]>1)
				mysnake.isAlive=false;
			mysnake.body[head+14]=mysnake.size+1;
			headforfruit=head+14;
			head=37;
		}
		else {
			errorbit=true;
			head=37;
		}
	}
	else if(mysnake.dir==upright) {
		if((head>=7&&head<=13)||(head>=22&&head<=28)) {
			if(mysnake.body[head-7]>1)
				mysnake.isAlive=false;
			mysnake.body[head-7]=mysnake.size+1;
			headforfruit=head-7;
			head=37;
		}
		else if(head==14||head==29) {
			if(mysnake.body[head-14]>1)
				mysnake.isAlive=false;
			mysnake.body[head-14]=mysnake.size+1;
			headforfruit=head-14;
			head=37;
		}
		else {
			errorbit=true;
			head=37;
		}
	}
	else if(mysnake.dir==downright) {
		if((head>=7&&head<=13)||(head>=22&&head<=28)) {
			if(mysnake.body[head+8]>1)
				mysnake.isAlive=false;
			mysnake.body[head+8]=mysnake.size+1;
			headforfruit=head+8;
			head=37;
		}
		else if(head==14||head==29) {
			if(mysnake.body[head+1]>1)
				mysnake.isAlive=false;
			mysnake.body[head+1]=mysnake.size+1;
			headforfruit=head+1;
			head=37;
		}
		else {
			errorbit=true;
			head=37;
		}
	}
	else if(mysnake.dir==rightdown) {
		if((head>=0&&head<=6)||(head>=15&&head<=21)) {
			if(mysnake.body[head+8]>1)
				mysnake.isAlive=false;
			mysnake.body[head+8]=mysnake.size+1;
			headforfruit=head+8;
			head=37;
		}
		else if(head>=30&&head<=36) {
			if(mysnake.body[head-22]>1)
				mysnake.isAlive=false;
			mysnake.body[head-22]=mysnake.size+1;
			headforfruit=head-22;
			head=37;
		}
		else {
			errorbit=true;
			head=37;
		}
	}
	else if(mysnake.dir==leftdown) {
		if((head>=0&&head<=6)||(head>=15&&head<=21)) {
			if(mysnake.body[head+7]>1)
				mysnake.isAlive=false;
			mysnake.body[head+7]=mysnake.size+1;
			headforfruit=head+7;
			head=37;
		}
		else if(head>=30&&head<=36) {
			if(mysnake.body[head-23]>1)
				mysnake.isAlive=false;
			mysnake.body[head-23]=mysnake.size+1;
			headforfruit=head-23;
			head=37;
		}
		else {
			errorbit=true;
			head=37;
		}
	}
	else if(mysnake.dir==rightup) {
		if((head>=15&&head<=21)||(head>=30&&head<=36)) {
			if(mysnake.body[head-7]>1)
				mysnake.isAlive=false;
			mysnake.body[head-7]=mysnake.size+1;
			headforfruit=head-7;
			head=37;
		}
		else if(head>=0&&head<=6) {
			if(mysnake.body[head+23]>1)
				mysnake.isAlive=false;
			mysnake.body[head+23]=mysnake.size+1;
			headforfruit=head+23;
			head=37;
		}
		else {
			errorbit=true;
			head=37;
		}
	}
	else if(mysnake.dir==leftup) {
		if((head>=15&&head<=21)||(head>=30&&head<=36)) {
			if(mysnake.body[head-8]>1)
				mysnake.isAlive=false;
			mysnake.body[head-8]=mysnake.size+1;
			headforfruit=head-8;
			head=37;
		}
		else if(head>=0&&head<=6) {
			if(mysnake.body[head+22]>1)
				mysnake.isAlive=false;
			mysnake.body[head+22]=mysnake.size+1;
			headforfruit=head+22;
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
		for(uint8_t i=0; i<37; i++)
			if(mysnake.body[i]!=0)
				mysnake.body[i]--;
		mysnake.size--;   //mivel nem ette meg a gyümölcsöt, csökkentsük vissza a méretet
		fruiteaten=false;
	}
	if(fruiteaten) //ha megette a gyümit, akkor újat le kell rakni
		*fruit=PlaceFood(mysnake);
	return mysnake;
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
