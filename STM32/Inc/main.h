#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

void Error_Handler(void);

#define USER_BUTTON_Pin GPIO_PIN_0
#define USER_BUTTON_GPIO_Port GPIOA
#define M5_PWM_Pin GPIO_PIN_9
#define M5_PWM_GPIO_Port GPIOE
#define M4_PWM_Pin GPIO_PIN_11
#define M4_PWM_GPIO_Port GPIOE
#define M3_PWM_Pin GPIO_PIN_13
#define M3_PWM_GPIO_Port GPIOE
#define M2_PWM_Pin GPIO_PIN_14
#define M2_PWM_GPIO_Port GPIOE
#define M1_PWM_Pin GPIO_PIN_12
#define M1_PWM_GPIO_Port GPIOD
#define M1_INB_Pin GPIO_PIN_3
#define M1_INB_GPIO_Port GPIOD
#define M1_IN_Pin GPIO_PIN_4
#define M1_IN_GPIO_Port GPIOD
#define M2_INB_Pin GPIO_PIN_5
#define M2_INB_GPIO_Port GPIOD
#define M2_IN_Pin GPIO_PIN_6
#define M2_IN_GPIO_Port GPIOD
#define M3_INB_Pin GPIO_PIN_7
#define M3_INB_GPIO_Port GPIOD
#define M3_IN_Pin GPIO_PIN_3
#define M3_IN_GPIO_Port GPIOB
#define M4_INB_Pin GPIO_PIN_4
#define M4_INB_GPIO_Port GPIOB
#define M4_IN_Pin GPIO_PIN_5
#define M4_IN_GPIO_Port GPIOB
#define M5_INB_Pin GPIO_PIN_6
#define M5_INB_GPIO_Port GPIOB
#define M5_IN_Pin GPIO_PIN_7
#define M5_IN_GPIO_Port GPIOB

// Extern Variables
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim4;
extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_rx;

/* Convert the three digits into a integer value
Return: Returns the new integer value              */
int toInt(uint8_t digit_1, uint8_t digit_2, uint8_t digit_3);

// Runs when a DMA USART process is complete. Process the data in the buffer and
// save it to the appropritate global variables
//
// Restart the DMA USART process again
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

// Set the Speed for Motor 5.
// Input: Integer value between -100 to 1000
// Conditions:
//    speed < 0 : the joint rotates down
//    speed > 0 : the joint rotates up
//    speed == 0: the joint stops moving
void Set_M5_Direction(int error);

// Set the Speed for Motor 4.
// Input: Integer value between -100 to 1000
// Conditions:
//    speed < 0 : the joint rotates down
//    speed > 0 : the joint rotates up
//    speed == 0: the joint stops moving
void Set_M4_Direction(int error);

// Set the Speed for Motor 3.
// Input: Integer value between -100 to 1000
// Conditions:
//    speed < 0 : the joint rotates down
//    speed > 0 : the joint rotates up
//    speed == 0: the joint stops moving
void Set_M3_Direction(int error);

// Set the Speed for Motor 2.
// Input: Integer value between -100 to 1000
// Conditions:
//    speed < 0 : the joint rotates down
//    speed > 0 : the joint rotates up
//    speed == 0: the joint stops moving
void Set_M2_Direction(int error);
// Set the Speed for Motor 1.
// Input: Integer value between -100 to 1000
// Conditions:
//    speed < 0 : the joint rotates to the left
//    speed > 0 : the joint rotates to the right
//    speed == 0: the joint stops moving
void Set_M1_Direction(int error);

// This function is hard coded to rotate the motor left while checking the QR Decoded message.
// It will center the camera at the target QR Code, i.e. either Red, Yellow or Green.
void Move_to_ColorQRCode(char target);

// This function is hard coded to rotate the motor to the right while checking the QR Decoded message.
// It will center the camera at the "Transport" QR Code.
void Horizontal_to_PickUpQRCode();

// This function is hard coded to rotate Motor 3 upwards while checking to detect a QR Code.
// Centers the object counter about the line y = 130
void Vertical_to_QRCode();

// This function is hard coded to rotate Motor 3 down while checking to detected a Red, Green, or Yellow object.
// Centers the object counter about the point (320,400)
// Returns: The color of the object detected
char Vertical_to_Color();

// Energize motor 5 to opem the gripper. Moving the gripper for 500 milliseconds
// was enough delay to open the gripper to a fully-open position.
void OpenGripper();

// Energize motor 5 to close the gripper. Moving the gripper for 510 milliseconds
// was enough delay to close and hold the object.
void CloseGripper();

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
