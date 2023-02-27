#pragma once

#include <Arduino.h>

bool initPins();

//*********************STM nixie pinout************************//
#define AlarmOff PA0 //Alarm off btn

//#define LED_R PA1 //placeholder
//#define LED_G PA2 //placeholder
//#define LED_B PA3 //placeholder

#define LED_Data PA4 //WS2811 data pin

#define Flash_ESP PA5 //Optional programming over io
#define RST_ESP PA6 //reset esp

#define Buzzer PA7 //Buzzer pin

#define HVON PA8 //optional switch for the HV: selectable via solder pad

//#define ProgRX PA9 //reserved for programming
//#define ProgRX PA10 //reserved for programming

//#define USB_DataP PA11 //placeholder
//#define USB_DataM PA12 //placeholder

//#define RsvPA13 PA13 //placeholder
//#define RsvPA14 PA14 //placeholder
//#define RsvPA15 PA15 //placeholder

#define N_Hour2 PB0 //Nixie Hour 2 (first)
#define N_Hour1 PB1 //Nixie Hour 1 (second)
#define N_Minute2 PB2 //Nixie Minute 2 (third)
#define N_Minute1 PB3 //Nixie Minute 1 (fourth)
#define N_Second2 PB4 //Nixie Second 2 (fifth)
#define N_Second1 PB5 //Nixie Second 1 (sixth)
#define N_Dot PB8 //Nixie Dots

#define N_Driver_A_ADDR (1 << 15)
#define N_Driver_B_ADDR (1 << 13)
#define N_Driver_C_ADDR (1 << 12)
#define N_Driver_D_ADDR (1 << 14)
	
#define N_Driver_C PB12 //Nixie Driver C signal
#define N_Driver_B PB13 //Nixie Driver B signal
#define N_Driver_D PB14 //Nixie Driver D signal
#define N_Driver_A PB15 //Nixie Driver A signal

//note to self PCB V2.0 has false pinout of RX TX on the MCU, fixed in V2.1
#define RX_ESP PB6 //RX of the ESP (bluepill TX, SCL)
#define TX_ESP PB7 //TX of the ESP (bluepill RX, SDA)

#define LED_RTC PC13 //RTC LED

//#define RsvPB9 PB9 //placeholder
//#define SCL PB10 //placeholder
//#define SDA PB11 //placeholder

//#define PC14 - RTC crystal
//#define PC15 - RTC Crystal

//#define PD0 - OSC crystal
//#define PD1 - OSC crystal
