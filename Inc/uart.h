#ifndef __UART_H__
#define __UART_H__

#include "stm32f4xx.h"

//#define USART_SR_RXNE 		(1U<<5)
#define GPIOAEN		(1U<<0)
#define UART2EN		(1U<<17)

#define DBG_UART_BAUDRATE		115200
#define SYS_FREQ				16000000
#define APB1_CLK				SYS_FREQ
#define CR1_TE					(1U<<3)
#define CR1_RE					(1U<<2)
#define CR1_UE					(1U<<13)
#define CR1_RXNEIE				(1U<<5)
#define SR_TXE					(1U<<7)



void uart_write(int ch);
void uart_init(void);

#endif
