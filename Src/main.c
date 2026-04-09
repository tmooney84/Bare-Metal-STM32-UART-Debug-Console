#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "uart.h"
#include "gpio.h"
#include "gpio_exti.h"

#define BUFFER_SIZE 128

//static void exti_pr13_callback(void);
void EXTI15_10_IRQHandler(void);
void print_help_menu(void);
void print_sysinfo(void);
void uart_clear_screen(void);
void parse_echo_payload(char *payload);

typedef struct {
	char buffer[BUFFER_SIZE];
	volatile uint16_t head_idx; //65,536 max theoretical buffer size
	volatile uint16_t tail_idx;
} RingBuffer;

RingBuffer rx_buffer = {{0},0,0};

#define CMD_BUFF_SIZE 32
char cmd_buffer[CMD_BUFF_SIZE];
uint16_t cmd_idx = 0;

const char ANSI_CLEAR[] = "\033[2J";

//!!! REMOVE???
uint8_t g_btn_press;



int main(void)
{
	uart_init();

	led_init();

	pc13_exti_init();

	for(volatile int i = 0; i < 10000; i++);

	printf(
			"Hello from STM32 NUCLEO-F411RE...\r\n"
			"UART DEBUG CONSOLE (System Ready)\r\n"
			"press: CTRL+H for options\r\n\r\n"
			"\r\n"
			);

	while(1)
	{
		if(rx_buffer.head_idx != rx_buffer.tail_idx){
			uint8_t incoming_byte = rx_buffer.buffer[rx_buffer.tail_idx];
			rx_buffer.tail_idx = (rx_buffer.tail_idx + 1) % BUFFER_SIZE;


			if (incoming_byte == 0x08 || incoming_byte == 0x7F) // Handles both Ctrl+M and DEL dependent on minicom settings
			{
				uart_clear_screen();
				print_help_menu();
				cmd_idx = 0;
			}


			else if (incoming_byte == '\n' || incoming_byte == '\r'){
				if(cmd_idx > 0 && cmd_idx < CMD_BUFF_SIZE - 1){
					cmd_buffer[cmd_idx]= '\0';
				}

				if (strncmp(cmd_buffer, "echo ",5)==0){
					parse_echo_payload(cmd_buffer + 5);
				}


				else if (strcmp(cmd_buffer, "led on") == 0){
					led_on();
					printf("LED ON\r\n\r\n");
				}

				else if (strcmp(cmd_buffer, "led off") == 0){
					led_off();
					printf("LED OFF\r\n\r\n");
				}

				else if (strcmp(cmd_buffer, "led toggle") == 0){
					led_toggle();
					bool led_on = get_led_state();
					if(led_on){
						printf("LED ON\r\n\r\n");
					}
					else{
						printf("LED OFF\r\n\r\n");
					}
				}

				else if (strcmp(cmd_buffer, "sysinfo")== 0){
					 print_sysinfo();
				}
				else {
					printf("Unknown command: %s\r\n\r\n", cmd_buffer);
				}

				cmd_idx = 0;

				printf("UART DEBUG CONSOLE (System Ready)\r\n"
					   "press: CTRL+H for options\r\n\r\n");
			}

			else if(cmd_idx < (CMD_BUFF_SIZE - 1)){
				cmd_buffer[cmd_idx++] = (char)incoming_byte;
			}

		}

		for(volatile int i = 0; i < 10000; i++);



	}
	return 0;
}

void parse_echo_payload(char *payload){
	char *start = strchr(payload, '\"');
	char *end = strrchr(payload, '\"');

	if(start != NULL && end != NULL && start != end){
		uint32_t length = end - start - 1;

		printf("%.*s\r\n\r\n", (int)length, start + 1);
	}
	else{
		printf("Error: Echo message must be enclosed in double quotes.\r\n\r");
	}
}


void uart_clear_screen(void){
	for(int i = 0; ANSI_CLEAR[i] != '\0'; i++){
		uart_write((int)ANSI_CLEAR[i]);
	}
}

void print_sysinfo(void){
	printf("processor speed: %d\r\n"
		   "flash size: %dKB\r\n"
		   "SRAM size: 128KB\r\n"
		   "baud rate: %d \r\n\r\n"
		   , SYS_FREQ, *(uint16_t *)0x1FFF7A22, DBG_UART_BAUDRATE);
}

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
		printf("LED ON\r\n\r\n");
	}
	else{
		printf("LED OFF\r\n\r\n");
	}
	}
}

void USART2_IRQHandler(void){
	if(USART2->SR & USART_SR_RXNE){
		char data = (char)(USART2->DR & 0xFF);

		//Calculate the next head position
		char next = (rx_buffer.head_idx + 1) % BUFFER_SIZE;

		//If buffer isn't full, store data
		if(next != rx_buffer.tail_idx){
			rx_buffer.buffer[rx_buffer.head_idx] = data;
			rx_buffer.head_idx = next;
		}
	}
}

void print_help_menu(void){
printf("HELP MENU\r\n\r\n"
	   "echo \"<message>\": This display a message back on host machine.\r\n"
	   "led on: This command will turn the led on.\r\n"
       "led off: This will turn the led off.\r\n"
	   "led toggle: This will toggle the led switch on/off.\r\n"
       "sysinfo: This will display vital system information including "
       "board name, processor speed, baud rate, memory size\r\n\r\n");
}
