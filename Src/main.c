#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "uart.h"
#include "gpio.h"
#include "gpio_exti.h"

#define BUFFER_SIZE 128
#define USART_SR_RXNE 		(1U<<5)

//static void exti_pr13_callback(void);
void EXTI15_10_IRQHandler(void);
void print_help_menu(void);
void print_sysinfo(void);
void uart_clear_screen(void);

typedef struct {
	uint8_t buffer[BUFFER_SIZE];
	volatile uint16_t head_idx; //65,536 max theoretical buffer size
	volatile uint16_t tail_idx;
} RingBuffer;

RingBuffer rx_buffer = {{0},0,0};

#define CMD_BUFF_SIZE 32
uint8_t cmd_buffer[CMD_BUFF_SIZE];
uint16_t cmd_idx = 0;

const char ANSI_CLEAR[] = "\033[2J";

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
		//processing interrupts such as buttons
		//processing the incoming bytes from the buffer

		if(rx_buffer.head_idx != rx_buffer.tail_idx){
			uint8_t incoming_byte = rx_fifo.buffer[rx_fifo.tail];
			rx_fifo.tail = (rx_fifo.tail + 1) % BUFFER_SIZE;

			//Temporary debug line
			printf("Received: 0x%02X\r\n", c);

			if (c == 0x08 || c == 0x7F) // Handles both Ctrl+M and DEL dependent on minicom settings
			{
				uart_clear_screen();
				print_help_menu();
				uart_clear_screen();
				cmd_idx = 0;
			}


			else if (incoming_byte == '\n' || incoming_byte == '\r'){
				if(cmd_idx > 0 && cmd_idx < CMD_BUFF_SIZE - 1){
					cmd_buffer[cmd_idx]= '\0';
				}
//				else {
//					printf("error: unable to properly parse string.");
//					return -1;
//				}

		 	 //print back to computer through Rx >>> need a buffer
				if (strncmp(cmd_buffer, "echo ",5)==0){
					parse_echo_payload(cmd_buffer + 5);
				}


				else if (strcmp(cmd_buffer, "led on") == 0){
					led_on();
					printf("LED ON\r\n");
				}

				else if (strcmp(cmd_buffer, "led off") == 0){
					led_off();
					printf("LED OFF\r\n");
				}

				else if (strcmp(cmd_buffer, "led toggle") == 0){
					led_toggle();
					bool led_on = get_led_state();
					if(led_on){
						printf("LED ON\r\n");
					}
					else{
						printf("LED OFF\r\n");
					}
				}

				else if (strcmp(cmd_buffer, "sys_info")== 0){
					 print_sysinfo ();
				}
			}

			else if(cmd_index < (CMD_BUFF_SIZE - 1)){
				cmd_buffer[cmd_index++] = incoming_byte;
			}
			}

		}

		for(volatile int i = 0; i < 10000; i++);

		printf("UART DEBUG CONSOLE (System Ready)\r\n"
			"press: CTRL+H for options\r\n");

	}
}

//void clear_cmd_buffer(void){
//	uint32_t *cb32 = (uint32_t *)cmd_buffer;
//	size_t words = sizeof(cmd_buffer)/4;
//	size_t rem = len % 4;
//
//	while(words--){
//		*cb32++ = 0;
//	}
//
//	uint8_t *p8 = (uint8_t *)cb32;
//	while(rem--){
//		*p8++ = 0;
//	}
//}

void parse_echo_payload(char *payload){
	char *start = strchr(payload, '\"');
	char *end = strrchr(payload, '\"');

	if(start != NULL && end != NULL && start != end){
		uint32_t length = end - start - 1;

		printf("%.*s\r\n", length, start + 1);
	}
	else{
		printf("Error: Echo message must be enclosed in double quotes.\r\n");
	}
}


int void uart_clear_screen(void){
	for(int i = 0; ANSI_CLEAR[i] != '\0'; i++){
		uart_write((int)ANSI_CLEAR[i]);
	}
}

void print_sysinfo(void){
//processor speed / baud rate / memory size
	printf("processor speed: %d\r\n"
		   "flash size: %dKB\r\n"
		   "SRAM size: 128KBr\n"
		   "baud rate: %d \r\n"
		   , SYS_FREQ, *(uint16_t *)0x1FF7A22, DBG_UART_BAUDRATE);
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

void print_help_menu(void){
printf("HELP MENU\r\n"
	   "echo \"<message>\": This display a message back on host machine.\r\n"
	   "led on: This command will turn the led on.\r\n"
       "led off: This will turn the led off.\r\n"
	   "led toggle: This will toggle the led switch on/off.\r\n"
       "sys info: This will display vital system information including "
       "board name, processor speed, baud rate, memory size\r\n");
}
