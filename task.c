//task.c
#include "common.h"
#include "task.h"

CBTaskEvent g_taskList[TASK_MAX]={0};

int taskFindEmpty()
{
    static int index = -1;

    for(int i=0; i<TASK_MAX; i++)
    {
        index++;
        index %= TASK_MAX;
        if(g_taskList[index]==NULL)
        {
            return index;
        }
    }

    return -1;
}

int taskCreat(CBTaskEvent task)
{
    int taskID;

    taskID=taskFindEmpty();
    if(taskID == -1)
    {
        printf("error:task list is full!\n");
        return -1;
    }

    g_taskList[taskID] = task;

    printf("creat task<%d>\n",taskID);

    return taskID;
}

void taskDestroy(int taskID)
{
    printf("Destroy task<%d>\n",taskID);

    g_taskList[taskID] = NULL;
}

void taskLoop()
{
    taskEventLoop();
    taskTimerLoop();
}

TASK_EVENT g_taskEventList[TASK_EVENT_MAX];
int g_TKEventWrite=0;
int g_TKEventRead=0;

int tkEventGetSize()
{
    return (g_TKEventWrite + TASK_EVENT_MAX - g_TKEventRead)% TASK_EVENT_MAX;
}

void taskEventIssue(int taskID,uint32_t eventID)
{
    int writePos;

    if(taskID >= TASK_EVENT_MAX || taskID < 0)
    {
        printf("taskEventIssue() error:taskID\n");
        return;
    }

    writePos = (g_TKEventWrite + 1)% TASK_EVENT_MAX;

    if(writePos == g_TKEventRead)
    {
        printf("taskEventIssue() error:task<%d> event list is full!\n",taskID);
        return;
    }

    g_taskEventList[g_TKEventWrite].taskID=taskID;
    g_taskEventList[g_TKEventWrite].eventID=eventID;
    g_TKEventWrite=writePos;

    //printf("add event:%x\n",eventID);
}

void taskEventLoop()
{
    TASK_EVENT event;
    CBTaskEvent task;
    int size;

    size=tkEventGetSize();
    while(size-- >0)
    {
        event=g_taskEventList[g_TKEventRead];
        g_TKEventRead = (g_TKEventRead + 1)% TASK_EVENT_MAX;

        task = g_taskList[event.taskID];
        if(!task)
        {
            printf("taskEventLoop() error:task is NULL\n");
            continue;
        }

        task(event.taskID,event.eventID);
    }
}

// 定时、休眠

uint32_t g_taskTicks=0;

uint32_t getTaskTicks()
{
    return g_taskTicks;
}

void taskTicksInc() // 1ms时间基准
{
    g_taskTicks++;
}

uint32_t taskTickDiff(uint32_t now,uint32_t last)
{
    uint64_t diff;
    diff = now + 0x100000000 - last;

    return (diff & 0xffffffff);
}

TASK_TIMER g_taskTimerList[TASK_TIMER_MAX]={0};

int taskTimerFindEmpty()
{
    for(int i=0; i<TASK_TIMER_MAX; i++)
    {
        if(!g_taskTimerList[i].isValid)
        {
            return i;
        }
    }

    return -1;
}

void taskTimer(int taskID,uint32_t eventID,uint32_t time_ms)
{
    int index;

    index=taskTimerFindEmpty();
    if(index==-1)
    {
        printf("taskTimer() error:timer list is full\n");
        return;
    }

    g_taskTimerList[index].taskID=taskID;
    g_taskTimerList[index].eventID=eventID;
    g_taskTimerList[index].timeMs=time_ms;
    g_taskTimerList[index].start=getTaskTicks();
    g_taskTimerList[index].isValid=true;

    printf("add timer:<%d,%x> %ums\n",taskID,eventID,time_ms);

}

void taskTimerLoop()
{
    static uint32_t start=0;
    if(taskTickDiff(getTaskTicks(),start)<3)
    {
        return;
    }

    start=getTaskTicks();

    for(int i=0; i<TASK_TIMER_MAX; i++)
    {
        if(g_taskTimerList[i].isValid)
        {
            if(taskTickDiff(start,g_taskTimerList[i].start)>=g_taskTimerList[i].timeMs)
            {
                taskEventIssue(g_taskTimerList[i].taskID,g_taskTimerList[i].eventID);
                g_taskTimerList[i].isValid=false;
            }
        }
    }
}
