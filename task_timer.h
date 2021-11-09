/*
 * task_timer.h
 */

#ifndef TASK_TIMER_H_
#define TASK_TIMER_H_

extern TaskHandle_t Task_Timer_Handle;

/*****************************************************
 * Sets the period at which to trigger an interrupt
 * for T32_1
 *****************************************************/
void timer1_task_init(int ms);

/******************************************************************************
* Bottom Half Task. Sends the drop message to Tetris periodically
******************************************************************************/
void Task_Timer_Bottom_Half(void *pvParameters);

#endif
