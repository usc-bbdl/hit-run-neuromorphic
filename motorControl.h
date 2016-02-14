#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include <NIDAQmx.h>
#include <utilities.h>
#include <iostream>
#include <windows.h>
#include <process.h>
#include <ctime>

class TimeData
{
      LARGE_INTEGER initialTick, currentTick, frequency;
public:

    double actualTime;
    TimeData(void);	
    ~TimeData(void);
    int resetTimer();
    double getCurrentTime(void);
};

class motorControl
{
    TaskHandle  motorTaskHandle, motorEnableHandle,loadCelltaskHandle;
    double loadCellOffset[MUSCLE_NUM+6],I;
    TimeData timeData;
    static void motorControlLoop(void*);
    void controlLoop(void);
    HANDLE hIOMutex;
    bool live;
    char header[200];
public:
    motorControl(double*);
    ~motorControl(void);
    bool resetMuscleLength;
    float64 loadCellData[MUSCLE_NUM+6], motorRef[MUSCLE_NUM];
    bool isEnable, isWindUp, isControlling;
    int motorEnable();
    int motorWindUp();
    int motorDisable();
    int motorControllerStart();
    int motorControllerEnd();
	int newCommand;
    void updateMotorRef(float64 *);
};

#endif

