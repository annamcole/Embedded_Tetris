/*
 * s1_task.h
 *
 *  Created on: Apr 21, 2021
 *      Author: Ian
 */

#ifndef S1_TASK_H_
#define S1_TASK_H_

#include <main.h>

extern TaskHandle_t Task_S1_Handle;

/******************************************************************************
 * This function will initialize Queue_Space_Ship
 ******************************************************************************/
void Task_S1_Init(void);

/******************************************************************************
 * This task manages the movement of the space ship. The joystick task or the
 * console task can send messages to the
 ******************************************************************************/
bool S1_Pressed(void);

/******************************************************************************
 * This function debounces the pushing of the S1 button
 ******************************************************************************/
bool Debounce_S1(void);

/******************************************************************************
 * This function performs task_s1.
 * If the S1 button has been pressed, then send a message to
 * Queue_Tetris to have the image fall down the LCD screen.
 ******************************************************************************/
void task_s1(void *pvParameters);

#endif /* S1_TASK_H_ */
