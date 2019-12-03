/*
 * main.c
 *
 *  Created on: 2017 Oct 25 16:08:21
 *  Author: Tobias
 */

#include "lm75.h"              //Declarations from DAVE Code Generation (includes SFR declaration)

unsigned int lm75_N();
unsigned int lm75_SW();
unsigned int lm75_SE();

int delay(int);
void setupLED();
int handleTemp();
void colorALL(uint16_t, uint16_t, uint16_t);
void color_N(uint16_t blue, uint16_t red, uint16_t green);
void color_SW(uint16_t blue, uint16_t red, uint16_t green);
void color_SE(uint16_t blue, uint16_t red, uint16_t green);

uint8_t trigger[1];

uint8_t usb = 0;

static uint8_t index_N, index_SW, index_SE;
static uint8_t data_N;
static uint8_t data_SW;
static uint8_t data_SE;
int8_t temp_N[1];
int8_t temp_SW[1];
uint8_t temp_SE[1];
uint8_t average_temp[1];
uint8_t rgb[4] = { 0 };
int a;

/**

 * @brief main() - Application entry point
 *
 * <b>Details of function</b><br>
 * This routine is the application entry point. It is invoked by the device startup code. It is responsible for
 * invoking the APP initialization dispatcher routine - DAVE_Init() and hosting the place-holder for user application
 * code.
 */

int main(void) {
	DAVE_STATUS_t status;
//	uint32_t index = 0;

	status = DAVE_Init(); /* Initialization of DAVE APPs  */

	if (status != DAVE_STATUS_SUCCESS) {
		/* Placeholder for error handler code. The while loop below can be replaced with an user error handler. */
		XMC_DEBUG("DAVE APPs initialization failed\n");

		while (1U) {

		}
	}
	setupLED();

//	PWM_SetDutyCycle(&PWM_0, 255);		// Red
	uint8_t rgb[4] = { 0 };
	/* Placeholder for user application code. The while loop below can be replaced with user application code. */
	while (1U) {

//      Only Transmit UART
//		UART_Transmit(&UART, "Hallo", sizeof("Hallo"));
		//lm75_SW();
		//lm75_SE();
		//	lm75_N();
//
//			handleTemp();

		delay(1000);

		if (UART_Receive(&UART_BL, rgb, 4) == UART_STATUS_SUCCESS) {
			if (rgb[0] == 1) {

				a = 1;
				while (a == 1) {
					lm75_N();
					lm75_SW();

					lm75_SE();
					handleTemp();
					if (rgb[0] != 1) {
						a = 0;
					}
				}
			}

			if (rgb[0] == 2) {
				while (rgb[0] == 2) {
					UART_Receive(&UART_BL, rgb, 4);
					color_N(rgb[1], rgb[2], rgb[3]);
				}
			}

			if (rgb[0] == 3) {
				while (rgb[0] == 3) {
					UART_Receive(&UART_BL, rgb, 4);
					color_SW(rgb[1], rgb[2], rgb[3]);
				}
			}

			if (rgb[0] == 4) {
				while (rgb[0] == 4) {
					UART_Receive(&UART_BL, rgb, 4);
					color_SE(rgb[1], rgb[2], rgb[3]);
				}
			}

			if (rgb[0] == 66) {
				setupLED();
				rgb[0] = 0;
			}
		} else if (rgb[0] != 1 && rgb[0] != 2 && rgb[0] != 3 && rgb[0] != 4
				&& rgb[0] != 66) {
			lm75_N();
			lm75_SW();
			lm75_SE();
			handleTemp();
		}

	}
	return 1U;

}

// UART output to console

