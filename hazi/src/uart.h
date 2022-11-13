#pragma once
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_usart.h"
#include "em_gpio.h"

volatile int UARTvalue;
volatile bool UARTflag;

void UART0_RX_IRQHandler(void);
int USART_RxNonblocking(USART_TypeDef *usart);
void UartInit();
