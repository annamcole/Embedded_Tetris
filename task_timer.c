/*
 * task_timer.c
 */

#include <main.h>

TaskHandle_t Task_Timer_Handle;

/*****************************************************
 * Sets the period at which to trigger an interrupt
 * for T32_1
 *
 * Parameters
 *      ms    :   Period of T32_1 Interrupt
 * Returns
 *      None
 *****************************************************/
void timer1_task_init(int ms)
{
    //ticks = desired period / core clock period
    //20e-3/(1/3e6) = (3e6 * 20)/1000
    uint32_t ticks = ((SystemCoreClock * ms)/1000) - 1;

    //Stop the timer
    TIMER32_1 -> CONTROL = 0;

    //Set the load register
    TIMER32_1 -> LOAD = ticks;

    //Enable the Timer32 interrupt in NVIC
    __enable_irq();
    NVIC_EnableIRQ(T32_INT1_IRQn);
    NVIC_SetPriority(T32_INT1_IRQn, 2);

    //Start Timer32 and enable interrupt
    TIMER32_1 -> CONTROL = TIMER32_CONTROL_ENABLE |     //turn timer on
                           TIMER32_CONTROL_MODE |       //periodic mode
                           TIMER32_CONTROL_SIZE |       //32 bit timer
                           TIMER32_CONTROL_IE;         //enable interrupts

}

/*****************************************************
 * Interrupt handler for T32_1
 *
 * Parameters
 *      None
 * Returns
 *      None
 *****************************************************/
void T32_INT1_IRQHandler(void)
{

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /*
     * Send a task notification to Task_Timer_Bottom_Half
     */
    vTaskNotifyGiveFromISR(
            Task_Timer_Handle,
            &xHigherPriorityTaskWoken
    );

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

    // Start the ADC conversion
    //ADC14 -> CTL0 |= ADC14_CTL0_SC | ADC14_CTL0_ENC;

    // Clear the timer interrupt
    TIMER32_1 -> INTCLR = BIT0;
}

/******************************************************************************
* Bottom Half Task.  Examines the ADC data from the joy stick on the MKII
******************************************************************************/
void Task_Timer_Bottom_Half(void *pvParameters)
{
    TETRIS_MSG_t dir;
    uint32_t ulEventsToProcess;


    while(1)
    {
        /*
         * Wait until we get a task notification from the ADC14 ISR
         */
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Wait 1000 mS
        vTaskDelay(pdMS_TO_TICKS(1000));

        // Create message
        dir.cmd = TETRIS_CMD_DROP;
        dir.value = 12;
        // Send message
        ulEventsToProcess = xQueueSendToBack(Queue_Tetris, &dir, portMAX_DELAY);

    }
}
