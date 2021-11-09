/*
 * song_task.c
 */

#include <main.h>
#include <song_task.h>

TaskHandle_t Task_Song_Handle = NULL;
SemaphoreHandle_t Sem_Buzzer;

/* **************************************************
 * Sets the PWM period of the Buzzer. The duty cycle
 * will be set to 50%
 *
 * Parameters
 *      ticks_period    :   Period of PWM Pulse
 *
 * Returns
 *      none
 * *************************************************/
void MKII_Buzzer_Init(uint16_t ticks_period)
{
    // Set P2.7 to be a GPIO OUTPUT pin
    P2->DIR |= BIT7;

    // the TIMERA PWM controller will control the buzzer on the MKII
    // P2.7 <--> TA0.4
    P2->SEL0 |= BIT7;
    P2->SEL1 &= ~BIT7;

    // Turn off TA0
    TIMER_A0->CTL = 0;

    // Set the period of the timer
    TIMER_A0->CCR[0] = ticks_period - 1;

    // configure BUZZER Duty Cycle to 50%
    TIMER_A0->CCR[4] = (ticks_period /2) - 1;

    // Configure TA0.4 for RESET/SET Mode
    TIMER_A0->CCTL[4] = TIMER_A_CCTLN_OUTMOD_7;

    // Select the master clock as the timer source
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK;
}

/* **************************************************
 * Turns the Buzzer on
 *
 * Parameters
 *      none
 *
 * Returns
 *      none
 * *************************************************/
void MKII_Buzzer_On(void)
{
    // Clear the current Mode Control Bits
    TIMER_A0->CTL &= ~TIMER_A_CTL_MC_MASK;

    // Set Mode Control to UP and Clear the current count
    TIMER_A0->CTL |= TIMER_A_CTL_MC__UP | TIMER_A_CTL_CLR;
}

/* **************************************************
 * Turns the Buzzer off
 *
 * Parameters
 *      none
 *
 * Returns
 *      none
 * *************************************************/
void MKII_Buzzer_Off(void)
{
    // Turn off the timer
    TIMER_A0->CTL &= ~TIMER_A_CTL_MC_MASK;
}

/* **************************************************
 * Check Buzzer Run Status
 *
 * Parameters
 *      none
 *
 * Returns
 *      true    :   TimerA0 is On
 *      false   :   TimerA0 is Off
 * *************************************************/
bool MKII_Buzzer_Run_Status(void)
{
    if ((TIMER_A0->CTL & TIMER_A_CTL_MC_MASK) == TIMER_A_CTL_MC__STOP)
    {
        return false;
    }
    else
    {
        return true;
    }
}

/********************************************************************
 * This function plays the note A4 for the designated length of time
 *
 * Parameters
 *  length  :   the duration selection for the note
 *              (   1 = eighth note
 *                  2 = quarter note
 *                  3 = dotted quarter note )
 *
 * Returns
 *  none
 *******************************************************************/
void note_A4(int length) {
    // Turn off buzzer if already on
    if(MKII_Buzzer_Run_Status() == true)
        MKII_Buzzer_Off();

    // Calculate the pwm_count based on the pitch frequency
    uint16_t pwm_count = (uint16_t)(SystemCoreClock/A4) - 1;
    // Set buzzer to calculated pitch
    MKII_Buzzer_Init(pwm_count);
    // Turn buzzer on
    MKII_Buzzer_On();

    // Hold buzzer on for duration of note
    switch(length) {
    case 1 :
        vTaskDelay(pdMS_TO_TICKS(EIGHTH_NOTE));
        break;
    case 2 :
        vTaskDelay(pdMS_TO_TICKS(QUARTER_NOTE));
        break;
    case 3 :
        vTaskDelay(pdMS_TO_TICKS(DOTTED_QUARTER_NOTE));
        break;
    }

    // Turn buzzer off
    MKII_Buzzer_Off();
    // Wait 2 mS to create slight separation between notes
    vTaskDelay(pdMS_TO_TICKS(2));
}

