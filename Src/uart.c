#include <stdint.h>
#include "uart.h"


static void uart_set_baudrate(uint32_t periph_clk,uint32_t baudrate);

int __io_putchar(int ch)
{
	uart_write(ch);
	return ch;
}

void uart_init(void)
{
	/*Disable global interrupts*/
	__disable_irq();

	/*Enable clock access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*Set the mode of PA2 to alternate function mode*/
	GPIOA->MODER &=~(1U<<4);
	GPIOA->MODER |=(1U<<5);

	/*Set alternate function type to AF7(UART2_TX)*/
	GPIOA->AFR[0] |=(1U<<8);
	GPIOA->AFR[0] |=(1U<<9);
	GPIOA->AFR[0] |=(1U<<10);
	GPIOA->AFR[0] &=~(1U<<11);

	/*Set the mode of PA3 to alternate function mode*/
	GPIOA->MODER &=~(1U<<6);
	GPIOA->MODER |=(1U<<7);

	/* Set PA3 to Pull-Up */
	GPIOA->PUPDR &= ~(1U << 7); // Clear bit 7
	GPIOA->PUPDR |=  (1U << 6); // Set bit 6 (01 = Pull-up)

	/*Set alternate function type to AF7(UART2_RX)*/
	GPIOA->AFR[0] |=(1U<<12);
	GPIOA->AFR[0] |=(1U<<13);
	GPIOA->AFR[0] |=(1U<<14);
	GPIOA->AFR[0] &=~(1U<<15);

	/*Enable clock access to UART2*/
     RCC->APB1ENR |=	UART2EN;

	/*Configure uart baudrate*/
     uart_set_baudrate(APB1_CLK,DBG_UART_BAUDRATE);

	/*Configure transfer and receive directions*/
     USART2->CR1 |= (CR1_TE | CR1_RE | CR1_RXNEIE);

	/*Enable UART Module*/
     USART2->CR1 |= CR1_UE;

     /*Enable USART2 interrupt in the NVIC*/
     NVIC_EnableIRQ(USART2_IRQn);

     /*Enable global interrupts*/
     __enable_irq();
}



void uart_write(int ch)
{
	/*Make sure transmit data register is empty*/
	while(!(USART2->SR & SR_TXE)){}

	/*Write to transmit data register*/
	USART2->DR =(ch & 0xFF);
}

static uint16_t compute_uart_bd(uint32_t periph_clk,uint32_t baudrate)
{
	return((periph_clk + (baudrate/2U))/baudrate);
}

static void uart_set_baudrate(uint32_t periph_clk,uint32_t baudrate)
{
	USART2->BRR = compute_uart_bd(periph_clk,baudrate);
}

