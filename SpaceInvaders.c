#include "..//tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "Random.h"
#include "TExaS.h"
#include "time.h"
#include "stdlib.h"
#include <stdio.h>

// Function prototypes for temperature sensor
void TemperatureSensor_Init(void);
void GetTemperature(void);
int getPotentiometerReading(void) ; 
double volt  ; 
double Temp ; 
// Function to adjust game speed based on temperature
void AdjustGameSpeed(void);
void lol(void) ; 
// Variable to store the current temperature
volatile double currentTemperature = 0;
volatile unsigned int range = 0 ; 
volatile unsigned int tempRange = 0 ; 
// Timer2_Init period value
volatile int currentSpeed = 0 ; 
unsigned long gamePeriod = 1000000; // Default value for normal speed

void ADC_Init(void);
void PortF_Init(void);
void PortB_Init(void);
void PortE_Init(void); // Initialize Port E
void ADC0_Init(void);  // Initialize ADC0
void GPIOPortF_Handler(void);  // Interrupt handler for Port F
void DisableInterrupts(void);  // Disable interrupts
void EnableInterrupts(void);   // Enable interrupts
void Timer2_Init(unsigned long period);
void Delay100ms(unsigned long count);  // time delay in 0.1 seconds

unsigned long TimerCount;
unsigned long Semaphore;


// *************************** Images ***************************
// enemy ship that starts at the top of the screen (arms/mouth closed)
// width=16 x height=10

const unsigned char car[] ={
 0x42, 0x4D, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF,
 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xF0, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x0F, 0xFF, 0xF0, 0x00, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xF0,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x0F, 0xFF, 0xF0, 0x00, 0xFF, 0xFF, 0xF0, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF,

};

const unsigned char enemy[] ={
 0x42, 0x4D, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF,
 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF,
 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF,

};



// *************************** Capture image dimensions out of BMP**********

#define carW        ((unsigned char)car[18])
#define carH        ((unsigned char)car[22])
#define enemyW        ((unsigned char)enemy[18])
#define enemyH        ((unsigned char)enemy[22])


// *************************** Variables ************************ //
volatile unsigned int gameOver = 0 ; 
int playerx = 0, playery, playerH = 10;  // Player position (x, y) and height
int enemyY1, enemyx2w, enemyMove, enemyMove1;  // Enemy position and movement
int screen_width = 84, screenH = 47;  // Screen dimensions
int tracks[3] = {28, 11, 45};         // Track x positions
int f1 = 0, f2 = 0, start = 0;        // Flags
int score;
int result;
char buffer[20];
// *************************** Main Function ************************ //

int main(void) {
  PortB_Init();
  PortF_Init();
   PortE_Init(); // Initialize Port E

  playery = 28; // Mid-screen y = 28
  enemyMove = 84 - enemyW;
  enemyMove1 = 84 - enemyW;
  enemyY1 = rand() % 3;
  score = 0;

  TExaS_Init(SSI0_Real_Nokia5110_Scope); // Set system clock to 80 MHz
  Random_Init(1);
  Nokia5110_Init();
  Nokia5110_ClearBuffer();
  Nokia5110_DisplayBuffer(); // Draw buffer
	ADC0_Init();  // Initialize ADC0 
	ADC_Init();
	
	GetTemperature();

  EnableInterrupts(); // Enable global interrupts

  // Welcome Screen
  Nokia5110_Clear();
  Nokia5110_SetCursor(3, 0);
  Nokia5110_OutString("A M Y");
	
			Nokia5110_SetCursor(1, 1);
		sprintf(buffer, " %.2f", currentTemperature/10);
		Nokia5110_OutString(buffer);
  
	Nokia5110_SetCursor(1, 2);
  Nokia5110_OutString("Racing Car");
  Nokia5110_SetCursor(0, 4);
  Nokia5110_OutString("For Playing");
  Nokia5110_SetCursor(1, 5);
  Nokia5110_OutString("Press SW1");

  GPIO_PORTB_DATA_R &= ~(1 << 5);  // Turn off PB4 (Game Start LED)
  GPIO_PORTB_DATA_R |= (1 << 4);   // Turn on PB5 (Game Over LED)

  while (start == 0) {
    // Wait for SW1 to be pressed (handled by interrupt)
  }

  // Timer2_Init(gamePeriod); // Initialize Timer2 with the default period

  // Game Loop
  while (1) {
	
    if (gameOver) {
      break;
    }
    AdjustGameSpeed(); // Adjust game speed dynamically 
  }
  

  // ***************** Game Over Screen **************** //
  GPIO_PORTB_DATA_R &= ~(1 << 4);  // Turn off PB4 (Game Start LED)
  GPIO_PORTB_DATA_R |= (1 << 5);   // Turn on PB5 (Game Over LED)

  Nokia5110_Clear();
	Nokia5110_ClearBuffer() ;
	Nokia5110_SetCursor(1, 0);
	sprintf(buffer, " %.2f", currentTemperature/10);
	Nokia5110_OutString(buffer);
  Nokia5110_SetCursor(1, 1);
  Nokia5110_OutString("GAME OVER");
  Nokia5110_SetCursor(1, 2);
  Nokia5110_OutString("Nice try");
  Nokia5110_SetCursor(1, 4);
  Nokia5110_OutString("Your score is");
  Nokia5110_SetCursor(3, 5);
  Nokia5110_OutUDec(score);
 
}
unsigned temp = 0 ;
void AdjustGameSpeed(void) {
	if(temp != gamePeriod) {
  Timer2_Init(gamePeriod); // Reinitialize Timer2 with new period
	temp = gamePeriod ;
	}
  currentSpeed = getPotentiometerReading(); // Read the temperature
  // Adjust delay based on temperature
  if (currentSpeed > 30) { // High temperature
    gamePeriod = 2000000; // Increase delay (slower speed)
  } else if (currentSpeed > 20) { // Normal temperature
    gamePeriod = 1000000; // Normal speed
  } else { // Low temperature
    gamePeriod = 500000; // Decrease delay (faster speed)
  }

}

