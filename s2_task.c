/*
 * task_s2.c
 */

#include <main.h>

TaskHandle_t Task_S2_Handle = NULL;

/****************************************************************
 * This function initializes the hardware for Task_S2
 *
 * PARAMS
 *  none
 ***************************************************************/
void Task_S2_Init(void)
{
    // Set P3.5 to be a GPIO input pin
    P3 -> DIR &= ~BIT5;
    // Enable resistor
    P3 -> REN |= BIT5;
    // Set resistor to pullup resistor
    P3 -> OUT |= BIT5;

}

/****************************************************************
 * This function checks if the S2 button has been pressed
 * on the MKII
 *
 * PARAMS
 *  none
 *
 * RETURNS
 *  true     if S1 has been pressed
 *  false    if S1 has not been pressed
 ***************************************************************/
bool S2_Pressed(void)
{
    if ((P3 -> IN & BIT5) == 0)
    {
        return true;
    } else {
        return false;
    }
}

/****************************************************************
 * This function debounces the S2 pushbutton
 *
 * PARAMS
 *  none
 *
 * RETURNS
 *  true     button has been pressed (after debounce)
 *  false    button has not been pressed
 ***************************************************************/
bool Debounce_S2(void)
{
    static uint8_t debounce_state = 0x00;

    // Shift the de-bounce variable to the left
    debounce_state = debounce_state << 1;

    // If S1 is being pressed, set the LSBit of debounce_state to a 1;
    if(S2_Pressed())
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
 * This function performs task_s2.
 * If the S2 button has been pressed, then send a message to
 * Queue_Tetris to have the image rotate on the LCD screen.
 ***************************************************************/
void task_s2(void *pvParameters)
{
    BaseType_t ulEventsToProcess;
    TETRIS_MSG_t msg;

    while(1)
    {
        // Debounce S2 pushbutton and check if button has been pressed
        if(Debounce_S2())
        {

            // Create message to send to Queue_Tetris
            msg.cmd = TETRIS_CMD_ROTATE;
            msg.value = 50;

            // Send message to Queue_Tetris
            ulEventsToProcess = xQueueSendToBack(Queue_Tetris, &msg, portMAX_DELAY);
        }

        // Wait 10 mS
        vTaskDelay(pdMS_TO_TICKS(10));
    }

}
