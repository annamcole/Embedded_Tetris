#include <main.h>
#include <task_tetris.h>


 TaskHandle_t Task_Accelerometer_Handle;
 TaskHandle_t Task_Accelerometer_Timer_Handle;

 volatile uint32_t ACCELEROMETER_X_DIR = 0;
 volatile uint32_t ACCELEROMETER_Y_DIR = 0;
 volatile uint32_t ACCELEROMETER_Z_DIR = 0;

/******************************************************************************
* Configure the IO pins for the X,Y, and Z directions of the analog
* accelerometer.
* The X direction should be configured to place the results in MEM[0].
* The Y direction should be configured to place the results in MEM[1].
* The Z direction should be configured to place the results in MEM[2].
*
* After ALL analog signals have finished being converted, a SINGLE interrupt
* should be generated.
*
* Parameters
*      None
* Returns
*      None
******************************************************************************/
 void Task_Accelerometer_Init(void)
 {
     // Configure the X direction as an analog input pin.
     P6->SEL0 |= BIT1;
     P6->SEL1 |= BIT1;

     // Configure the Y direction as an analog input pin.
     P4->SEL0 |= BIT0;
     P4->SEL1 |= BIT0;

     // configure the Z direction as an analog input pin.
     P4->SEL0 |= BIT2;
     P4->SEL1 |= BIT2;

     // Configure CTL0 to sample 16-times in pulsed sample mode.
     // NEW -- Indicate that this is a sequence of samples.
     ADC14->CTL0 = ADC14_CTL0_SHT0_2 | ADC14_CTL0_SHP | ADC14_CTL0_CONSEQ_1;

     // Configure CTL1 to return 12-bit values
     ADC14->CTL1 = ADC14_CTL1_RES_2;         // Use sampling timer, 12-bit conversion results

     // Associate the X direction analog signal with MEM[0]
     ADC14->MCTL[0] |= ADC14_MCTLN_INCH_14;

     // Associate the Y direction analog signal with MEM[1]
     ADC14->MCTL[1] |= ADC14_MCTLN_INCH_13;

     // Associate the Z direction analog signal with MEM[2]
     //Make sure to indicate this is the end of a sequence.
     ADC14->MCTL[2] |= ADC14_MCTLN_INCH_11 | ADC14_MCTLN_EOS;

     // Enable interrupts AFTER a value is written into MEM[2].
     ADC14->IER0 |= ADC14_IER0_IE1 ;

     // Enable ADC Interrupt
     NVIC_EnableIRQ(ADC14_IRQn);

     // Interrupt priorities must NOT be set to 0.  It is preferred if they are set to a value that is > than 1 too.
     NVIC_SetPriority(ADC14_IRQn,2);

     // Turn ADC ON
     ADC14->CTL0 |= ADC14_CTL0_ON;

 }

 /******************************************************************************
 * Used to start an ADC14 Conversion
 ******************************************************************************/
 void Task_Accelerometer_Timer(void *pvParameters)
 {
     while(1)
     {
         /*
          * Start the ADC conversion
          */
         ADC14->CTL0 |= ADC14_CTL0_SC | ADC14_CTL0_ENC;

         /*
          * Delay 5mS
          */
         vTaskDelay(pdMS_TO_TICKS(5));

     }
 }


/******************************************************************************
* Bottom Half Task.  Examines the ADC data from the accelerometer on the MKII
******************************************************************************/
void Task_Accelerometer_Bottom_Half(void *pvParameters)
{
    TETRIS_MSG_t dir;
    uint32_t ulEventsToProcess;
    bool backToCenter = false;

    while(1)
    {
        /*
         * Wait until we get a task notification from the ADC14 ISR
         */
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);



        /*
         * Set the dir.cmd field to one of the following values based
         * on the values of ACCELEROMETER_X_DIR, and set the
         * value to 12, since we only want the image to move 1 column at a time.
         * Then send the message to the queue. Don't send message to queue if
         * accelerometer is in center
         */
        if(ACCELEROMETER_X_DIR < VOLT_0P99)
        {
           if (backToCenter)
           {
               dir.cmd = TETRIS_CMD_LEFT;
               dir.value = 12;
               ulEventsToProcess = xQueueSendToBack(Queue_Tetris, &dir, portMAX_DELAY);

               backToCenter = false;
           }
        }
        else if(ACCELEROMETER_X_DIR > VOLT_2P31)
        {
            if (backToCenter)
            {
                dir.cmd = TETRIS_CMD_RIGHT;
                dir.value = 12;
                ulEventsToProcess = xQueueSendToBack(Queue_Tetris, &dir, portMAX_DELAY);

                backToCenter = false;
            }
        }
        else
        {
            // no message is sent if accelerometer is in center
            // position
            backToCenter = true;
        }
    }
}


/******************************************************************************
* Top Half of ADC14 Handler.
******************************************************************************/
void ADC14_IRQHandler(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    ACCELEROMETER_X_DIR = ADC14->MEM[0]; // Read the value and clear the interrupt
    ACCELEROMETER_Y_DIR = ADC14->MEM[1]; // Read the value and clear the interrupt
    ACCELEROMETER_Z_DIR = ADC14->MEM[2]; // Read the value and clear the interrupt


    /*
     * Send a task notification to Task_Accelerometer_Bottom_Half
     */
    vTaskNotifyGiveFromISR(
            Task_Accelerometer_Handle,
            &xHigherPriorityTaskWoken
    );

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}



