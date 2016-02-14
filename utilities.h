#pragma once
#ifndef UTILITIES_H
#define UTILITIES_H
#include <math.h>
#include <NIDAQmx.h>
//#include <kinematic\KinematicPerturbation.h>

#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto Error; else

const int controlFreq = 100;
const double loadCellScale[13] = {(1/sqrt(2.0)) * 6.1463,(1/sqrt(2.0)) * 5.9382,(1/sqrt(2.0)) * 6,(1/sqrt(2.0)) * 6,(1/sqrt(2.0)) * 6,(1/sqrt(2.0)) * 6,(1/sqrt(2.0)) * 6, 1, 1 , 1 , 1, 1, 1}; //From calibration test with weights
const int motorMinVoltage = 0;
const int motorMaxVoltage = 7;
const int loadCellMinVoltage = -10;
const int loadCellMaxVoltage = +10;

#define     MOTOR_STATE_INIT 0
#define     MOTOR_STATE_WINDING_UP 1
#define     MOTOR_STATE_CLOSED_LOOP 2
#define     MOTOR_STATE_RUN_EXPERIMENT 3
#define     MOTOR_STATE_SHUTTING_DOWN 4
#define EXIT 5

int proceedState(int *);
#define  SMARTA 0
#define SMARTB 1
#define FDS 2
#define PI 3
#define DI 4
#define LBR 5
#define EIP 6
#define MUSCLE_NUM 7


#endif