unsigned int lm75_N() {

	index_N = 0;

	I2C_MASTER_SendStart(&I2C_MASTER_0, TEMP_ADDRESS_W_N, XMC_I2C_CH_CMD_WRITE); //
	while (I2C_MASTER_GetFlagStatus(&I2C_MASTER_0,
			XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U) {

	}
	I2C_MASTER_ClearFlag(&I2C_MASTER_0, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);
// write to address 0
	I2C_MASTER_TransmitByte(&I2C_MASTER_0, TEMP_ADDRESS_T); //0x00
	while (I2C_MASTER_GetFlagStatus(&I2C_MASTER_0,
			XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U) {
		// wait for ACK
	}

	I2C_MASTER_ClearFlag(&I2C_MASTER_0, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);
	I2C_MASTER_SendRepeatedStart(&I2C_MASTER_0, TEMP_ADDRESS_W_N,
			XMC_I2C_CH_CMD_READ);
	while (I2C_MASTER_GetFlagStatus(&I2C_MASTER_0,
			XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U) {
		// wait for ACK
	}
	I2C_MASTER_ClearFlag(&I2C_MASTER_0, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);
	while (index_N < 16) {
		I2C_MASTER_ReceiveACK(&I2C_MASTER_0);
		index_N++;
	}
// only for the last byte
	I2C_MASTER_ReceiveNACK(&I2C_MASTER_0);
//Wait when Rx FIFO is empty
	while (!I2C_MASTER_IsRXFIFOEmpty(&I2C_MASTER_0)) {
		data_N = I2C_MASTER_GetReceivedByte(&I2C_MASTER_0);
	}
	temp_N[0] = data_N;

	I2C_MASTER_SendStop(&I2C_MASTER_0);

	delay(750);

	return (temp_N);

}
unsigned int lm75_SW() {

	index_SW = 0;

	I2C_MASTER_SendStart(&I2C_MASTER_0, TEMP_ADDRESS_W_SW,
			XMC_I2C_CH_CMD_WRITE);
	while (I2C_MASTER_GetFlagStatus(&I2C_MASTER_0,
			XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U) {

	}
	I2C_MASTER_ClearFlag(&I2C_MASTER_0, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);
// write to address 0
	I2C_MASTER_TransmitByte(&I2C_MASTER_0, TEMP_ADDRESS_T); //0x00
	while (I2C_MASTER_GetFlagStatus(&I2C_MASTER_0,
			XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U) {
// wait for ACK
	}

	I2C_MASTER_ClearFlag(&I2C_MASTER_0, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);
	I2C_MASTER_SendRepeatedStart(&I2C_MASTER_0, TEMP_ADDRESS_W_SW,
			XMC_I2C_CH_CMD_READ);
	while (I2C_MASTER_GetFlagStatus(&I2C_MASTER_0,
			XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U) {
// wait for ACK
	}
	I2C_MASTER_ClearFlag(&I2C_MASTER_0, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);
	while (index_SW < 16) {
		I2C_MASTER_ReceiveACK(&I2C_MASTER_0);
		index_SW++;
	}
// only for the last byte
	I2C_MASTER_ReceiveNACK(&I2C_MASTER_0);
//Wait when Rx FIFO is empty
	while (!I2C_MASTER_IsRXFIFOEmpty(&I2C_MASTER_0)) {
		data_SW = I2C_MASTER_GetReceivedByte(&I2C_MASTER_0);
	}
	temp_SW[0] = data_SW;

	I2C_MASTER_SendStop(&I2C_MASTER_0);

	delay(750);

	return (temp_SW[0]);

}

unsigned int lm75_SE() {

	index_SE = 0;

	I2C_MASTER_SendStart(&I2C_MASTER_0, TEMP_ADDRESS_W_SE,
			XMC_I2C_CH_CMD_WRITE);
	while (I2C_MASTER_GetFlagStatus(&I2C_MASTER_0,
			XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U) {

	}
	I2C_MASTER_ClearFlag(&I2C_MASTER_0, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);
// write to address 0
	I2C_MASTER_TransmitByte(&I2C_MASTER_0, TEMP_ADDRESS_T); //0x00
	while (I2C_MASTER_GetFlagStatus(&I2C_MASTER_0,
			XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U) {
// wait for ACK
	}

	I2C_MASTER_ClearFlag(&I2C_MASTER_0, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);
	I2C_MASTER_SendRepeatedStart(&I2C_MASTER_0, TEMP_ADDRESS_W_SE,
			XMC_I2C_CH_CMD_READ);
	while (I2C_MASTER_GetFlagStatus(&I2C_MASTER_0,
			XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U) {
// wait for ACK
	}
	I2C_MASTER_ClearFlag(&I2C_MASTER_0, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);
	while (index_SE < 16) {
		I2C_MASTER_ReceiveACK(&I2C_MASTER_0);
		index_SE++;
	}
// only for the last byte
	I2C_MASTER_ReceiveNACK(&I2C_MASTER_0);
//Wait when Rx FIFO is empty
	while (!I2C_MASTER_IsRXFIFOEmpty(&I2C_MASTER_0)) {
		data_SE = I2C_MASTER_GetReceivedByte(&I2C_MASTER_0);
	}
	temp_SE[0] = data_SE;

	I2C_MASTER_SendStop(&I2C_MASTER_0);

	delay(750);

	return (temp_SE[0]);

}

int delay(int x) {
	for (int i = 0; i < x; i++)
		;
}

void setupLED() {

	PWM_SetDutyCycle(&Red_N, 0);
	PWM_SetDutyCycle(&Green_N, 0);
	PWM_SetDutyCycle(&Blue_N, 0);
	PWM_SetDutyCycle(&Red_SW, 0);
	PWM_SetDutyCycle(&Green_SW, 0);
	PWM_SetDutyCycle(&Blue_SW, 0);
	PWM_SetDutyCycle(&Red_SE, 0);
	PWM_SetDutyCycle(&Green_SE, 0);
	PWM_SetDutyCycle(&Blue_SE, 0);

}

int handleTemp() {

	average_temp[0] = ((data_N + data_SW + data_SE) / 3);
	UART_Transmit(&UART_BL, average_temp, 1);

	if (average_temp[0] < 0) {
		colorALL(0xE8, 0xFF, 0xFA);
	}
	if (average_temp[0] >= 0 && average_temp[0] <= 5) {
		colorALL(0xA3, 0xE4, 0xFF);
	}
	if (average_temp[0] > 5 && average_temp[0] <= 10) {
		colorALL(0x84, 0xDA, 0xFF);
	}
	if (average_temp[0] > 10 && average_temp[0] <= 15) {
		colorALL(0x96, 0xB7, 0xFF);
	}
	if (average_temp[0] > 15 && average_temp[0] <= 20) {
		colorALL(0x00, 0x00, 0xFF);
	}
	if (average_temp[0] > 20 && average_temp[0] <= 25) {
		colorALL(0xFF, 0x80, 0x00);
	}
	if (average_temp[0] > 25 && average_temp[0] <= 30) {
		colorALL(0xFF, 0x5F, 0x2E);
		//colorALL(0xFF,0xFF,0xFF);
		//colorALL(0x00, 0xFF, 0x00);
	}
	if (average_temp[0] > 30 && average_temp[0] <= 35) {
		colorALL(0xFF, 0x68, 0x2E);
//colorALL(0xFF, 0x00,0x00);
	}
	if (average_temp[0] > 35 && average_temp[0] <= 40) {
		colorALL(0xFF, 0x5F, 0x19);
	}
	if (average_temp[0] > 40 && average_temp[0] <= 45) {
		colorALL(0xFF, 0x5F, 0x00);
	}
	if (average_temp[0] > 45 && average_temp[0] <= 50) {
		colorALL(0xFF, 0x4C, 0x00);
	}
	if (average_temp[0] > 50 && average_temp[0] <= 60) {
		colorALL(0xFF, 0x40, 0x2E);
	}
	if (average_temp[0] > 60 && average_temp[0] <= 75) {
		colorALL(0xFF, 0x40, 0x19);
	}
	if (average_temp[0] > 75 && average_temp[0] <= 100) {
		colorALL(0xFF, 0x33, 0x16);
	}
	if (average_temp[0] > 100) {
		colorALL(0xFF, 0x00, 0x00);
	}
//	if (data2 >= 24 || data <= 30){
//		color(0xFF,0x00,0x00);
//	}
//	if (data2 >= 27)
//		color(0x00,0xFF,0x00);
//	if(data2 >= 30)
//		color(0x00,0x00,0xFF);
}

void colorALL(uint16_t red, uint16_t green, uint16_t blue) {
	uint16_t redPortion = red * 10000 / 255;
	uint16_t greenPortion = green * 10000 / 255;
	uint16_t bluePortion = blue * 10000 / 255;

	PWM_SetDutyCycle(&Red_N, redPortion);
	PWM_SetDutyCycle(&Green_N, greenPortion);
	PWM_SetDutyCycle(&Blue_N, bluePortion);
	PWM_SetDutyCycle(&Red_SW, redPortion);
	PWM_SetDutyCycle(&Green_SW, greenPortion);
	PWM_SetDutyCycle(&Blue_SW, bluePortion);
	PWM_SetDutyCycle(&Red_SE, redPortion);
	PWM_SetDutyCycle(&Green_SE, greenPortion);
	PWM_SetDutyCycle(&Blue_SE, bluePortion);
}

void color_N(uint16_t red, uint16_t green, uint16_t blue) {
	uint16_t redPortion = red * 10000 / 255;
	uint16_t greenPortion = green * 10000 / 255;
	uint16_t bluePortion = blue * 10000 / 255;

	PWM_SetDutyCycle(&Red_N, redPortion);
	PWM_SetDutyCycle(&Green_N, greenPortion);
	PWM_SetDutyCycle(&Blue_N, bluePortion);
}

void color_SW(uint16_t red, uint16_t green, uint16_t blue) {

	uint16_t redPortion = red * 10000 / 255;
	uint16_t greenPortion = green * 10000 / 255;
	uint16_t bluePortion = blue * 10000 / 255;

	PWM_SetDutyCycle(&Red_SW, redPortion);
	PWM_SetDutyCycle(&Green_SW, greenPortion);
	PWM_SetDutyCycle(&Blue_SW, bluePortion);
}

void color_SE(uint16_t red, uint16_t green, uint16_t blue) {
	uint16_t redPortion = red * 10000 / 255;
	uint16_t greenPortion = green * 10000 / 255;
	uint16_t bluePortion = blue * 10000 / 255;

	PWM_SetDutyCycle(&Red_SE, redPortion);
	PWM_SetDutyCycle(&Green_SE, greenPortion);
	PWM_SetDutyCycle(&Blue_SE, bluePortion);
}

