

#ifndef SyraUOS_h
#define SyraUOS_h

#define BAD_TASK_ID  -1

typedef struct 
{
  void (*callback)();
  long tSleep;
}XTask;

class SyraUOS
{
  public:
  bool Setup(int count);
  void Run();
  int TaskCreate(void (*callback)());
  void TaskWait(long ms);
  void TaskSuspend();
  void TaskSuspend(int taskID);
  void TaskResume(int taskID);
  void TaskDelete(int taskID);

  private:
  XTask* tasks = nullptr;
  unsigned char volatile maxTasksCount;

  long volatile tSleepMS;
  unsigned long volatile tMillis;

};

extern SyraUOS uOS;

#define _TaskWait(x)      uOS.TaskWait(x)
#define _TaskSuspend()    uOS.TaskSuspend()

#define _TaskSuspendByID(x)   uOS.TaskSuspend(x)
#define _TaskResumeByID(x)    uOS.TaskResume(x)
#define _TaskDeleteByID(x)    uOS.TaskDelete(x)


#endif