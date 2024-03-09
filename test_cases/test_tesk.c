/test_task.h
#ifndef _TEST_THREAD_H
#define _TEST_THREAD_H

void testInit();
void testLoop();

#endif // 
//test_task.c
#include "common.h"
#include "task.h"

#define CTRL_EVENT1 0x01
#define CTRL_EVENT2 0x02
#define CTRL_EVENT3 0x04

void eventProcess(int taskID,uint32_t event)
{
    switch(event)
    {
        case CTRL_EVENT1:
            printf("task[%d] CTRL_EVENT1\n",taskID);
            //taskEventIssue(taskID,CTRL_EVENT2);
            taskTimer(taskID,CTRL_EVENT2,1000);
            break;

        case CTRL_EVENT2:
            printf("task[%d] CTRL_EVENT2\n",taskID);
            //taskEventIssue(taskID,CTRL_EVENT3);
            taskTimer(taskID,CTRL_EVENT3,2000);
            break;

        case CTRL_EVENT3:
            printf("task[%d] CTRL_EVENT3\n",taskID);
            taskTimer(taskID,CTRL_EVENT1,4000);
            break;

        default:
            break;
    }
}

void testLoop()
{
    taskLoop();
}

void testInit()
{
    int taskID1,taskID2;

    printf("testInit()\n");

    taskID1 = taskCreat((CBTaskEvent)&eventProcess);

    taskTimer(taskID1,CTRL_EVENT1,5000);

    taskID2 = taskCreat((CBTaskEvent)&eventProcess);
    taskEventIssue(taskID2,CTRL_EVENT2);
    taskDestroy(taskID1);
    taskDestroy(taskID2);
    //taskEventIssue(taskID1,CTRL_EVENT1);
    taskID1 = taskCreat((CBTaskEvent)&eventProcess);
    taskEventIssue(taskID1,CTRL_EVENT1);
}
