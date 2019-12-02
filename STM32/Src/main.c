/**
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
#include "main.h"
#include "Configuration.h"
#include "string.h"
#include "stdint.h"
#include "stdlib.h"

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim4;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;
uint8_t buffer[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int result, color_xPos, color_yPos, QR_xPos, QR_yPos, error;
char Color, QR_Code, target_Color;
int color_xErr, color_yErr;

int main(void)
{
  // Microcontrollower set up
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM1_Init();
  MX_TIM4_Init();
  MX_USART2_UART_Init();
  HAL_UART_Receive_DMA(&huart2, buffer, 14 );

  while (1) {
    // Wait for User Button Press before running
    if (HAL_GPIO_ReadPin(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin)) {
      while (1){

        // Rotate the motor to the right until "Transport" QR Code is seen
        Horizontal_to_PickUpQRCode();
        HAL_Delay(1000);

        // Rotate Motor 3 down to find the colored object and center it.
        // Returns the color of the object
        target_Color = Vertical_to_Color();
        HAL_Delay(1000);

        // Pick up the object
        CloseGripper();

        // Rotate Motor 3 Vertically and center about previous QR code
        Vertical_to_QRCode();
        HAL_Delay(1000);

        // Rotate the Robot left towards the target color QR Code
        Move_to_ColorQRCode(target_Color);
        HAL_Delay(1000);

        // Lower Motor 3 for a 1.6 seconds
        Set_M3_Direction(-80);
        HAL_Delay(1600);
        Set_M3_Direction(0);
        HAL_Delay(1000);

        // Drop the object
        OpenGripper();

        // Move up to QR code
        Vertical_to_QRCode();
        HAL_Delay(1000);
      }
    }
  }
}
/* Convert the three digits into a integer value
Return: Returns the new integer value              */
int toInt(uint8_t digit_1, uint8_t digit_2, uint8_t digit_3){
	result = (digit_1 - 48)*100 + (digit_2 - 48)*10 + (digit_3 - 48);
	return result;
}

// Runs when a DMA USART process is complete. Process the data in the buffer and
// save it to the appropritate global variables
//
// Restart the DMA USART process again
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	//Set the data to its corresponding variable
	Color = buffer[0];
	color_xPos = toInt(buffer[1], buffer[2],buffer[3]);
	color_yPos = toInt(buffer[4], buffer[5],buffer[6]);
	QR_Code = buffer[7];
	QR_xPos = toInt(buffer[8], buffer[9],buffer[10]);
	QR_yPos = toInt(buffer[11], buffer[12],buffer[13]);

	//Restart the DMA Process to read Serial Data
	HAL_UART_Receive_DMA(&huart2, buffer, 14);
}