/********************************************************************
 * This function plays the note B4 for the designated length of time
 *
 * Parameters
 *  length  :   the duration selection for the note
 *              (   1 = eighth note
 *                  2 = quarter note
 *                  3 = dotted quarter note )
 *
 * Returns
 *  none
 *******************************************************************/
void note_B4(int length) {
    // Turn off buzzer if already on
    if(MKII_Buzzer_Run_Status() == true)
        MKII_Buzzer_Off();

    // Calculate the pwm_count based on the pitch frequency
    uint16_t pwm_count = (uint16_t)(SystemCoreClock/B4) - 1;
    // Set buzzer to calculated pitch
    MKII_Buzzer_Init(pwm_count);
    // Turn buzzer on
    MKII_Buzzer_On();

    // Hold buzzer on for duration of note
    switch(length) {
    case 1 :
        vTaskDelay(pdMS_TO_TICKS(EIGHTH_NOTE));
        break;
    case 2 :
        vTaskDelay(pdMS_TO_TICKS(QUARTER_NOTE));
        break;
    case 3 :
        vTaskDelay(pdMS_TO_TICKS(DOTTED_QUARTER_NOTE));
        break;
    }

    // Turn buzzer off
    MKII_Buzzer_Off();
    // Wait 2 mS to create slight separation between notes
    vTaskDelay(pdMS_TO_TICKS(2));
}

/********************************************************************
 * This function plays the note C5 for the designated length of time
 *
 * Parameters
 *  length  :   the duration selection for the note
 *              (   1 = eighth note
 *                  2 = quarter note
 *                  3 = dotted quarter note )
 *
 * Returns
 *  none
 *******************************************************************/
void note_C5(int length) {
    // Turn off buzzer if already on
    if(MKII_Buzzer_Run_Status() == true)
        MKII_Buzzer_Off();

    // Calculate the pwm_count based on the pitch frequency
    uint16_t pwm_count = (uint16_t)(SystemCoreClock/C5) - 1;
    // Set buzzer to calculated pitch
    MKII_Buzzer_Init(pwm_count);
    // Turn buzzer on
    MKII_Buzzer_On();

    // Hold buzzer on for duration of note
    switch(length) {
    case 1 :
        vTaskDelay(pdMS_TO_TICKS(EIGHTH_NOTE));
        break;
    case 2 :
        vTaskDelay(pdMS_TO_TICKS(QUARTER_NOTE));
        break;
    case 3 :
        vTaskDelay(pdMS_TO_TICKS(DOTTED_QUARTER_NOTE));
        break;
    }

    // Turn buzzer off
    MKII_Buzzer_Off();
    // Wait 2 mS to create slight separation between notes
    vTaskDelay(pdMS_TO_TICKS(2));
}

/********************************************************************
 * This function plays the note D5 for the designated length of time
 *
 * Parameters
 *  length  :   the duration selection for the note
 *              (   1 = eighth note
 *                  2 = quarter note
 *                  3 = dotted quarter note )
 *
 * Returns
 *  none
 *******************************************************************/
void note_D5(int length) {
    // Turn off buzzer if already on
    if(MKII_Buzzer_Run_Status() == true)
        MKII_Buzzer_Off();

    // Calculate the pwm_count based on the pitch frequency
    uint16_t pwm_count = (uint16_t)(SystemCoreClock/D5) - 1;
    // Set buzzer to calculated pitch
    MKII_Buzzer_Init(pwm_count);
    // Turn buzzer on
    MKII_Buzzer_On();

    // Hold buzzer on for duration of note
    switch(length) {
    case 1 :
        vTaskDelay(pdMS_TO_TICKS(EIGHTH_NOTE));
        break;
    case 2 :
        vTaskDelay(pdMS_TO_TICKS(QUARTER_NOTE));
        break;
    case 3 :
        vTaskDelay(pdMS_TO_TICKS(DOTTED_QUARTER_NOTE));
        break;
    }

    // Turn buzzer off
    MKII_Buzzer_Off();
    // Wait 2 mS to create slight separation between notes
    vTaskDelay(pdMS_TO_TICKS(2));
}