// Temperature sensor initialization
void PortE_Init(void) {
    volatile unsigned long delay;
    SYSCTL_RCGCGPIO_R |= 1 << 4;  // Enable clock for Port E
    delay = SYSCTL_RCGCGPIO_R;    // Allow time for the clock to stabilize

    GPIO_PORTE_AFSEL_R |= 1 << 3; // Enable alternate function on PE3
    GPIO_PORTE_DEN_R &= ~(1 << 3); // Disable digital I/O on PE3
    GPIO_PORTE_AMSEL_R |= 1 << 3; // Enable analog function on PE3
}

void ADC0_Init(void) {
	    volatile unsigned long delay;

	SYSCTL_RCGC0_R |=(1 << 16); //ADC0 clock
	while ((SYSCTL_RCGC0_R &0x00010000)== 0); //ADC0 clock to settle
	SYSCTL_RCGC0_R &=~(1<<8 | 1<< 9); 
    delay = SYSCTL_RCGCADC_R;     // Allow time for the clock to stabilize

    ADC0_ACTSS_R &= ~ ( 1 << 3 );           // Disable sample sequencer 3
    ADC0_EMUX_R &= ~( 1 << 12 | 1 << 13 | 1 << 14 | 1 << 15);       // continous sampling
    ADC0_SSMUX3_R = 0;            // Get input from channel 0
    ADC0_SSCTL3_R = ( 1 << 1 | 1 << 2);          // Set flag and end after one sample
    ADC0_ACTSS_R |= ( 1 << 3);            // Enable sample sequencer 3
}

// Read temperature sensor value
void GetTemperature(void) {
  ADC0_PSSI_R = 8; // Start conversion
  while ((ADC0_RIS_R & 8) == 0); // Wait for conversion to complete
  result = ADC0_SSFIFO3_R; // Read ADC result
  ADC0_ISC_R = 8; // Clear completion flag
   volt = ((3.3/4096)*result);
	 Temp = 147.5 - (75.0 * volt);
	currentTemperature =  Temp ; // Convert to temperature
}


int getPotentiometerReading(void) {
  ADC1_PSSI_R = 8; // Start conversion
  while ((ADC1_RIS_R & 8) == 0); // Wait for conversion to complete
  result = ADC1_SSFIFO3_R; // Read ADC result
  ADC1_ISC_R = 8; // Clear completion flag
   volt = ((3.3/4096)*result);
	 Temp = ((2.7-volt)*75)-55;
	return Temp ; // Convert to temperature
}

void ADC_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= ( 1 << 3) ;         // 1) activate clock for Port D
//  SYSCTL_RCGCGPIO_R |= 0x08;      // 1) activate clock for Port D
  delay = SYSCTL_RCGC2_R;         //    allow time for clock to stabilize
  delay = SYSCTL_RCGC2_R;
  GPIO_PORTD_DIR_R &= ~(1 << 3);      // 2) make PD3 input
  GPIO_PORTD_AFSEL_R |= (1 << 3);     // 3) enable alternate function on PD3
  GPIO_PORTD_DEN_R &= ~ (1 << 3) ;      // 4) disable digital I/O on PD3
  GPIO_PORTD_AMSEL_R |= (1 << 3);     // 5) enable analog functionality on PD3
  SYSCTL_RCGC0_R |= (1<<17);   // 6) activate ADC1 (legacy code)
