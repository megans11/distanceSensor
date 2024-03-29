/*
 * Copyright (c) 2016-2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== main_freertos.c ========
 */
#include <stdint.h>


#ifdef __ICCARM__
#include <DLib_Threads.h>
#endif

/* POSIX Header files */
#include <pthread.h>

/* RTOS header files */
#include <FreeRTOS.h>
#include <task.h>
#include "test_thread.h"
#include <find_distance_task_thread.h>
#include "main_thread.h"
#include <ti/drivers/SPI.h>

/* Example/Board Header files */
#include <ti/drivers/Board.h>

extern void *sensorMainThread(void *arg0);
extern void *testThread(void *arg0);
extern void *findDistanceTaskThread(void *arg0);
//extern void *mainThread(void *arg0);

/* Stack size in bytes */
#define THREADSTACKSIZE   4096

/*
 *  ======== main ========
 */
int main(void)
{
    pthread_t           thread;
    pthread_attr_t      attrs;
    struct sched_param  priParam;
    int                 retc;

    /* initialize the system locks */
#ifdef __ICCARM__
    __iar_Initlocks();
#endif

    /* Call driver init functions */
    Board_init();

    UART_init();
    GPIO_init();
    Capture_init();
    Timer_init();

    UART_Params_init(&uartDistanceSensorParams);
    uartDistanceSensorParams.writeMode = UART_MODE_BLOCKING;
    uartDistanceSensorParams.writeDataMode = UART_DATA_BINARY;
    uartDistanceSensorParams.baudRate = 115200;
    uartDistanceSensorParams.readEcho = UART_ECHO_OFF;

    /* Setting up the Capture driver to detect two rising edges and report
    * the result in microseconds
    */
    Capture_Params_init(&distanceCaptureParams);
    distanceCaptureParams.mode = Capture_ANY_EDGE;
    distanceCaptureParams.periodUnit = Capture_PERIOD_US;
    distanceCaptureParams.callbackFxn = distanceCaptureCallback;

    // init timer
     Timer_Params_init(&distance_sensor_params);
     distance_sensor_params.period = DISTANCE_SENSOR_TIMER_PERIOD;
     distance_sensor_params.periodUnits = Timer_PERIOD_US;
     distance_sensor_params.timerMode = Timer_CONTINUOUS_CALLBACK;
     distance_sensor_params.timerCallback = distanceSensorTimerCallback;

    /* Initialize the attributes structure with default values */
    pthread_attr_init(&attrs);

    /* Set priority, detach state, and stack size attributes */
    priParam.sched_priority = 1;
    retc = pthread_attr_setschedparam(&attrs, &priParam);
    retc |= pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_DETACHED);
    retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
    if (retc != 0) {
        /* failed to set attributes */
        while (1) {}
    }

    retc = pthread_create(&thread, &attrs, findDistanceTaskThread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1) {}
    }

    retc = pthread_create(&thread, &attrs, sensorMainThread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1) {}
    }

//    retc = pthread_create(&thread, &attrs, mainThread, NULL);
//    if (retc != 0) {
//        /* pthread_create() failed */
//        while (1) {}
//    }

    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler();

    return (0);
}

//*****************************************************************************
//
//! \brief Application defined malloc failed hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationMallocFailedHook()
{
    /* Handle Memory Allocation Errors */
    while(1)
    {
    }
}

//*****************************************************************************
//
//! \brief Application defined stack overflow hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    //Handle FreeRTOS Stack Overflow
    while(1)
    {
    }
}