/********************************************************************
 * This function plays the note E5 for the designated length of time
 *
 * Parameters
 *  length  :   the duration selection for the note
 *              (   1 = eighth note
 *                  2 = quarter note
 *                  3 = dotted quarter note )
 *
 * Returns
 *  none
 *******************************************************************/
void note_E5(int length) {
    // Turn off buzzer if already on
    if(MKII_Buzzer_Run_Status() == true)
        MKII_Buzzer_Off();

    // Calculate the pwm_count based on the pitch frequency
    uint16_t pwm_count = (uint16_t)(SystemCoreClock/E5) - 1;
    // Set buzzer to calculated pitch
    MKII_Buzzer_Init(pwm_count);
    // Turn buzzer on
    MKII_Buzzer_On();

    // Hold buzzer on for duration of note
    switch(length) {
    case 1 :
        vTaskDelay(pdMS_TO_TICKS(EIGHTH_NOTE));
        break;
    case 2 :
        vTaskDelay(pdMS_TO_TICKS(QUARTER_NOTE));
        break;
    case 3 :
        vTaskDelay(pdMS_TO_TICKS(DOTTED_QUARTER_NOTE));
        break;
    }

    // Turn buzzer off
    MKII_Buzzer_Off();
    // Wait 2 mS to create slight separation between notes
    vTaskDelay(pdMS_TO_TICKS(2));
}

/********************************************************************
 * This function plays the note F5 for the designated length of time
 *
 * Parameters
 *  length  :   the duration selection for the note
 *              (   1 = eighth note
 *                  2 = quarter note
 *                  3 = dotted quarter note )
 *
 * Returns
 *  none
 *******************************************************************/
void note_F5(int length) {
    // Turn off buzzer if already on
    if(MKII_Buzzer_Run_Status() == true)
        MKII_Buzzer_Off();

    // Calculate the pwm_count based on the pitch frequency
    uint16_t pwm_count = (uint16_t)(SystemCoreClock/F5) - 1;
    // Set buzzer to calculated pitch
    MKII_Buzzer_Init(pwm_count);
    // Turn buzzer on
    MKII_Buzzer_On();

    // Hold buzzer on for duration of note
    switch(length) {
    case 1 :
        vTaskDelay(pdMS_TO_TICKS(EIGHTH_NOTE));
        break;
    case 2 :
        vTaskDelay(pdMS_TO_TICKS(QUARTER_NOTE));
        break;
    case 3 :
        vTaskDelay(pdMS_TO_TICKS(DOTTED_QUARTER_NOTE));
        break;
    }

    // Turn buzzer off
    MKII_Buzzer_Off();
    // Wait 2 mS to create slight separation between notes
    vTaskDelay(pdMS_TO_TICKS(2));
}

/********************************************************************
 * This function plays the note G5 for the designated length of time
 *
 * Parameters
 *  length  :   the duration selection for the note
 *              (   1 = eighth note
 *                  2 = quarter note
 *                  3 = dotted quarter note )
 *
 * Returns
 *  none
 *******************************************************************/
void note_G5(int length) {
    // Turn off buzzer if already on
    if(MKII_Buzzer_Run_Status() == true)
        MKII_Buzzer_Off();

    // Calculate the pwm_count based on the pitch frequency
    uint16_t pwm_count = (uint16_t)(SystemCoreClock/G5) - 1;
    // Set buzzer to calculated pitch
    MKII_Buzzer_Init(pwm_count);
    // Turn buzzer on
    MKII_Buzzer_On();

    // Hold buzzer on for duration of note
    switch(length) {
    case 1 :
        vTaskDelay(pdMS_TO_TICKS(EIGHTH_NOTE));
        break;
    case 2 :
        vTaskDelay(pdMS_TO_TICKS(QUARTER_NOTE));
        break;
    case 3 :
        vTaskDelay(pdMS_TO_TICKS(DOTTED_QUARTER_NOTE));
        break;
    }

    // Turn buzzer off
    MKII_Buzzer_Off();
    // Wait 2 mS to create slight separation between notes
    vTaskDelay(pdMS_TO_TICKS(2));
}

