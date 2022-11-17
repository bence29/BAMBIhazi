#include "timer.h"

volatile bool TIMERflag=false;

void TIMER0_IRQHandler(void){ //A timer IT ezt a függvényt hívja meg
TIMERflag=true;
TIMER_IntClear(TIMER0, TIMER_IF_OF); //a többi interrupt törlése
}

void TimerInit() {
// a periféria órajel osztójának beállítása
CMU_ClockDivSet(cmuClock_HFPER, cmuClkDiv_1);
// *******************************
// * TIMER inicializálása *
// *******************************
// idõzítõ órajelének engedélyezése
CMU_ClockEnable(cmuClock_TIMER0, true);
// létrehozzuk az inicializációhoz szükséges paraméterstruktúrát
TIMER_Init_TypeDef TIMER0_init = TIMER_INIT_DEFAULT;
// a prescaler-t átállítjuk
TIMER0_init.prescale = timerPrescale256; // timerPrescale1...timerPrescale1024
// inicializálás a paraméterstruktúrával
//void TIMER_Init(TIMER_TypeDef *timer, const TIMER_Init_TypeDef *init);
TIMER_Init(TIMER0, &TIMER0_init);
// reseteljük a számlálót
TIMER_CounterSet(TIMER0, 0); //
// beállítjuk a TOP értéket
//__STATIC_INLINE void TIMER_TopSet(TIMER_TypeDef *timer, uint32_t val)
TIMER_TopSet(TIMER0, 27344); // 14MHz/presc/TOP 27344-el 0,5 másodpercenként van, 1s=>54687,5
// töröljük az esetleges függõ megszakításokat
//__STATIC_INLINE void TIMER_IntClear(TIMER_TypeDef *timer, uint32_t flags);
TIMER_IntClear(TIMER0, TIMER_IF_OF);
// Timer IT engedélyezése
//TIMER_IntEnable(TIMER_TypeDef *timer, uint32_t flags);
TIMER_IntEnable(TIMER0, TIMER_IF_OF);
// Timer IT engedélyezése az NVIC-ben
NVIC_EnableIRQ(TIMER0_IRQn);
}
