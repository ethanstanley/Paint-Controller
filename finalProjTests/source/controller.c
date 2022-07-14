/*
 * This C file contains most of the code for the Paint Controller. It contains GPIO initializations,
 * integrates the ADC functions from adc.c, sends information to python using UART communication,
 * contains polling methods, and other functions.
 */

#include <pin_mux.h>
#include <clock_config.h>
#include <stdio.h>
#include <board.h>
#include <MKL46Z4.h>
#include <fsl_debug_console.h>
#include <adc.h>

//Initialize the button pins
const int RED_LED_PIN = 29;
const int SWITCH_1_PIN = 2;
const int SWITCH_2_PIN = 16;
const int SWITCH_3_PIN = 16;
SIM_Type* global_SIM = SIM;
PORT_Type* global_PORTE = PORTE;
GPIO_Type* global_PTE = PTE;

// declare functions used to run the infinite loop for the switch
void color_button_polling();
void draw_button_polling();
void clear_button_polling();

//Initialize GLOBAL VARS
int drawCheck = 0;
int colorPaint;
int drawEnabled = 1;
int clearButton = 0;


/*------------------------------*/
/* UART communication functions */
/*------------------------------*/
/* Initialize the UART for TX (115200, 8N1) */

void init_uart(void)
{
	BOARD_InitBootPins();
    BOARD_InitBootClocks();
 	BOARD_InitDebugConsole();
}

void uart_putc (char ch)
{
	// Wait until space is available in the FIFO
	while(!(UART0->S1 & UART_S1_TDRE_MASK));
	// Send the character
	UART0->D = (uint8_t)ch;
}

void uart_puts(char *ptr_str)
{
    while(*ptr_str){
		// Replace newlines with \r\n carriage return
		if(*ptr_str == '\n') {
			uart_putc('\r');
		}
		uart_putc(*ptr_str++);
	}
}

/* Short delay between sending strings through UART */
void short_delay()
{
	for(int i=100000; i>0; i--){}
}

/* Sets up the buttons */
void buttonSetup(){
	global_SIM = global_SIM;
	global_PORTE = global_PORTE;
	global_PTE = global_PTE;

	// setup red led
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK; //Enable the clock to port E
	PORTE->PCR[RED_LED_PIN] = PORT_PCR_MUX(0b001); //Set up PTE29 as GPIO
	PTE->PDDR |= GPIO_PDDR_PDD(1 << RED_LED_PIN); // make it output
	PTE->PSOR |= GPIO_PSOR_PTSO(1 << RED_LED_PIN); // turn off LED

	// setup switch 1
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK; //Enable the clock to port C
	PORTC->PCR[SWITCH_1_PIN] &= ~PORT_PCR_MUX(0b111); // Clear PCR Mux bits for PTC2
	PORTC->PCR[SWITCH_1_PIN] |= PORT_PCR_MUX(0b001); // Set up PTC2 as GPIO
	PTC->PDDR &= ~GPIO_PDDR_PDD(1 << SWITCH_1_PIN); // make it input
	PORTC->PCR[SWITCH_1_PIN] |= PORT_PCR_PE(1); // Turn on the pull enable
	PORTC->PCR[SWITCH_1_PIN] |= PORT_PCR_PS(1); // Enable the pullup resistor
	PORTC->PCR[SWITCH_1_PIN] &= ~PORT_PCR_IRQC(0b1111); // Clear IRQC bits for PTC2
	PORTC->PCR[SWITCH_1_PIN] |= PORT_PCR_IRQC(0b1000); // Set up the IRQC to interrupt on high edge

	//switch 2
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK; //Enable the clock to port A
	PORTA->PCR[SWITCH_2_PIN] &= ~PORT_PCR_MUX(0b111); // Clear PCR Mux bits for PTA16
	PORTA->PCR[SWITCH_2_PIN] |= PORT_PCR_MUX(0b001); // Set up PTA16 as GPIO
	PTA->PDDR &= ~GPIO_PDDR_PDD(1 << SWITCH_2_PIN); // make it input
	PORTA->PCR[SWITCH_2_PIN] |= PORT_PCR_PE(1); // Turn on the pull enable
	PORTA->PCR[SWITCH_2_PIN] |= PORT_PCR_PS(1); // Enable the pullup resistor
	PORTA->PCR[SWITCH_2_PIN] &= ~PORT_PCR_IRQC(0b1111); // Clear IRQC bits for PTA16
	PORTA->PCR[SWITCH_2_PIN] |= PORT_PCR_IRQC(0b1011); // Set up the IRQC to interrupt on eit

	//setup switch 3
	PORTC->PCR[SWITCH_3_PIN] &= ~PORT_PCR_MUX(0b111); // Clear PCR Mux bits for PTC16
	PORTC->PCR[SWITCH_3_PIN] |= PORT_PCR_MUX(0b001); // Set up PTC16 as GPIO
	PTC->PDDR &= ~GPIO_PDDR_PDD(1 << SWITCH_3_PIN); // make it input
	PORTC->PCR[SWITCH_3_PIN] |= PORT_PCR_PE(1); // Turn on the pull enable
	PORTC->PCR[SWITCH_3_PIN] |= PORT_PCR_PS(1); // Enable the pullup resistor
	PORTC->PCR[SWITCH_3_PIN] &= ~PORT_PCR_IRQC(0b1111); // Clear IRQC bits for PTC16
	PORTC->PCR[SWITCH_3_PIN] |= PORT_PCR_IRQC(0b1011); // Set up the IRQC to interrupt on either edge



}

