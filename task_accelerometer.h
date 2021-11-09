#ifndef TASK_ACCELEROMETER_H_
#define TASK_ACCELEROMETER_H_

#include "main.h"

#define VOLT_0P99  1228      // 0.99 /(3.3/4096)
#define VOLT_2P31  2868      // 2.31 /(3.3/4096)

extern TaskHandle_t Task_Accelerometer_Handle;
extern TaskHandle_t Task_Accelerometer_Timer_Handle;

/******************************************************************************
* Configure the IO pins for the X,Y, and Z directions of the analog
* accelerometer.
* The X direction should be configured to place the results in MEM[0].
* The Y direction should be configured to place the results in MEM[1].
* The Z direction should be configured to place the results in MEM[2].
*
* After ALL analog signals have finished being converted, a SINGLE interrupt
* should be generated.
******************************************************************************/
 void Task_Accelerometer_Init(void);

/******************************************************************************
* Used to start an ADC14 Conversion
******************************************************************************/
void Task_Accelerometer_Timer(void *pvParameters);

/******************************************************************************
* Examines the ADC data from the accelerometer on the MKII
******************************************************************************/
void Task_Accelerometer_Bottom_Half(void *pvParameters);

#endif /* TASK_ACCELEROMETER_H_ */
