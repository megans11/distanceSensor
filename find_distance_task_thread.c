/*
 *  ======== find_color_task_thread.c ========
 */

#include <find_distance_task_thread.h>

/*
 *  ======== findColorTaskThread ========
 */
void *findDistanceTaskThread(void *arg0)
{

    init_distance_sensor_UART();

//    const char  echoPrompt[] = "Echoing characters:\r\n";
//    UART_write(distanceSensorUART, echoPrompt, sizeof(echoPrompt));

    if (create_DistanceTaskQueue() == CREATE_QUEUE_FAILURE)
    {
//        errorRoutine(CONTROL_QUEUE_CREATE_FAILURE);
//        printf("Queue creation failed.\n");
    }
    else
    {
//        printf("Queue creation succeeded\n");
    }

    // Command variables
    int msg_type;
    long msg_buffer;
    static long myRiseTime = 0;
    static long myFallTime = 0;
    static long distance = 0;

    int looping = 0;

    while (1) {

        msg_type = readMsg_DistanceTaskQueue(&msg_buffer);
        if (msg_type == MSG_TYPE_RISE)
        {
            myRiseTime = msg_buffer;
        }
        if (msg_type == MSG_TYPE_FALL)
        {
            myFallTime = msg_buffer;
            distance = myFallTime - myRiseTime;
            distance = distance / 1000; //turning nanoseconds into microseconds
            float cmDistance= (distance * .0343) / 2;

            char buffer[17];
            snprintf(buffer, sizeof(buffer), "\r\n distance %ld", distance);
            UART_write(distanceSensorUART, buffer, sizeof(buffer));
        }

        //print to UART

    }
}
