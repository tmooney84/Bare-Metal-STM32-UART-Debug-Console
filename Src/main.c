#include <stdio.h>
#include <stdbool.h>
#include "uart.h"
#include "gpio.h"
#include "gpio_exti.h"

#define BUFFER_SIZE 128
#define USART_SR_RXNE 		(1U<<5)

//static void exti_pr13_callback(void);
void EXTI15_10_IRQHandler(void);
void print_help_menu(void);

typedef struct {
	uint8_t buffer[BUFFER_SIZE];
	volatile uint16_t head_idx; //65,536 max theoretical buffer size
	volatile uint16_t tail_idx;
} RingBuffer;

RingBuffer rx_buffer = {{0},0,0};
uint8_t cmd_buffer[BUFFER_SIZE];


//!!! REMOVE???
uint8_t g_btn_press;



int main(void)
{

	/*Initialize debug UART*/
	uart_init();

	/*Initialize LED*/
	led_init();

	/*Initialize EXTI*/
	pc13_exti_init();

	for(volatile int i = 0; i < 10000; i++);

	printf(
			"Hello from STM32 NUCLEO-F411RE...\r\n"
			"\r\n"
			"UART DEBUG CONSOLE (System Ready)\r\n"
			"press: CTRL+H for options\r\n"
			);


	while(1)
	{
		//processing the incoming bytes from the buffer

		//create command buffer >>> may need same \0 cap logic to be safe???
		//or is it ok coming back from processing buffer?






		/*parse incoming info waiting for \r\n \r \n then cap
		 * it with \0>>> then use strncmp/strcmp






		if echo:
		 	 print back to computer through Rx >>> need a buffer

		if led on:
			led_on();
			printf("LED ON\r\n");

		if led off:
			led_off()
			printf("LED OFF\r\n");

		if led toggle:
			led_toggle()
			bool led_on = get_led_state();
			if(led_on){
				printf("LED ON\r\n");
			}
		else{
			printf("LED OFF\r\n");
			}
		if sys_info:
			print_sysinfo (processor speed / baud rate / memory size)

		if CTRL + H:
			print_help_menu();




		for(volatile int i = 0; i < 10000; i++);

		printf("UART DEBUG CONSOLE (System Ready)\r\n"
			"press: CTRL+H for options\r\n")


		 */
	}
}
//static void exti_pr13_callback(void){
//	printf("BTN Pressed...\r\n");
//	led_toggle();
//
//	bool led_on = get_led_state();
//	if(led_on){
//		printf("LED ON\r\n");
//	}
//	else{
//		printf("LED OFF\r\n");
//	}
//}

void EXTI15_10_IRQHandler(void){
	if((EXTI->PR & LINE13)!=0)
	{
		/*Clear PR flag*/
		EXTI->PR |=LINE13;

		//Do something...
	//	exti_pr13_callback();
		printf("BTN Pressed...\r\n");
	led_toggle();

	bool led_on = get_led_state();
	if(led_on){
		printf("LED ON\r\n");
	}
	else{
		printf("LED OFF\r\n");
	}
	}
}

void USART2_IRQHandler(void){
	if(USART2->SR & USART_SR_RXNE){
		uint8_t data = (uint8_t)(USSRT2->DR & 0xFF);

		//Calculate the next head position
		uint16_t next = (rx_buffer.head + 1) % BUFFER_SIZE;

		//If buffer isn't full, store data
		if(next != rx_buffer.tail){
			rx_buffer.buffer[rx_buffer.head] = data;
			rx_buffer.head = next;
		}
	}
}


/*static void exti_uart_rx_callback(void){
	Needs to be able to handle ctrl + H for help menu
		-echo
		-led on
		-led off
		-sys info: processor speed / baud rate / memory size


*/
//*** Handler for Rx input
//void EXTI15_10_IRQHandler(void){



//will this need to go into a buffer before going to the host
		 // machine due to size?

		inline void print_help_menu(){
		printf("echo "<message>": This display a message back on host machine.\r\n"
		"led on: This command will turn the led on.\r\n"
		"led off: This will turn the led off.\r\n"
		"led toggle: This will toggle the led switch on/off.\r\n"
		"sys info: This will display vital system information including "
		"board name, processor speed, baud rate, memory size\r\n");
		return;
		}