/* Polls the color button */
void color_button_polling() {
	if((PORTC->PCR[SWITCH_1_PIN] & PORT_PCR_ISF(1)) != 0) {	//If the color button is pressed
		PORTC->PCR[SWITCH_1_PIN] |= PORT_PCR_ISF(1);  		//Clear the interrupt status flag by writing 1 to it
		colorPaint+=1;										//Increases the color (changes the color)
		if(colorPaint > 8){									//Resets the color to 1.
			colorPaint = 1;
		}
	}

}

/* Polls the draw button */
void draw_button_polling(){
	if((PORTA->PCR[SWITCH_2_PIN] & PORT_PCR_ISF(1)) != 0) {	//If the draw button is pressed
		drawCheck = 1 - drawCheck;							//This code switches draw enable, whether the lines are drawn or not
		if(drawCheck == 0){
			drawEnabled = 1 - drawEnabled;
		}
		PORTA->PCR[SWITCH_2_PIN] |= PORT_PCR_ISF(1);  		//Clears the interrupt status flag by writing 1 to it
	}
}

/* Polls the clear button */
void clear_button_polling(){
	if((PORTC->PCR[SWITCH_3_PIN] & PORT_PCR_ISF(1)) != 0) {	//If the clear button is pressed
		clearButton = 1;									//Sets clear to true
		PORTC->PCR[SWITCH_3_PIN] |= PORT_PCR_ISF(1);  		//Clears the interrupt status flag by writing 1 to it
	}
}

/* Converts the color number into a string to send to python through UART */
const char* stringOutput(int col) {
  if(col == 1)
	  return "red";
  else if(col == 2)
	  return "orange";
  else if(col == 3)
	  return "yellow";
  else if(col == 4)
	  return "green";
  else if(col == 5)
	  return "cyan";
  else if(col == 6)
	  return "blue";
  else if(col == 7)
	  return "purple";
  else
	  return "white";
}

int main(void) {
	buttonSetup();	//Setup the buttons
	char text[50];	//Initialize text as a char array to represent a string
	init_uart();	//Initializes UART
	setupADC();		//Sets up the ADC
	int analogX;
	int analogY;
	while(1){
		analogX = getADC1Value();	//reads the joystick's position in X direction
		analogY = getADC2Value();	//reads the joystick's position in Y direction
		sprintf(text, "%d, %d, %s, %d, %d", analogX, analogY, stringOutput(colorPaint), drawEnabled, clearButton);	//Creates large string of information to send to python
		clearButton = 0;	//Sets clear back to false
		short_delay();		//Short delay between UART communication
		uart_puts(text);	//Send the string to python
		uart_puts("\n");
		color_button_polling();	//Polls for the color button
		draw_button_polling();	//Polls for the draw button
		clear_button_polling();	//Polls for the clear button
	}
	return 0 ;
}







