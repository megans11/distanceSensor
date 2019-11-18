/*
 * testThread.h
 *
 */

#ifndef TEST_THREAD_H_
#define TEST_THREAD_H_

/* For usleep() */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>

#include <ti/drivers/GPIO.h>

#include "Board.h"
#include "find_distance_task_thread.h"
#include "my_callback_files/distance_sensor_timer.h"
#include "my_driver_files/distance_sensor_driver.h"

#include "my_queue_files/distance_task_queue.h"

#define TEST_DISTANCE_QUEUE 0
#define TEST_DISTANCE_DRIVER 0
#define TEST_DISTANCE_THREAD 1
#define TEST_ALL_QUEUES 0

void *testThread(void *arg0);

#endif /* TEST_THREAD_H_ */