/********************************************************************
 * This function plays the note A5 for the designated length of time
 *
 * Parameters
 *  length  :   the duration selection for the note
 *              (   1 = eighth note
 *                  2 = quarter note
 *                  3 = dotted quarter note )
 *
 * Returns
 *  none
 *******************************************************************/
void note_A5(int length) {
    // Turn off buzzer if already on
    if(MKII_Buzzer_Run_Status() == true)
        MKII_Buzzer_Off();

    // Calculate the pwm_count based on the pitch frequency
    uint16_t pwm_count = (uint16_t)(SystemCoreClock/A5) - 1;
    // Set buzzer to calculated pitch
    MKII_Buzzer_Init(pwm_count);
    // Turn buzzer on
    MKII_Buzzer_On();

    // Hold buzzer on for duration of note
    switch(length) {
    case 1 :
        vTaskDelay(pdMS_TO_TICKS(EIGHTH_NOTE));
        break;
    case 2 :
        vTaskDelay(pdMS_TO_TICKS(QUARTER_NOTE));
        break;
    case 3 :
        vTaskDelay(pdMS_TO_TICKS(DOTTED_QUARTER_NOTE));
        break;
    }

    // Turn buzzer off
    MKII_Buzzer_Off();
    // Wait 2 mS to create slight separation between notes
    vTaskDelay(pdMS_TO_TICKS(2));
}

/********************************************************************
 * This function turns off the buzzer and waits for the
 * designated length of time (musical rest)
 *
 * Parameters
 *  length  :   the duration selection for the rest
 *              (   1 = eighth rest
 *                  2 = quarter rest    )
 *
 * Returns
 *  none
 *******************************************************************/
void rest(int length) {
    // Turn off buzzer if already on
    if(MKII_Buzzer_Run_Status() == true)
        MKII_Buzzer_Off();

    // Hold rest for duration of rest
    switch(length) {
    case 1 :
        vTaskDelay(pdMS_TO_TICKS(EIGHTH_NOTE));
        break;
    case 2 :
        vTaskDelay(pdMS_TO_TICKS(QUARTER_NOTE));
        break;
    }

    // Wait 2 mS to keep notes and rests consistent in total duration
    vTaskDelay(pdMS_TO_TICKS(2));
}

/********************************************************************
 * This function performs the Task_Song.
 * It plays the theme song from Tetris on repeat while there is
 * power flowing through the board :)
 * Try not to get too annoyed
 *******************************************************************/
void Task_Song(void *pvParameters)
{
    BaseType_t ulEventsToProcess;
    TETRIS_MSG_t msg;

    while(1)
    {
        // play tetris theme song
        // 120 bmp
        // 4/4 time
        note_E5(2);
        note_B4(1);
        note_C5(1);
        note_D5(2);
        note_C5(1);
        note_B4(1);

        note_A4(3);
        note_C5(1);
        note_E5(2);
        note_D5(1);
        note_C5(1);

        note_B4(3);
        note_C5(1);
        note_D5(2);
        note_E5(2);

        note_C5(2);
        note_A4(2);
        note_A4(2);
        rest(2);

        rest(1);
        note_C5(2);
        note_E5(1);
        note_A5(2);
        note_G5(1);
        note_F5(1);

        note_E5(3);
        note_C5(1);
        note_E5(2);
        note_D5(1);
        note_C5(1);

        note_B4(3);
        note_C5(1);
        note_D5(2);
        note_E5(2);

        note_C5(2);
        note_A4(2);
        note_A4(2);

        rest(2);
        rest(2);

    }

}
