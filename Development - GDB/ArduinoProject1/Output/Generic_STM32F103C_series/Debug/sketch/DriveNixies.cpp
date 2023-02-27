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
#pragma region Old code
		/*case IN12:
			switch (i)
			{
			case 0:
				digitalWrite(N_Driver_A, LOW);
				digitalWrite(N_Driver_B, LOW);
				digitalWrite(N_Driver_C, LOW);
				digitalWrite(N_Driver_D, LOW);
				break;

		case 1:
			digitalWrite(N_Driver_A, HIGH);
			digitalWrite(N_Driver_B, LOW);
			digitalWrite(N_Driver_C, LOW);
			digitalWrite(N_Driver_D, LOW);
			break;
    
		case 2:
			digitalWrite(N_Driver_A, LOW);
			digitalWrite(N_Driver_B, HIGH);
			digitalWrite(N_Driver_C, LOW);
			digitalWrite(N_Driver_D, LOW);
			break;

		case 3:
			digitalWrite(N_Driver_A, HIGH);
			digitalWrite(N_Driver_B, HIGH);
			digitalWrite(N_Driver_C, LOW);
			digitalWrite(N_Driver_D, LOW);
			break;

		case 4:
			digitalWrite(N_Driver_A, LOW);
			digitalWrite(N_Driver_B, LOW);
			digitalWrite(N_Driver_C, HIGH);
			digitalWrite(N_Driver_D, LOW);
			break;

		case 5:
			digitalWrite(N_Driver_A, HIGH);
			digitalWrite(N_Driver_B, LOW);
			digitalWrite(N_Driver_C, HIGH);
			digitalWrite(N_Driver_D, LOW);
			break;

		case 6:
			digitalWrite(N_Driver_A, LOW);
			digitalWrite(N_Driver_B, HIGH);
			digitalWrite(N_Driver_C, HIGH);
			digitalWrite(N_Driver_D, LOW);
			break;
      
		case 7:
			digitalWrite(N_Driver_A, HIGH);
			digitalWrite(N_Driver_B, HIGH);
			digitalWrite(N_Driver_C, HIGH);
			digitalWrite(N_Driver_D, LOW);
			break;

		case 8:
			digitalWrite(N_Driver_A, LOW);
			digitalWrite(N_Driver_B, LOW);
			digitalWrite(N_Driver_C, LOW);
			digitalWrite(N_Driver_D, HIGH);
			break;

		case 9:
			digitalWrite(N_Driver_A, HIGH);
			digitalWrite(N_Driver_B, LOW);
			digitalWrite(N_Driver_C, LOW);
			digitalWrite(N_Driver_D, HIGH);
			break;

		case 10:
			digitalWrite(N_Driver_A, LOW);
			digitalWrite(N_Driver_B, HIGH);
			digitalWrite(N_Driver_C, LOW);
			digitalWrite(N_Driver_D, HIGH);
			break;

		case 11:
			digitalWrite(N_Driver_A, HIGH);
			digitalWrite(N_Driver_B, HIGH);
			digitalWrite(N_Driver_C, LOW);
			digitalWrite(N_Driver_D, HIGH);
			break;

		case 12:
			digitalWrite(N_Driver_A, LOW);
			digitalWrite(N_Driver_B, LOW);
			digitalWrite(N_Driver_C, HIGH);
			digitalWrite(N_Driver_D, HIGH);
			break;

		case 13:
			digitalWrite(N_Driver_A, HIGH);
			digitalWrite(N_Driver_B, LOW);
			digitalWrite(N_Driver_C, HIGH);
			digitalWrite(N_Driver_D, HIGH);
			break;
		case 14:
			digitalWrite(N_Driver_A, LOW);
			digitalWrite(N_Driver_B, HIGH);
			digitalWrite(N_Driver_C, HIGH);
			digitalWrite(N_Driver_D, HIGH);
			break;

		case 15:
			digitalWrite(N_Driver_A, HIGH);
			digitalWrite(N_Driver_B, HIGH);
			digitalWrite(N_Driver_C, HIGH);
			digitalWrite(N_Driver_D, HIGH);
			break;

		default:
			digitalWrite(N_Driver_A, LOW);
			digitalWrite(N_Driver_B, LOW);
			digitalWrite(N_Driver_C, LOW);
			digitalWrite(N_Driver_D, LOW);
			break;
		}
		break;
		
	case IN14:
		switch (i)
		{
		case 0:
			digitalWrite(N_Driver_A, LOW);
			digitalWrite(N_Driver_B, LOW);
			digitalWrite(N_Driver_C, LOW);
			digitalWrite(N_Driver_D, LOW);
			break;

		case 1:
			digitalWrite(N_Driver_A, HIGH);
			digitalWrite(N_Driver_B, LOW);
			digitalWrite(N_Driver_C, LOW);
			digitalWrite(N_Driver_D, HIGH);
			break;

		case 2:
			digitalWrite(N_Driver_A, LOW);
			digitalWrite(N_Driver_B, LOW);
			digitalWrite(N_Driver_C, LOW);
			digitalWrite(N_Driver_D, HIGH);
			break;

		case 3:
			digitalWrite(N_Driver_A, HIGH);
			digitalWrite(N_Driver_B, HIGH);
			digitalWrite(N_Driver_C, HIGH);
			digitalWrite(N_Driver_D, LOW);
			break;

		case 4:
			digitalWrite(N_Driver_A, LOW);
			digitalWrite(N_Driver_B, HIGH);
			digitalWrite(N_Driver_C, HIGH);
			digitalWrite(N_Driver_D, LOW);
			break;

		case 5:
			digitalWrite(N_Driver_A, HIGH);
			digitalWrite(N_Driver_B, LOW);
			digitalWrite(N_Driver_C, HIGH);
			digitalWrite(N_Driver_D, LOW);
			break;
		case 6:
			digitalWrite(N_Driver_A, LOW);
			digitalWrite(N_Driver_B, LOW);
			digitalWrite(N_Driver_C, HIGH);
			digitalWrite(N_Driver_D, LOW);
			break;

		case 7:
			digitalWrite(N_Driver_A, HIGH);
			digitalWrite(N_Driver_B, HIGH);
			digitalWrite(N_Driver_C, LOW);
			digitalWrite(N_Driver_D, LOW);
			break;

		case 8:
			digitalWrite(N_Driver_A, LOW);
			digitalWrite(N_Driver_B, HIGH);
			digitalWrite(N_Driver_C, LOW);
			digitalWrite(N_Driver_D, LOW);
			break;

		case 9:
			digitalWrite(N_Driver_A, HIGH);
			digitalWrite(N_Driver_B, LOW);
			digitalWrite(N_Driver_C, LOW);
			digitalWrite(N_Driver_D, LOW);
			break;

		case 10:
			digitalWrite(N_Driver_A, LOW);
			digitalWrite(N_Driver_B, HIGH);
			digitalWrite(N_Driver_C, LOW);
			digitalWrite(N_Driver_D, HIGH);
			break;

		case 11:
			digitalWrite(N_Driver_A, HIGH);
			digitalWrite(N_Driver_B, HIGH);
			digitalWrite(N_Driver_C, LOW);
			digitalWrite(N_Driver_D, HIGH);
			break;

		case 12:
			digitalWrite(N_Driver_A, LOW);
			digitalWrite(N_Driver_B, LOW);
			digitalWrite(N_Driver_C, HIGH);
			digitalWrite(N_Driver_D, HIGH);
			break;

		case 13:
			digitalWrite(N_Driver_A, HIGH);
			digitalWrite(N_Driver_B, LOW);
			digitalWrite(N_Driver_C, HIGH);
			digitalWrite(N_Driver_D, HIGH);
			break;
		case 14:
			digitalWrite(N_Driver_A, LOW);
			digitalWrite(N_Driver_B, HIGH);
			digitalWrite(N_Driver_C, HIGH);
			digitalWrite(N_Driver_D, HIGH);
			break;

		case 15:
			digitalWrite(N_Driver_A, HIGH);
			digitalWrite(N_Driver_B, HIGH);
			digitalWrite(N_Driver_C, HIGH);
			digitalWrite(N_Driver_D, HIGH);
			break;

		default:
			digitalWrite(N_Driver_A, LOW);
			digitalWrite(N_Driver_B, LOW);
			digitalWrite(N_Driver_C, LOW);
			digitalWrite(N_Driver_D, LOW);
			break;
		}
		break;*/
