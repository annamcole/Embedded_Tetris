/*
 * task_console.c
 */
#include <main.h>

QueueHandle_t Queue_Console;
TaskHandle_t Task_Console_Handle;
SemaphoreHandle_t Sem_Console;

/******************************************************************************
 * This function configures the eUSCI_A0 to be a UART that communicates at
 * 115200 8N1.
 *
 * The configuration should be identical to what you have done
 * in a previous ICE-08 EXCEPT that you will need to set the priority of the
 * eUSCI to be a value that is equal to 2
 ******************************************************************************/
static void console_init(void)
{
    // Configure the IO pins used for the UART
    // set 2-UART pin as secondary function
    P1->SEL0 |= BIT2 | BIT3;
    P1->SEL1 &= ~(BIT2 | BIT3);

    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST; //Put eUSCI in reset
    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST | // remain eUSCI in reset
                EUSCI_B_CTLW0_SSEL__SMCLK; // Configure eUSCI clock source

    // Baud Rate Calculation
    // 24000000/(16*115200) = 13.020833333
    // Fractional portion = 0.0208333333
    // UCBRFx = int ( (13.020833333-13)*16) = 0
    EUSCI_A0->BRW = 13;             // 24000000/16/115200

    // Set the fractional portion of the baud rate and turn
    // an oversampling
    EUSCI_A0->MCTLW = (0 << EUSCI_A_MCTLW_BRF_OFS) | EUSCI_A_MCTLW_OS16;

    // enable eUSCI in UART mode
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;

    // clear any outstanding interrupts
    EUSCI_A0->IFG &= ~(EUSCI_A_IFG_RXIFG | EUSCI_A_IFG_TXIFG);

    // Enable Rx Interrupt
    EUSCI_A0->IE |= EUSCI_A_IE_RXIE;

    // Enable EUSCIA0 Interrupt
    NVIC_EnableIRQ(EUSCIA0_IRQn);
    NVIC_SetPriority(EUSCIA0_IRQn,2);

    // Prime the pump
    EUSCI_A0->TXBUF = 0;
}

/******************************************************************************
 * This function initializes the eUSCI_A0 hardware by calling console_hw_init().
 * It will also initialize Sem_Console.
 *
 * NOTE:  This function must be run prior to the FreeRTOS scheduler being
 * started.
 ******************************************************************************/
void Task_Console_Init(void)
{
    // Initialize UART
    console_init();

    Queue_Console = xQueueCreate(2,sizeof(CONSOLE_MSG_t));

    // Initialize the binary semaphore used to provide mutual exclusion to
    // the UART.
    Sem_Console = xSemaphoreCreateBinary();

}

/*****************************************************
 * Needed to get printf to work using polling when
 * transmitting characters.
 *****************************************************/
int fputc(int c, FILE* stream)
{
    // while UART is busy, wait
    while(EUSCI_A0->STATW & EUSCI_A_STATW_BUSY){};

    EUSCI_A0->TXBUF = c;

    return 0;
}

/*****************************************************
 * Displays the points earned when one or more
 * rows are cleared
 *
 * PARAMS
 *  value   :   number of rows cleared
 *
 * RETURNS
 *  none
 *****************************************************/
void displayPointsEarned(uint8_t value) {
    int points;
    char str[25];

    // convert # rows cleared to points
    if(value == 1)
        points = 40;
    else if(value == 2)
        points = 100;
    else if(value == 3)
        points = 300;
    else if(value == 4)
        points = 1200;
    else
        points = value * 500;

    // print value of points
    sprintf(str, "+%d\n\r", points);
    printf(str);
    printf("\n\r");

}

/*****************************************************
 * Displays the final score and game over details
 *****************************************************/
void displayGameOver(void) {

    char str[25];

    printf("***************************\n\r");
    printf("         GAME OVER!\n\r");
    printf("***************************\n\r");
    printf("FINAL SCORE: ");
    sprintf(str, "%d", SCORE);
    printf(str);
    printf("\n\r");
    printf("***************************\n\r");
    printf("Press S1 To Restart\n\r");
    printf("***************************\n\r");

}

/******************************************************************************
* Task used to parse the commands received Tetris
******************************************************************************/
void Task_Console(void *pvParameters)
{
    BaseType_t status;
    CONSOLE_MSG_t msg;

    while(1) {

        // wait for notification

        // get command from queue
        status = xQueueReceive(Queue_Console, &msg, portMAX_DELAY);

        if(msg.cmd == CONSOLE_CMD_CLEAR_ROW) {
            displayPointsEarned(msg.value);
        } else if (msg.cmd == CONSOLE_CMD_GAME_OVER) {
            displayGameOver();
        } else {
            // do nothing
        }
    }
}


