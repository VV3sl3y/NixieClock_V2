#include <Arduino.h>

#include "DriveNixies.h"
#include "Pinout.h"
#include "Settings.h"

HEADER_BOARD UsedHeaderBoard;

//in this function there are all possible pin combos defined, but in reality we'll only use 0 - 9
void SetNixieDriverVal(int i)
{
  
	int portValue = 0;
	switch (UsedHeaderBoard)
	{
#pragma region IN12 PORTB values
	case IN12:
		switch (i)
		{
		case 0:
			portValue = 0;
			break;
		case 1:
			portValue = (N_Driver_A_ADDR | N_Driver_D_ADDR);
			break;
		case 2:
			portValue = (N_Driver_D_ADDR);
			break;
		case 3:
			portValue = (N_Driver_A_ADDR | N_Driver_B_ADDR | N_Driver_C_ADDR);
			break;
		case 4:
			portValue = (N_Driver_B_ADDR | N_Driver_C_ADDR);
			break;
		case 5:
			portValue = (N_Driver_A_ADDR | N_Driver_C_ADDR);
			break;
		case 6:
			portValue = (N_Driver_C_ADDR);
			break;
		case 7:
			portValue = (N_Driver_A_ADDR | N_Driver_B_ADDR);
			break;
		case 8:
			portValue = (N_Driver_B_ADDR);
			break;
		case 9:
			portValue = (N_Driver_A_ADDR);
			break;
		case 10:
			portValue = (N_Driver_B_ADDR | N_Driver_D_ADDR);
			break;
		case 11:
			portValue = (N_Driver_A_ADDR | N_Driver_B_ADDR | N_Driver_D_ADDR);
			break;
		case 12:
			portValue = (N_Driver_C_ADDR | N_Driver_D_ADDR);
			break;
		case 13:
			portValue = (N_Driver_A_ADDR | N_Driver_C_ADDR | N_Driver_D_ADDR);
			break;
		case 14:
			portValue = (N_Driver_B_ADDR | N_Driver_C_ADDR | N_Driver_D_ADDR);
			break;
		case 15:
			portValue = (N_Driver_A_ADDR | N_Driver_B_ADDR | N_Driver_C_ADDR | N_Driver_D_ADDR);
			break;
		default:
			portValue = 0;
			break;
		}
#pragma endregion
    
#pragma region IN14 PORTB values
	case IN14:
		switch (i)
		{
		case 0:
			portValue = 0;
			break;
		case 1:
			portValue = (N_Driver_A_ADDR);
			break;
		case 2:
			portValue = (N_Driver_B_ADDR);
			break;
		case 3:
			portValue = (N_Driver_A_ADDR | N_Driver_B_ADDR);
			break;
		case 4:
			portValue = (N_Driver_C_ADDR);
			break;
		case 5:
			portValue = (N_Driver_A_ADDR | N_Driver_C_ADDR);
			break;
		case 6:
			portValue = (N_Driver_B_ADDR | N_Driver_C_ADDR);
			break;
		case 7:
			portValue = (N_Driver_A_ADDR | N_Driver_B_ADDR | N_Driver_C_ADDR);
			break;
		case 8:
			portValue = (N_Driver_D_ADDR);
			break;
		case 9:
			portValue = (N_Driver_A_ADDR | N_Driver_D_ADDR);
			break;
		case 10:
			portValue = (N_Driver_B_ADDR | N_Driver_D_ADDR);
			break;
		case 11:
			portValue = (N_Driver_A_ADDR | N_Driver_B_ADDR | N_Driver_D_ADDR);
			break;
		case 12:
			portValue = (N_Driver_C_ADDR | N_Driver_D_ADDR);
			break;
		case 13:
			portValue = (N_Driver_A_ADDR | N_Driver_C_ADDR | N_Driver_D_ADDR);
			break;
		case 14:
			portValue = (N_Driver_B_ADDR | N_Driver_C_ADDR | N_Driver_D_ADDR);
			break;
		case 15:
			portValue = (N_Driver_A_ADDR | N_Driver_B_ADDR | N_Driver_C_ADDR | N_Driver_D_ADDR);
			break;
		default:
			portValue = 0;
			break;
		}
#pragma endregion

		GPIOB_BASE->ODR &= 0b0000111111111111;  // clear previous values
		GPIOB_BASE->ODR |= portValue;           // set new values
		break;
	}
}

void SetNixieTube(int n) {
	switch (UsedHeaderBoard)
	{
#pragma region IN12 SB
	case IN12:
		switch (n) {
		case HOURS_MSB:
			GPIOB_BASE->ODR |= (N_Second1_ADDR);
			break;
		case HOURS_LSB:
			GPIOB_BASE->ODR |= (N_Second2_ADDR);
			break;
		case MINUTES_MSB:
			GPIOB_BASE->ODR |= (N_Minute1_ADDR);
			break;
		case MINUTES_LSB:
			GPIOB_BASE->ODR |= (N_Minute2_ADDR);
			break;
		case SECONDS_MSB:
			GPIOB_BASE->ODR |= (N_Hour1_ADDR);
			break;
		case SECONDS_LSB:
			GPIOB_BASE->ODR |= (N_Hour2_ADDR);
			break;
		default:
			break;
		}
		break;
#pragma endregion
    
#pragma region IN14 SB
	case IN14:
		switch (n) {
		case HOURS_MSB:
			GPIOB_BASE->ODR |= (N_Hour2_ADDR);
			break;
		case HOURS_LSB:
			GPIOB_BASE->ODR |= (N_Hour1_ADDR);
			break;
		case MINUTES_MSB:
			GPIOB_BASE->ODR |= (N_Minute2_ADDR);
			break;
		case MINUTES_LSB:
			GPIOB_BASE->ODR |= (N_Minute1_ADDR);
			break;
		case SECONDS_MSB:
			GPIOB_BASE->ODR |= (N_Second2_ADDR);
			break;
		case SECONDS_LSB:
			GPIOB_BASE->ODR |= (N_Second1_ADDR);
			break;
		default:
			break;
		}
		break;
#pragma endregion
	}
}

void testNixieTube(int n) {
	NixiesOff();
	SetNixieTube(n);
}

void NixiesOff() {
	GPIOB_BASE->ODR &= 0b1111111111000000;
}

void SwitchDot(int i) {
	switch (i) {
	case DOT_SWITCH:
		GPIOB_BASE->ODR ^= (N_Dot_ADDR);
		//digitalWrite(N_Dot, !digitalRead(N_Dot));
		break;
	case DOT_ON:
		GPIOB_BASE->ODR |= (N_Dot_ADDR);
		//digitalWrite(N_Dot, HIGH);
		break;
	case DOT_OFF:
		GPIOB_BASE->ODR &= ~(N_Dot_ADDR);
		//digitalWrite(N_Dot, LOW);
		break;
	}
}