// Set the Speed for Motor 1.
// Input: Integer value between -100 to 1000
// Conditions:
//    speed < 0 : the joint rotates to the left
//    speed > 0 : the joint rotates to the right
//    speed == 0: the joint stops moving
void Set_M1_Direction(int speed) {
	uint8_t dutycycle;
	if(abs(speed) < 20){

		HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);

	} else if (speed < 0){

		dutycycle = abs(speed);
		TIM4->CCR1 = dutycycle;
		HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
		HAL_GPIO_WritePin(M1_IN_GPIO_Port, M1_IN_Pin, 1);
		HAL_GPIO_WritePin(M1_INB_GPIO_Port, M1_INB_Pin, 0);

	} else {

		dutycycle = abs(speed);
		TIM4->CCR1 = dutycycle;
		HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
		HAL_GPIO_WritePin(M1_IN_GPIO_Port, M1_IN_Pin, 0);
		HAL_GPIO_WritePin(M1_INB_GPIO_Port, M1_INB_Pin, 1);

	}
}
// Set the Speed for Motor 2.
// Input: Integer value between -100 to 1000
// Conditions:
//    speed < 0 : the joint rotates down
//    speed > 0 : the joint rotates up
//    speed == 0: the joint stops moving
void Set_M2_Direction(int speed) {
	uint8_t dutycycle;
	if(abs(speed) < 20){

		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_4);

	} else if(speed < 0){

		dutycycle = abs(speed);
		TIM1->CCR4 = dutycycle;
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
		HAL_GPIO_WritePin(M2_IN_GPIO_Port, M2_IN_Pin, 1);
		HAL_GPIO_WritePin(M2_INB_GPIO_Port, M2_INB_Pin, 0);

	}else {

		dutycycle = abs(speed);
		TIM1->CCR4 = dutycycle;
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
		HAL_GPIO_WritePin(M2_IN_GPIO_Port, M2_IN_Pin, 0);
		HAL_GPIO_WritePin(M2_INB_GPIO_Port, M2_INB_Pin, 1);

	}
}
// Set the Speed for Motor 3.
// Input: Integer value between -100 to 1000
// Conditions:
//    speed < 0 : the joint rotates down
//    speed > 0 : the joint rotates up
//    speed == 0: the joint stops moving
void Set_M3_Direction(int speed) {
	uint8_t dutycycle;
	if(abs(speed) < 20){

		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);

	} else if(speed < 0){

		dutycycle = abs(speed);
		TIM1->CCR3 = dutycycle;
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
		HAL_GPIO_WritePin(M3_IN_GPIO_Port, M3_IN_Pin, 1);
		HAL_GPIO_WritePin(M3_INB_GPIO_Port, M3_INB_Pin, 0);

	} else if(speed > 0){

		dutycycle = abs(speed);
		TIM1->CCR3 = dutycycle;
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
		HAL_GPIO_WritePin(M3_IN_GPIO_Port, M3_IN_Pin, 0);
		HAL_GPIO_WritePin(M3_INB_GPIO_Port, M3_INB_Pin, 1);

	}

}
// Set the Speed for Motor 4.
// Input: Integer value between -100 to 1000
// Conditions:
//    speed < 0 : the joint rotates down
//    speed > 0 : the joint rotates up
//    speed == 0: the joint stops moving
void Set_M4_Direction(int speed)
{
	uint8_t dutycycle;
	if(abs(speed) < 20){

		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);

	} else if(speed < 0){

		dutycycle=abs(speed);
		TIM1->CCR2 = dutycycle;
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
		HAL_GPIO_WritePin(M4_IN_GPIO_Port, M4_IN_Pin, 1);
		HAL_GPIO_WritePin(M4_INB_GPIO_Port, M4_INB_Pin, 0);

	} else if(speed > 0){

		dutycycle = abs(speed);
		TIM1->CCR2 = dutycycle;
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
		HAL_GPIO_WritePin(M4_IN_GPIO_Port, M4_IN_Pin, 0);
		HAL_GPIO_WritePin(M4_INB_GPIO_Port, M4_INB_Pin, 1);

	}

}
// Set the Speed for Motor 5.
// Input: Integer value between -100 to 1000
// Conditions:
//    speed < 0 : the joint rotates down
//    speed > 0 : the joint rotates up
//    speed == 0: the joint stops moving
void Set_M5_Direction(int speed)
{
	uint8_t dutycycle;
	if(abs(speed) < 20){

		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);

	} else if(speed < 0){

		dutycycle = abs(speed);
		TIM1->CCR1 = dutycycle;
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
		HAL_GPIO_WritePin(M5_IN_GPIO_Port, M5_IN_Pin, 1);
		HAL_GPIO_WritePin(M5_INB_GPIO_Port, M5_INB_Pin, 0);

	} else if(speed > 0){

		dutycycle = abs(speed);
		TIM1->CCR1 = dutycycle;
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
		HAL_GPIO_WritePin(M5_IN_GPIO_Port, M5_IN_Pin, 0);
		HAL_GPIO_WritePin(M5_INB_GPIO_Port, M5_INB_Pin, 1);

	}
}
// This function is hard coded to rotate the motor left while checking the QR Decoded message.
// It will center the camera at the target QR Code, i.e. either Red, Yellow or Green.
void Move_to_ColorQRCode(char target){
	while(1){
    // Check if the QR Code on camera is not the target
		if(QR_Code != target){
      // Keep rotating towards the left
			Set_M1_Direction(-80);
			HAL_Delay(300);
			Set_M1_Direction(0);
			HAL_Delay(350);

		} else{
			// When the target QRCode is seen, attempt to center at x axis value 320
			// Loop will center about the x-axis
			while(1){
				error = QR_xPos - 320;
        // Once it is within 20 pixels of the center, stop and exit loop
				if(abs(error) < 20){

					Set_M1_Direction(0);
					HAL_Delay(300);
					break;

				} else if(error < 0) {
          // The QR Code is on the right side, move left
					Set_M1_Direction(-60);
					HAL_Delay(90);
					Set_M1_Direction(0);
					HAL_Delay(300);

				} else {
          // The QR Code is on the left side, move right
					Set_M1_Direction(60);
					HAL_Delay(90);
					Set_M1_Direction(0);
					HAL_Delay(300);

				}
			}
      //exit main loop
			Set_M1_Direction(0);
			HAL_Delay(300);
			break;
		}
	}
}
// This function is hard coded to rotate the motor to the right while checking the QR Decoded message.
// It will center the camera at the "Transport" QR Code.
void Horizontal_to_PickUpQRCode(){
	while(1){
      // Check if the QR Code on camera is not the target
			if(QR_Code != 'T'){
        // Keep rotating to the right
				Set_M1_Direction(80);
				HAL_Delay(300);
				Set_M1_Direction(0);
				HAL_Delay(500);

			} else{
        // When the "Transport" QRCode is seen, attempt to center at x axis value 320
				// Loop will center about the x-axis
				while(1){
					error = QR_xPos - 320;
          // Once it is within 20 pixels of the center, stop and exit loop
					if(abs(error) < 20){

						Set_M1_Direction(0);
						HAL_Delay(300);
						break;

					} else if(error < 0) {
            // The QR Code is on the right side, move left
						Set_M1_Direction(-60);
						HAL_Delay(50);
						Set_M1_Direction(0);
						HAL_Delay(300);

					} else {
            // The QR Code is on the left side, move right
						Set_M1_Direction(60);
						HAL_Delay(50);
						Set_M1_Direction(0);
						HAL_Delay(300);

					}
				}
        //exit main loop
				Set_M1_Direction(0);
				HAL_Delay(300);
				break;
			}
		}
}
// This function is hard coded to rotate Motor 3 upwards while checking to detect a QR Code.
// Centers the object counter about the line y = 130
void Vertical_to_QRCode(){
	while(1){
    //The if statement only executes when a QR Code is in the camera frame.
		if(QR_Code != 'N'){

			while(1){
				error = 130 - QR_yPos;
				if(abs(error) < 10){
          // When error is within 10 pixels of 130, stop motor and exit loop
					Set_M3_Direction(0);
					HAL_Delay(300);
					break;

				} else if(error < 0) {
          // When error is negative, rotate Motor 3 down
					Set_M3_Direction(-50);
					HAL_Delay(75);
					Set_M3_Direction(0);
					HAL_Delay(300);

				} else {
          // When error is positive, rotate Motor 3 Upwards
					Set_M3_Direction(80);
					HAL_Delay(100);
					Set_M3_Direction(0);
					HAL_Delay(300);

				}
			}
			Set_M1_Direction(0);
			HAL_Delay(300);
			break;

		} else {
      // Keep rotating upwards when no QR Code is in frame.
			Set_M3_Direction(100);
			HAL_Delay(200);
			Set_M3_Direction(0);
			HAL_Delay(300);

		}
	}
}
// This function is hard coded to rotate Motor 3 down while checking to detected a Red, Green, or Yellow object.
// Centers the object counter about the point (320,400)
// Returns: The color of the object detected
char Vertical_to_Color(){

	while(1){
    // When object is detected, the if statement executes to center the contour about (320,400)
		if(Color != 'N'){
			// Center y position at 390-410
			while(1){
				color_yErr = 400 - color_yPos;
				if(abs(color_yErr) < 10){
          // When error within 10 pixels of 400, stop motor and exit loop
					Set_M3_Direction(0);
					HAL_Delay(300);
					break;

				} else if(color_yErr < 0) {
          // When error is negative, rotate Motor 3 down.
					Set_M3_Direction(-50);
					HAL_Delay(75);
					Set_M3_Direction(0);
					HAL_Delay(300);

				} else {
          // When error is positive, rotate Motor 3 up.
					Set_M3_Direction(80);
					HAL_Delay(100);
					Set_M3_Direction(0);
					HAL_Delay(300);

				}
			}
			// Center x position at 320
			while(1){
				color_xErr = color_xPos - 320;
				if(abs(color_xErr) < 10){
          // When error within 10 pixels of 400, stop motor and exit loop
					Set_M1_Direction(0);
					HAL_Delay(300);
					break;

				} else if(color_xErr < 0) {
          // When error is negative, move towards the left
					Set_M1_Direction(-60);
					HAL_Delay(75);
					Set_M1_Direction(0);
					HAL_Delay(300);

				} else {
          // When error is positive, move towards the right
					Set_M1_Direction(60);
					HAL_Delay(50);
					Set_M1_Direction(0);
					HAL_Delay(300);

				}
			}
			Set_M1_Direction(0);
			HAL_Delay(300);
			break;

		} else {
      // While no object is detected, keep rotating down
			Set_M3_Direction(-60);
			HAL_Delay(100);
			Set_M3_Direction(0);
			HAL_Delay(300);

		}
	}
	return Color;
}
// Energize motor 5 to opem the gripper. Moving the gripper for 500 milliseconds
// was enough delay to open the gripper to a fully-open position.
void OpenGripper(){

  Set_M5_Direction(60);
  HAL_Delay(500);
  Set_M5_Direction(0);
  HAL_Delay(1000);

}

// Energize motor 5 to close the gripper. Moving the gripper for 510 milliseconds
// was enough delay to close and hold the object.
void CloseGripper(){

  Set_M5_Direction(-70);
  HAL_Delay(510);
  Set_M5_Direction(0);
  HAL_Delay(1000);

}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/