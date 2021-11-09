/*
 * task_s2.h
 */

#ifndef TASK_S2_H_
#define TASK_S2_H_

#include <main.h>

extern TaskHandle_t Task_S2_Handle;

/******************************************************************************
 * This function will initialize Queue_Space_Ship
 ******************************************************************************/
void Task_S2_Init(void);

/******************************************************************************
 * This task manages the movement of the space ship. The joystick task or the
 * console task can send messages to the
 ******************************************************************************/
bool S2_Pressed(void);

/******************************************************************************
 * This function debounces the pushing of the S2 button
 ******************************************************************************/
bool Debounce_S2(void);

/******************************************************************************
 * This function performs task_s2.
 * If the S2 button has been pressed, then send a message to
 * Queue_Tetris to have the image rotate on the LCD screen.
 ******************************************************************************/
void task_s2(void *pvParameters);

#endif /* TASK_SPACE_SHIP_H_ */
