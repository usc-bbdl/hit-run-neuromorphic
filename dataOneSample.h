#ifndef DATA_ONE_SAMPLE_H
#define DATA_ONE_SAMPLE_H

#include <NIDAQmx.h>
#include <stdio.h>
//#include <windows.h>
#include <iostream>
#include <utilities.h>

class dataOneSample
{
    TaskHandle loadCelltaskHandle;
public:
    dataOneSample();
    double loadCellOffset[MUSCLE_NUM+6];
};
#endif