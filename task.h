//task.h
#ifndef _THREAD_H
#define _THREAD_H

#define TASK_MAX 20 // �����������
#define TASK_EVENT_MAX 100 // ������г���
#define TASK_TIMER_MAX 100 // ��ʱ���������

typedef void (*CBTaskEvent)(int taskID,uint32_t eventID);
typedef struct _TASK_EVENT
{
    int taskID;
    uint32_t eventID;

} TASK_EVENT;

int taskCreat(CBTaskEvent task);
void taskLoop();
void taskEventIssue(int taskID,uint32_t eventID);
void taskEventLoop();

//��ʱ������

typedef struct _TASK_TIMER
{
    bool isValid;
    int taskID;
    uint32_t eventID;
    uint32_t timeMs;
    uint32_t start;

} TASK_TIMER;

void taskTicksInc();
void taskTimer(int taskID,uint32_t eventID,uint32_t time_ms);
void taskTimerLoop();

#endif // _THREAD_H
