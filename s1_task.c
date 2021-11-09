/*
 * s1_task.c
 */

#include <main.h>

TaskHandle_t Task_S1_Handle = NULL;

/****************************************************************
 * This function initializes the hardware for Task_S1
 *
 * PARAMS
 *  none
 ***************************************************************/
void Task_S1_Init(void)
{
    // Set P5.1 to be a GPIO input pin
    P5 -> DIR &= ~BIT1;
    // Enable resistor
    P5 -> REN |= BIT1;
    // Set resistor to pullup resistor
    P5 -> OUT |= BIT1;

}

/****************************************************************
 * This function checks if the S1 button has been pressed
 * on the MKII
 *
 * PARAMS
 *  none
 *
 * RETURNS
 *  true     if S1 has been pressed
 *  false    if S1 has not been pressed
 ***************************************************************/
bool S1_Pressed(void)
{
    if ((P5 -> IN & BIT1) == 0)
    {
        return true;
    } else {
        return false;
    }
}

/****************************************************************
 * This function debounces the S1 pushbutton
 *
 * PARAMS
 *  none
 *
 * RETURNS
 *  true     button has been pressed (after debounce)
 *  false    button has not been pressed
 ***************************************************************/
bool Debounce_S1(void)
{
    static uint8_t debounce_state = 0x00;

    // Shift the de-bounce variable to the left
    debounce_state = debounce_state << 1;

    // If S1 is being pressed, set the LSBit of debounce_state to a 1;
    if(S1_Pressed())
    {
        debounce_state |= 0x01;
    }

    // If the de-bounce variable is equal to 0x7F, change the color of the tri-color LED.
    if(debounce_state == 0x7F)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/****************************************************************
 * This function performs task_s1.
 * If the S1 button has been pressed, then send a message to
 * Queue_Tetris to have the image fall down the LCD screen.
 ***************************************************************/
void task_s1(void *pvParameters)
{
    BaseType_t ulEventsToProcess;
    TETRIS_MSG_t msg;

    while(1)
    {
        // Debounce S1 pushbutton and check if button has been pressed
        if(Debounce_S1())
        {

            // Create message to send to Queue_Tetris
            msg.cmd = TETRIS_CMD_FALL;
            msg.value = 12;

            // Send message to Queue_Tetris
            ulEventsToProcess = xQueueSendToBack(Queue_Tetris, &msg, portMAX_DELAY);

        }

        // Wait 10 mS
        vTaskDelay(pdMS_TO_TICKS(10));
    }

}
