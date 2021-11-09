/*
 * task_console.h
 */

#ifndef TASK_CONSOLE_H_
#define TASK_CONSOLE_H_

#include <main.h>

extern QueueHandle_t Queue_Console;
extern TaskHandle_t Task_Console_Handle;
extern SemaphoreHandle_t Sem_Console;

typedef enum {
    CONSOLE_CMD_CLEAR_ROW,
    CONSOLE_CMD_GAME_OVER
} CONSOLE_CMD_t;

typedef struct {
    CONSOLE_CMD_t cmd;
    uint8_t value;
} CONSOLE_MSG_t;

/******************************************************************************
 * This function configures the eUSCI_A0 to be a UART that communicates at
 * 115200 8N1.
 ******************************************************************************/
void Task_Console_Init(void);

/******************************************************************************
* Task used to parse the commands received by Tetris
******************************************************************************/
void Task_Console(void *pvParameters);

/*****************************************************
 * Displays the points earned when one or more
 * rows are cleared
 *****************************************************/
void displayPointsEarned(uint8_t value);

/*****************************************************
 * Displays the final score and game over details
 *****************************************************/
void displayGameOver(void);

#endif /* TASK_CONSOLE_H_ */