//  SYSCTL_RCGCADC_R |= 0x02;
  for(delay = 0; delay<20; delay++){};  // allow time for clock to stabilize
//  SYSCTL_RCGC0_R &= ~0x00000C00;  // 7) configure for 125K (legacy code)
  ADC1_PC_R = ( 1 << 0);               // 7) 125K rate
  ADC1_SSPRI_R = ( 1 << 0 | 1 << 1 | 1 << 5 | 1 << 8 );           // 8) Sequencer 3 is highest priority
  ADC1_ACTSS_R = 0;          // 9) disable sample sequencer 3
  ADC1_EMUX_R |= ( 1 << 12 | 1 << 13 | 1 << 14 | 1 << 15);          // 10) seq3 is always/continuous trigger
  ADC1_SAC_R = ( 1 << 0 | 1 << 1);              //   8-point average 125kHz/8 = 15,625 Hz
  ADC1_SSMUX3_R = ( 1 << 2 );              // 11) set channel 4
  ADC1_SSCTL3_R = (1 <<1 | 1 << 2);         // 12) no TS0 D0, yes IE0 END0
  ADC1_IM_R = 0;             // 13) disable SS3 interrupts
  ADC1_ACTSS_R = (1<<3);          // 14) enable sample sequencer 3
}


// *************************** PortF Initialization ************************ //

void PortF_Init(void) {volatile unsigned long delay = SYSCTL_RCGC2_R;
  SYSCTL_RCGC2_R |= (1 << 5);  // Enable clock for Port F
  delay = SYSCTL_RCGC2_R; 

  GPIO_PORTF_LOCK_R = 0x4C4F434B;       // Unlock Port F
  GPIO_PORTF_CR_R |= (1<<0 |1 << 1 | 1 << 2 | 1 << 3 | 1 << 4) ;              // Allow changes to PF4
  GPIO_PORTF_AMSEL_R = 0;            // Disable analog functionality
  GPIO_PORTF_PCTL_R = 0;       // GPIO clear PCTL
  GPIO_PORTF_DIR_R &= ~(1 << 4);  // PF4 as inputs
  GPIO_PORTF_AFSEL_R = 0;            // Disable alternate functions
  GPIO_PORTF_PUR_R |= (1 << 4);  // Enable pull-up resistors
  GPIO_PORTF_DEN_R |= (1<<0 |1 << 1 | 1 << 2 | 1 << 3 | 1 << 4) ;             // Enable digital pins PF4

  GPIO_PORTF_IS_R &= ~(1 << 4);   // Edge-sensitive
  GPIO_PORTF_IBE_R &= ~(1 << 4); // Not both edges
  GPIO_PORTF_IEV_R &= ~(1 << 4); // Falling edge
  GPIO_PORTF_ICR_R |= (1 << 4);    // Clear any prior interrupt
  GPIO_PORTF_IM_R |= (1 << 4);     // Enable interrupts

  NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF00FFFF) | 0x00A00000;  // Priority 5
  NVIC_EN0_R |= (1 << 30);  // Enable interrupt 30 in NVIC
}

// *************************** PortF ISR ************************ //

void GPIOPortF_Handler(void) {
  if (GPIO_PORTF_RIS_R & (1 << 4)) {  // SW1 (PF4) pressed
    GPIO_PORTF_ICR_R |= (1 << 4);    // Acknowledge interrupt
    if (!start) {
      start = 1;  // Start game
    } 
	}
	
}


void GPIOPortB_Handler(void) {
  if (GPIO_PORTB_RIS_R & (0x01)) {  // SW1 (PB0) pressed
    GPIO_PORTB_ICR_R |= (0x01);    // Acknowledge interrupt
    if (start) {
      f1 = 1;  // move car up
    }
  }
  if (GPIO_PORTB_RIS_R & (0x02)) {  // SW2 (PB1) pressed
    GPIO_PORTB_ICR_R |= (0x02);    // Acknowledge interrupt
		if(start)
    f2 = 1;  // Move car down
  }
	
}

// *************************** Other Functions ************************ //

