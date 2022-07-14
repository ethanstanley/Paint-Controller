#include <stdio.h>
#include <MKL46Z4.h>

void setupADC(void) {
	// Enable clock gate for ADC0
	SIM->SCGC6 |= (1 << 27);

	// Setup ADC Clock ( < 4 MHz)
	ADC0->CFG1 = 0;  // Default everything.

	// Analog Calibrate
	ADC0->SC3 = 0x07; // Enable Maximum Hardware Averaging
	ADC0->SC3 |= 0x80; // Start Calibration

	// Wait for Calibration to Complete (either COCO or CALF)
	while(!(ADC0->SC1[0] & 0x80)){	}

	ADC0->SC3 = 0; // Turn off Hardware Averaging

	return;
}

/* Returns the joystick's position in the horizontal direction */
int getADC1Value(void) {
	ADC0->SC1[0] = 0x0; //Prepare ADC for data, selects the first channel
	while(ADC0->SC2 & ADC_SC2_ADACT_MASK);
	while(!(ADC0->SC1[0] & 0x80)){	} //Collecting data
	return ADC0->R[0]; //Returns the result
}

/* Returns the joystick's position in the vertical direction */
int getADC2Value(void) {
	ADC0->SC1[0] = 1;	//Prepare ADC for data, selects the second channel
	while(ADC0->SC2 & ADC_SC2_ADACT_MASK);
	while(!(ADC0->SC1[0] & 0x80)){	} //Collecting data
	return ADC0->R[0]; //Returns the result
}

