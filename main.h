/*
 * main.h
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "msp.h"
#include "msp432p401r.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* RTOS header files */
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

/* ECE353 Includes */
#include <task_accelerometer.h>
#include <s1_task.h>
#include <s2_task.h>
#include <task_timer.h>
#include <task_console.h>
#include <song_task.h>
#include <task_tetris.h>

#include <lcd.h>

/* BITMAPS */
#include "game_over.h"
#include "L.h"
#include "L_1.h"
#include "L_2.h"
#include "L_3.h"
#include "Line_horizontal.h"
#include "Line_vertical.h"
#include "S_horizontal.h"
#include "single.h"
#include "Square.h"
#include "T.h"
#include "T_1.h"
#include "T_2.h"
#include "T_3.h"
#include "Z_horizontal.h"

#endif /* MAIN_H_ */
