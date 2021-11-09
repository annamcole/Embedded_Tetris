/*
 * song_task.h
 */

#ifndef SONG_TASK_H_
#define SONG_TASK_H_

#include <main.h>

/* Frequency of Respective Notes */
#define A4   440
#define B4   494
#define C5   523
#define D5   587
#define E5   659
#define F5   698
#define G5   784
#define A5   880

/* Length in mS of each note */
#define EIGHTH_NOTE         250
#define QUARTER_NOTE        500
#define DOTTED_QUARTER_NOTE 750

extern TaskHandle_t Task_Song_Handle;

/********************************************************************
 * This function initializes and sets up the hardware
 * for the Piezo Buzzer
 *******************************************************************/
void MKII_Buzzer_Init(uint16_t ticks_period);

/********************************************************************
 * This function turns on the buzzer
 *******************************************************************/
void MKII_Buzzer_On(void);

/********************************************************************
 * This function turns off the buzzer
 *******************************************************************/
void MKII_Buzzer_Off(void);

/********************************************************************
 * This function checks whether the buzzer is currently on or off
 *******************************************************************/
bool MKII_Buzzer_Run_Status(void);

/********************************************************************
 * This function plays the note A4 for the designated length of time
 *******************************************************************/
void note_A4(int length);

/********************************************************************
 * This function plays the note B4 for the designated length of time
 *******************************************************************/
void note_B4(int length);

/********************************************************************
 * This function plays the note C5 for the designated length of time
 *******************************************************************/
void note_C5(int length);

/********************************************************************
 * This function plays the note D5 for the designated length of time
 *******************************************************************/
void note_D5(int length);

/********************************************************************
 * This function plays the note E5 for the designated length of time
 *******************************************************************/
void note_E5(int length);

/********************************************************************
 * This function plays the note F5 for the designated length of time
 *******************************************************************/
void note_F5(int length);

/********************************************************************
 * This function plays the note G5 for the designated length of time
 *******************************************************************/
void note_G5(int length);

/********************************************************************
 * This function plays the note A5 for the designated length of time
 *******************************************************************/
void note_A5(int length);

/********************************************************************
 * This function turns off the buzzer and waits for the
 * designated length of time (musical rest)
 *******************************************************************/
void rest(int length);

/********************************************************************
 * This function performs the Task_Song.
 * It plays the theme song from Tetris on repeat while there is
 * power flowing through the board :)
 * Try not to get too annoyed
 *******************************************************************/
void Task_Song(void *pvParameters);

#endif /* S1_TASK_H_ */