#pragma endregion
		
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

		GPIOB_BASE->BSRR &= ~(N_Driver_A_ADDR | N_Driver_B_ADDR | N_Driver_C_ADDR | N_Driver_D_ADDR);        // clear previous values
		GPIOB_BASE->BSRR |= portValue;        // set new values
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
			digitalWrite(N_Second1, HIGH);
			break;
		case HOURS_LSB:
			digitalWrite(N_Second2, HIGH);
			break;
		case MINUTES_MSB:
			digitalWrite(N_Minute1, HIGH);
			break;
		case MINUTES_LSB:
			digitalWrite(N_Minute2, HIGH);
			break;
		case SECONDS_MSB:
			digitalWrite(N_Hour1, HIGH);
			break;
		case SECONDS_LSB:
			digitalWrite(N_Hour2, HIGH);
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
			digitalWrite(N_Hour2, HIGH);
			break;
		case HOURS_LSB:
			digitalWrite(N_Hour1, HIGH);
			break;
		case MINUTES_MSB:
			digitalWrite(N_Minute2, HIGH);
			break;
		case MINUTES_LSB:
			digitalWrite(N_Minute1, HIGH);
			break;
		case SECONDS_MSB:
			digitalWrite(N_Second2, HIGH);
			break;
		case SECONDS_LSB:
			digitalWrite(N_Second1, HIGH);
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
	GPIOB_BASE->BSRR &= 0b11000000;
	//digitalWrite(N_Hour2, LOW);
	//digitalWrite(N_Hour1, LOW);
	//digitalWrite(N_Minute2, LOW);
	//digitalWrite(N_Minute1, LOW);
	//digitalWrite(N_Second2, LOW);
	//digitalWrite(N_Second1, LOW);
}

void SwitchDot(int i) {
	switch (i) {
	case DOT_SWITCH:
		digitalWrite(N_Dot, !digitalRead(N_Dot));
		break;
	case DOT_ON:
		digitalWrite(N_Dot, HIGH);
		break;
	case DOT_OFF:
		digitalWrite(N_Dot, LOW);
		break;
	}
}