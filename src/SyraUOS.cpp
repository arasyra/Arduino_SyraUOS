#include <Arduino.h>
#include "SyraUOS.h"

SyraUOS uOS;

bool SyraUOS::Setup(int count)
{
  tSleepMS = 0;
  tMillis = 0;
  
  maxTasksCount = 2;
  if(count > 0 && count < 50)
  {
    maxTasksCount = count;
  }

  tasks = new XTask[maxTasksCount];
  if(tasks != nullptr)
  {
    unsigned char i = 0;
    while (i < maxTasksCount)
    {
      tasks[i].callback = nullptr;
      ++i;
    }

    return true;
  }
  
  return false;
}

void SyraUOS::Run()
{	
  if(tasks == nullptr) return;
  

  unsigned long timeMs = millis();

  unsigned long dt = 0;
  
  if(timeMs > tMillis)
  {
    dt = timeMs - tMillis; 
    tMillis = timeMs; 
  }
  else if(timeMs < 0x0FFF)
  {
    if(tMillis > 0x7FFF)
    {
      dt = 0xFFFF - tMillis;
      dt += timeMs;
      tMillis = timeMs;
    }
  }

  unsigned char i;
  XTask* tmpTsk;

  i = 0;
  while (i < maxTasksCount)
  {
    tmpTsk = &tasks[i];
    if (tmpTsk->callback != nullptr)
    {
      if(tmpTsk->tSleep > 0)
      {
        if(dt > 0)
        {
          unsigned long x = (unsigned long)tmpTsk->tSleep;
          if(x > dt)
          {
            x -= dt;
          }
          else
          {
            x = 0;
          }

          tmpTsk->tSleep = x;
        }
      }
      
      if(tmpTsk->tSleep == 0)
      {
        tSleepMS = 0;
        (*tmpTsk->callback)();
        if (tSleepMS > 0)
        {
          tmpTsk->tSleep = tSleepMS;
        }
      }
    }

    ++i;
  }

}

///////////////////////////////////////////////////////////////////////////
//#######################################################################//
//-------------------
void SyraUOS::TaskWait(long ms)
{	
  tSleepMS = ms;
}

///////////////////////////////////////////////////////////////////////////
//#######################################################################//
//-------------------
int SyraUOS::TaskCreate(void (*callback)())
{
  if(tasks != nullptr)
  {
    for (int i = 0; i < maxTasksCount; i++)
    {
      if (tasks[i].callback == nullptr)
      { 
        tasks[i].callback = callback;
        tasks[i].tSleep = 0;
        return i;
      }
    }
  }
	
	return -1;
}

///////////////////////////////////////////////////////////////////////////
//#######################################################################//
//-------------------
void SyraUOS::TaskDelete(int taskID)
{
  if(tasks != nullptr)
  {
    if(taskID < maxTasksCount)
    {
      tasks[taskID].callback = nullptr;
    }
  }
}

///////////////////////////////////////////////////////////////////////////
//#######################################################################//
//-------------------
void SyraUOS::TaskResume(int taskID)
{
  if(tasks != nullptr)
  {    
    if(taskID < maxTasksCount)
    {
      if (tasks[taskID].callback != nullptr)
      {
        tasks[taskID].tSleep = 0;
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////
//#######################################################################//
//-------------------
void SyraUOS::TaskSuspend()
{	
  tSleepMS = -1;
}

void SyraUOS::TaskSuspend(int taskID)
{
  if(tasks != nullptr)
  {
    if(taskID < maxTasksCount)
    {
      if (tasks[taskID].callback != nullptr)
      {
        tasks[taskID].tSleep = -1;
      }
    }
  }
}

