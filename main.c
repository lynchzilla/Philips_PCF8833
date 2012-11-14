//  *****************************************************************************
//   						main.c
// 
//		Nokia 6610 LCD demonstration program for Olimex SAM7-EX256 Evaluation Board
//
//		Performs a series of tests of the LCD driver.
//
//		When tests are complete, blinks LED4 (pin PA3) with an endless loop
//		PA3 is pin 1 on the EXT 20-pin connector (3.3v is pin 18)
//
//		The Olimex SAM7-EX256 board has no programmable LEDs. 
//		Added a simple test LED from Radio Shack as shown below (can be attached to the 20-pin EXT connector.)
//
//		    3.3 volts  |---------|           anode  |----|                PA3
//		    EXT O------| 470 ohm |------------------|LED |--------------O EXT
//          Pin 18     |---------|                  |----| cathode        pin 1
//                                              Radio Shack Red LED
//                                              276-026 T-1 size (anode is the longer wire)
//
//          LED current:  I = E/R  = 3.3/470  =  .007 amps = 7 ma
//			Note: most PIO pins can drive 8 ma on the AT91SAM7X256, so we're OK
//
//
//		
//		Author:  James P Lynch  July 7, 2007
//  *****************************************************************************
 
//  *******************************************************
//                Header Files
//  *******************************************************
#include "AT91SAM7X256.h"
#include "lcd.h"
#include "board.h"


//  *******************************************************
//                External References
//  *******************************************************
extern	void LowLevelInit(void);




int	main (void) {
	
	unsigned long	j;
	unsigned long	k;
	unsigned long	col;
	unsigned long	row;
	unsigned int	IdleCount = 0;
	int				TempColor[11] = {WHITE, BLACK, RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, BROWN, ORANGE, PINK};
	char			*TempChar[11] = {"White", "Black", "Red", "Green", "Blue", "Cyan", "Magenta", "Yellow", "Brown", "Orange", "Pink"};		
		
	// Initialize the Atmel AT91SAM7S256 (watchdog, PLL clock, default interrupts, etc.)
	LowLevelInit();

	// Set up the LED (PA3)
	volatile AT91PS_PIO	pPIO = AT91C_BASE_PIOA;			// pointer to PIO data structure
	pPIO->PIO_PER = LED_MASK;							// PIO Enable Register - allow PIO to control pin PP3
	pPIO->PIO_OER = LED_MASK;							// PIO Output Enable Register - sets pin P3 to outputs
	pPIO->PIO_SODR = LED_MASK;							// PIO Set Output Data Register - turns off the LED
	
	// Initialize SPI interface to LCD
	InitSpi();
	
	// Init LCD
	InitLcd();

	// clear the screen
	LCDClearScreen();
	
	// ***************************************************************
	// *  color test - show boxes of different colors                *
	// ***************************************************************
	
	for (j = 0; j < 11; j++) {
		
		// draw a filled box
		LCDSetRect(120, 10, 25, 120, FILL, TempColor[j]);
		
		// label the color
		LCDPutStr("        ", 5, 40, LARGE, BLACK, BLACK);
		LCDPutStr(TempChar[j], 5, 40, LARGE, YELLOW, BLACK);
		
		// wait a bit
		Delay(2000000);
	}
	
	
	
	// ***************************************************************
	// *  character and line tests - draw lines, strings, etc.       *
	// ***************************************************************
	
	// clear the screen
	LCDClearScreen();
	
	// set a few pixels
	LCDSetPixel(30, 120, RED);
	LCDSetPixel(34, 120, GREEN);
	LCDSetPixel(38, 120, BLUE);
	LCDSetPixel(40, 120, WHITE);
	
	// draw some characters
	LCDPutChar('E', 10, 10, SMALL, WHITE, BLACK);
	
	// draw a string
	LCDPutStr("Hello World", 60, 5, SMALL, WHITE, BLACK);
	LCDPutStr("Hello World", 40, 5, MEDIUM, ORANGE, BLACK);
	LCDPutStr("Hello World", 20, 5, LARGE, PINK, BLACK);
	
	// draw a filled box
	LCDSetRect(120, 60, 80, 80, FILL, BROWN);
	
	// draw a empty box
	LCDSetRect(120, 85, 80, 105, NOFILL, CYAN);

	// draw some lines
	LCDSetLine(120, 10, 120, 50, YELLOW);
	LCDSetLine(120, 50, 80, 50, YELLOW);
	LCDSetLine(80, 50, 80, 10, YELLOW);
	LCDSetLine(80, 10, 120, 10, YELLOW);
	
	LCDSetLine(120, 85, 80, 105, YELLOW);
	LCDSetLine(80, 85, 120, 105, YELLOW);
	
	// draw a circle
	LCDSetCircle(65, 100, 10, RED);
	
	
	Delay(8000000);
	
	// ***************************************************************
	// *  bmp display test - display the Olimex photograph           *
	// ***************************************************************
	
	LCDClearScreen();
	
	LCDWrite130x130bmp();
	
	LCDPutStr("This guy is nuts", 115, 2, LARGE, BLACK, CYAN);

	// draw a filled box
	LCDSetRect(90, 70, 75, 120, FILL, YELLOW);
	
	LCDPutStr("HELP!", 80, 80, SMALL, BLACK, YELLOW);



	// *****************************
	// *  endless blink loop       *
	// *****************************
	while (1) {
		if  ((pPIO->PIO_ODSR & LED4) == LED4)			// read previous state of LED4
			pPIO->PIO_CODR = LED4;						// turn LED4 (DS1) on	
		else
			pPIO->PIO_SODR = LED4;						// turn LED4 (DS1) off
		
		for (j = 1000000; j != 0; j-- );				// wait 1 second 1000000
	
		IdleCount++;									// count # of times through the idle loop
	}
}