void PortB_Init(void) {  volatile unsigned long delay = SYSCTL_RCGC2_R;

  SYSCTL_RCGC2_R |= (1 << 1);  // Enable clock for Port B
	delay = SYSCTL_RCGC2_R; 
   GPIO_PORTB_DIR_R |= (1 <<4 ) | (1 << 5); // Set PB4 and PB5 as output
	GPIO_PORTB_DIR_R &= ~(1<<1 | 1<<0);			// Setting PB0, PB1 as inputs
  GPIO_PORTB_AFSEL_R &= ~((1 << 4) | (1 << 5) | (1<<1) | (1<<0)); // Disable alternate functions for PB4, PB5, PB0, PB1
  GPIO_PORTB_DEN_R |= (1 << 4) | (1 << 5) | (1<<1) | (1<<0); // Enable digital I/O for PB4, PB5, PB0, PB1
  GPIO_PORTB_PCTL_R &= ~0x000000FF;  // Configure PB4 and PB5 as GPIO
  GPIO_PORTB_AMSEL_R &= ~((1 << 4) | (1 << 5) | (1<<1) | (1<<0)); // Disable analog functionality for PB4, PB5, PB0, PB1
	
	GPIO_PORTB_PUR_R |= (1 << 1) | (1 << 0);  // Enable pull-up resistors for PB0, PB1
	
	
	GPIO_PORTB_IS_R &= ~((1 << 1) | (1 << 0));  // Edge-sensitive
  GPIO_PORTB_IBE_R &= ~((1 << 1) | (1 << 0)); // Not both edges
  GPIO_PORTB_IEV_R &= ~((1 << 1) | (1 << 0)); // Falling edge
  GPIO_PORTB_ICR_R |= (1 << 1) | (1 << 0);    // Clear any prior interrupt
  GPIO_PORTB_IM_R |= (1 << 1) | (1 << 0);     // Enable interrupts

  NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF00FFFF) | 0x00A00000;  // Priority 5
  NVIC_EN0_R |= (1 << 1);           // Enable interrupt in NVIC for Port B (Interrupt 1)

}

void Timer2_Init(unsigned long period){ 
  unsigned long volatile delay;
 // configuration of the RCGCTIMER ( pin 2  block (2) ) 
  SYSCTL_RCGCTIMER_R |= (1 << 2);   // 0) activate timer2
 // delaying to ensure that the rcgc clock of the timer is activated successfully
 // it takes time about 4 cycles ...
  delay = SYSCTL_RCGCTIMER_R;
 // TimerCount = 0;
  //Semaphore = 0;
 // ensuring that the TnEN bit are disabled first .
  TIMER2_CTL_R &= ~(1 << 0);    // 1) disable timer2A during setup
 // here to decide whether to use all block or only one timer 
  TIMER2_CFG_R &= ~((1 << 0) | (1 << 1));    // 2) configure for 32-bit mode ( all block is used)
 // here  we dedict the mode of certain timer 
 // timer a is used as it is configured here for the whole block 
 TIMER2_TAMR_R |= (1 << 1 );   // 3) configure for periodic mode, default down-count settings
  TIMER2_TAILR_R = period-1;    // 4) reload value
  TIMER2_TAPR_R = 0;            // 5) bus clock resolution
  TIMER2_ICR_R |= 1 << 0 ;    // 6) clear timer2A timeout flag
  TIMER2_IMR_R |= 1 << 0;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN0_R = 1<<23;           // 9) enable IRQ 23 in NVIC
  TIMER2_CTL_R |= 1 << 0;    // 10) enable timer2A
}

void Timer2A_Handler(void){ 
// contains what in the while loop 
	TIMER2_ICR_R |= 1 << 0 ; // acknowledge the timer2 timeout 
		Nokia5110_ClearBuffer();
		GetTemperature() ;
    // ***************** Car Movement **************** //
    if (f1 && (playery > 11)) {  // Move up
      playery -= 17;
      f1 = 0;
    }
    if (f2 && (playery < 45)) {  // Move down
      playery += 17;
      f2 = 0;
    }
    Nokia5110_PrintBMP(playerx, playery, car, 0);

    // ***************** Enemy Movement **************** //
    if (enemyMove != 0) {
      Nokia5110_PrintBMP(enemyMove, tracks[enemyY1], enemy, 0);
      enemyMove--;
    }

    if (enemyMove == 0) {
      if (enemyMove1 == 0) {
        score++;  // Increment score for passed enemies
        enemyY1 = rand() % 3;
        enemyMove1 = 84 - enemyW;
      }
      Nokia5110_PrintBMP(enemyMove1, tracks[enemyY1], enemy, 0);
    }

    if (enemyMove1 <= playerx + carW - 2 && tracks[enemyY1] == playery) {
       gameOver = 1 ;  // Collision detected
    }
    enemyMove1--;

    // ***************** Screen Display **************** //
    Nokia5110_DisplayBuffer();

}

void Delay100ms(unsigned long count) {
  unsigned long volatile time;
  while (count > 0) {
    time = 727240 / 80;  // 0.1sec at 80 MHz
    while (time) {
      time--;
    }
    count--;
  }
}
