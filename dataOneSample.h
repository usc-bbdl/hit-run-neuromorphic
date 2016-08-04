#ifndef DATA_ONE_SAMPLE_H
#define DATA_ONE_SAMPLE_H

#include <NIDAQmx.h>
#include <stdio.h>
//#include <windows.h>
#include <iostream>
#include <utilities.h>
#include "hardwareConfiguration.h"
class dataOneSample
{
    hardwareConfiguration *hardware;
    TaskHandle loadCelltaskHandle;
public:
    int readOffset();
    dataOneSample(hardwareConfiguration*);
    //~dataOneSample(void);
    double loadCellOffset[MUSCLE_NUM+6];
};
#endif
