#include "motorControl.h"
#include <utilities.h>
#include <stdio.h>
motorControl::motorControl(double *param)
{
    I = 3;
    char        errBuff[2048]={'\0'};
    int32       error=0;
    loadCellOffset[0] = param[0];
    loadCellOffset[1] = param[1];
    loadCellOffset[2] = param[2];
    loadCellOffset[3] = param[3];
    loadCellOffset[4] = param[4];
    loadCellOffset[5] = param[5];
    loadCellOffset[6] = param[6];
    loadCellOffset[7] = param[7];
    loadCellOffset[8] = param[8];
    loadCellOffset[9] = param[9];
    loadCellOffset[10] = param[10];
    loadCellOffset[11] = param[11];
    loadCellOffset[12] = param[12];

    isEnable = FALSE;
    isWindUp = FALSE;
    isControlling = FALSE;
    live = FALSE;
	for(int i=0;i<MUSCLE_NUM;i++)
		motorRef[i] = 2;
	char subHeader[100]="";
    //strcpy(header,"Time, New Command, ");
    strcpy(header,"Time, New Command, Measured EIP, Command EIP, Measured DI, Command DI, Measured EDC, Command EDC, Measured PI, Command PI, Measured LUM, Command LUM, Measured FDS, Command FDS, Measured FDP, Command FDP");
	/*for (int i=0; i<MUSCLE_NUM; i++) {
		sprintf(subHeader,", Measured Force %d, Force Command %d",i,i);
		strcat (header, subHeader);
	}*/
	strcat (header, ", Force y, Mx, Force x, M z, M y, Force z\n");

    DAQmxErrChk (DAQmxCreateTask("",&loadCelltaskHandle));
    DAQmxErrChk (DAQmxCreateAIVoltageChan(loadCelltaskHandle,"PXI1Slot5/ai0","loadCell1",DAQmx_Val_RSE,loadCellMinVoltage,loadCellMaxVoltage,DAQmx_Val_Volts,NULL));
    DAQmxErrChk (DAQmxCreateAIVoltageChan(loadCelltaskHandle,"PXI1Slot5/ai8","loadCell2",DAQmx_Val_RSE,loadCellMinVoltage,loadCellMaxVoltage,DAQmx_Val_Volts,NULL));
	DAQmxErrChk (DAQmxCreateAIVoltageChan(loadCelltaskHandle,"PXI1Slot5/ai1","loadCell3",DAQmx_Val_RSE,loadCellMinVoltage,loadCellMaxVoltage,DAQmx_Val_Volts,NULL));
    DAQmxErrChk (DAQmxCreateAIVoltageChan(loadCelltaskHandle,"PXI1Slot5/ai9","loadCell4",DAQmx_Val_RSE,loadCellMinVoltage,loadCellMaxVoltage,DAQmx_Val_Volts,NULL));
    DAQmxErrChk (DAQmxCreateAIVoltageChan(loadCelltaskHandle,"PXI1Slot5/ai2","loadCell5",DAQmx_Val_RSE,loadCellMinVoltage,loadCellMaxVoltage,DAQmx_Val_Volts,NULL));
    DAQmxErrChk (DAQmxCreateAIVoltageChan(loadCelltaskHandle,"PXI1Slot5/ai10","loadCell6",DAQmx_Val_RSE,loadCellMinVoltage,loadCellMaxVoltage,DAQmx_Val_Volts,NULL));
    DAQmxErrChk (DAQmxCreateAIVoltageChan(loadCelltaskHandle,"PXI1Slot5/ai3","loadCell7",DAQmx_Val_RSE,loadCellMinVoltage,loadCellMaxVoltage,DAQmx_Val_Volts,NULL));

    DAQmxErrChk (DAQmxCreateAIVoltageChan(loadCelltaskHandle,"PXI1Slot5/ai16","JR3-FY",DAQmx_Val_RSE,loadCellMinVoltage,loadCellMaxVoltage,DAQmx_Val_Volts,NULL));
    DAQmxErrChk (DAQmxCreateAIVoltageChan(loadCelltaskHandle,"PXI1Slot5/ai17","JR3-MX",DAQmx_Val_RSE,loadCellMinVoltage,loadCellMaxVoltage,DAQmx_Val_Volts,NULL));
	DAQmxErrChk (DAQmxCreateAIVoltageChan(loadCelltaskHandle,"PXI1Slot5/ai18","JR3-FX",DAQmx_Val_RSE,loadCellMinVoltage,loadCellMaxVoltage,DAQmx_Val_Volts,NULL));
    DAQmxErrChk (DAQmxCreateAIVoltageChan(loadCelltaskHandle,"PXI1Slot5/ai19","JR3-MZ",DAQmx_Val_RSE,loadCellMinVoltage,loadCellMaxVoltage,DAQmx_Val_Volts,NULL));
	DAQmxErrChk (DAQmxCreateAIVoltageChan(loadCelltaskHandle,"PXI1Slot5/ai20","JR3-MY",DAQmx_Val_RSE,loadCellMinVoltage,loadCellMaxVoltage,DAQmx_Val_Volts,NULL));
	DAQmxErrChk (DAQmxCreateAIVoltageChan(loadCelltaskHandle,"PXI1Slot5/ai21","JR3-FZ",DAQmx_Val_RSE,loadCellMinVoltage,loadCellMaxVoltage,DAQmx_Val_Volts,NULL));
	

	DAQmxErrChk (DAQmxCfgSampClkTiming(loadCelltaskHandle,"",controlFreq,DAQmx_Val_Rising,DAQmx_Val_HWTimedSinglePoint,NULL));
    DAQmxErrChk (DAQmxSetRealTimeConvLateErrorsToWarnings(loadCelltaskHandle,1));

    DAQmxErrChk (DAQmxCreateTask("",&motorTaskHandle));
    DAQmxErrChk (DAQmxCreateAOVoltageChan(motorTaskHandle,"PXI1Slot2/ao8","motor1",motorMinVoltage,motorMaxVoltage,DAQmx_Val_Volts,NULL));
	DAQmxErrChk (DAQmxCreateAOVoltageChan(motorTaskHandle,"PXI1Slot2/ao9","motor2",motorMinVoltage,motorMaxVoltage,DAQmx_Val_Volts,NULL));
    DAQmxErrChk (DAQmxCreateAOVoltageChan(motorTaskHandle,"PXI1Slot2/ao10","motor3",motorMinVoltage,motorMaxVoltage,DAQmx_Val_Volts,NULL));
	DAQmxErrChk (DAQmxCreateAOVoltageChan(motorTaskHandle,"PXI1Slot2/ao11","motor4",motorMinVoltage,motorMaxVoltage,DAQmx_Val_Volts,NULL));
	DAQmxErrChk (DAQmxCreateAOVoltageChan(motorTaskHandle,"PXI1Slot2/ao12","motor5",motorMinVoltage,motorMaxVoltage,DAQmx_Val_Volts,NULL));
	DAQmxErrChk (DAQmxCreateAOVoltageChan(motorTaskHandle,"PXI1Slot2/ao13","motor6",motorMinVoltage,motorMaxVoltage,DAQmx_Val_Volts,NULL));
	DAQmxErrChk (DAQmxCreateAOVoltageChan(motorTaskHandle,"PXI1Slot2/ao14","motor7",motorMinVoltage,motorMaxVoltage,DAQmx_Val_Volts,NULL));
    DAQmxErrChk (DAQmxCfgSampClkTiming(motorTaskHandle,"",controlFreq,DAQmx_Val_Rising,DAQmx_Val_HWTimedSinglePoint,1));

    DAQmxErrChk (DAQmxCreateTask("",&motorEnableHandle));
    DAQmxErrChk (DAQmxCreateDOChan(motorEnableHandle,"PXI1Slot2/port0","motorEnable",DAQmx_Val_ChanForAllLines));

    
Error:
	if( DAQmxFailed(error) ) {
		DAQmxGetExtendedErrorInfo(errBuff,2048);
		DAQmxClearTask(motorTaskHandle);
		DAQmxClearTask(loadCelltaskHandle);
		DAQmxClearTask(motorEnableHandle);

		printf("DAQmx Error: %s\n",errBuff);
        printf("Motor, load cell or encoder initialization error\n");
	}
}
motorControl::~motorControl()
{
	live = FALSE;
	DAQmxStopTask(motorEnableHandle);
    DAQmxStopTask(motorTaskHandle);
    DAQmxStopTask(loadCelltaskHandle);

    DAQmxClearTask(motorEnableHandle);
    DAQmxClearTask(motorTaskHandle);
    DAQmxClearTask(loadCelltaskHandle);
    
}

int motorControl::motorEnable()
{
    uInt32      dataEnable=0x0000000f;
    char        errBuff[2048]={'\0'};
    int32       error=0;
    float64 zeroVoltages[MUSCLE_NUM]={0.0};

    DAQmxErrChk (DAQmxStartTask(motorEnableHandle));
    DAQmxErrChk (DAQmxStartTask(motorTaskHandle));
    DAQmxErrChk (DAQmxWriteDigitalU32(motorEnableHandle,1,1,10.0,DAQmx_Val_GroupByChannel,&dataEnable,NULL,NULL));
    DAQmxErrChk (DAQmxWriteAnalogF64(motorTaskHandle,1,FALSE,10,DAQmx_Val_GroupByChannel,zeroVoltages,NULL,NULL));
    Sleep(500);
    DAQmxStopTask(motorTaskHandle);
    DAQmxStopTask(motorEnableHandle);
    isEnable = TRUE;
Error:
	if( DAQmxFailed(error) ) {
		DAQmxGetExtendedErrorInfo(errBuff,2048);
        DAQmxStopTask(motorTaskHandle);
		DAQmxClearTask(motorTaskHandle);
        DAQmxStopTask(motorEnableHandle);
		DAQmxClearTask(motorEnableHandle);

		printf("DAQmx Error: %s\n",errBuff);
        printf("Motor enable Error\n");
	}
    return 0;
}

int motorControl::motorWindUp()
{
    char        errBuff[2048]={'\0'};
    int32       error=0;
    float64 windingUpCmnd[MUSCLE_NUM];
	for (int i=0;i<MUSCLE_NUM;i++){
		windingUpCmnd[i] = 0.5;
	}
    if (isEnable){
        DAQmxErrChk (DAQmxStartTask(motorTaskHandle));
        DAQmxErrChk (DAQmxWriteAnalogF64(motorTaskHandle,1,FALSE,10,DAQmx_Val_GroupByChannel,windingUpCmnd,NULL,NULL));
        Sleep(500);
        isWindUp = TRUE;

        DAQmxStopTask(motorTaskHandle);
        
        printf("Windup Pass.\n");
    }else  printf("Motors must be first enabled prior to winding up.\n");
Error:
	if( DAQmxFailed(error) ) {
		DAQmxGetExtendedErrorInfo(errBuff,2048);
        DAQmxStopTask(motorTaskHandle);
        DAQmxClearTask(motorTaskHandle);
		printf("DAQmx Error: %s\n",errBuff);
        printf("winding up Error\n");
	}
     return 0;
}

void motorControl::motorControlLoop(void* a)
{
	((motorControl*)a)->controlLoop();
}

void motorControl::controlLoop(void)
{
    int32       error=0;
    bool keepReading=TRUE;
    bool32 isLate = {0};
    double tick=0.0,tock=0.0;
    float64 motorCommand[MUSCLE_NUM]={0.0},errorForce[MUSCLE_NUM]= {0.0},integral[MUSCLE_NUM]={0.0};
    char        errBuff[2048]={'\0'};
    FILE *dataFile;
    time_t t = time(NULL);
    tm* timePtr = localtime(&t);
    char fileName[200];
    char dataSample[600]="";
    char dataTemp[100]="";
    sprintf_s(
            fileName,
            "C:\\data\\realTimeData%4d_%02d_%02d_%02d_%02d_%02d.txt",
            timePtr->tm_year+1900, 
            timePtr->tm_mon+1, 
            timePtr->tm_mday, 
            timePtr->tm_hour, 
            timePtr->tm_min, 
            timePtr->tm_sec
            );
    dataFile = fopen(fileName,"w");
    fprintf(dataFile,header);
    DAQmxErrChk (DAQmxStartTask(loadCelltaskHandle));
    DAQmxErrChk (DAQmxStartTask(motorTaskHandle));
    timeData.resetTimer();
    tick = timeData.getCurrentTime();
    float64 goffsetLoadCell[2]={0};
    while(live)
    {
        WaitForSingleObject(hIOMutex, INFINITE);
        //desire Forced, muscle Length, muscle Velocity PIPES should be read here
        DAQmxErrChk(DAQmxWaitForNextSampleClock(loadCelltaskHandle,10, &isLate));
        DAQmxErrChk (DAQmxReadAnalogF64(loadCelltaskHandle,-1,10.0,DAQmx_Val_GroupByScanNumber,loadCellData,MUSCLE_NUM+6,NULL,NULL));
        DAQmxErrChk (DAQmxWriteAnalogF64(motorTaskHandle,1,FALSE,10,DAQmx_Val_GroupByChannel,motorCommand,NULL,NULL));
        tock = timeData.getCurrentTime();
		for (int i=0;i<MUSCLE_NUM;i++){
			loadCellData[i] = (loadCellData[i] * loadCellScale[i]) - loadCellOffset[i];
			errorForce[i] = motorRef[i] - loadCellData[i];
			integral[i] = integral[i] + errorForce[i] * (tock - tick);
			motorCommand[i] = integral[i] * I;
			if (motorCommand[i] > motorMaxVoltage)
				motorCommand[i] = motorMaxVoltage;
			if (motorCommand[i] < motorMinVoltage)
				motorCommand[i] = motorMinVoltage;
		}
        loadCellData[7] = (loadCellData[7] * loadCellScale[7]) - loadCellOffset[7];
        loadCellData[8] = (loadCellData[8] * loadCellScale[8]) - loadCellOffset[8];
        loadCellData[9] = (loadCellData[9] * loadCellScale[9]) - loadCellOffset[9];
        loadCellData[10] = (loadCellData[10] * loadCellScale[10]) - loadCellOffset[10];
        loadCellData[11] = (loadCellData[11] * loadCellScale[11]) - loadCellOffset[11];
        loadCellData[12] = (loadCellData[12] * loadCellScale[12]) - loadCellOffset[12];
        //printf("Load Cell: %+6.2f; Force Command: %+6.2f \r",loadCellData[0],motorRef[0]);
        printf("1:%+6.2f,2:%+6.2f,3: %+6.2f,4: %+6.2f,5: %+6.2f,6: %+6.2f,7: %+6.2f \r",motorRef[0],motorRef[1],motorRef[2],motorRef[3],motorRef[4],motorRef[5],motorRef[6]);
        ReleaseMutex( hIOMutex);
        sprintf(dataSample,"%.3f,%d",tock,newCommand);
		for(int i = 0;i<MUSCLE_NUM;i++)
		{
			sprintf(dataTemp,",%.6f,%.6f",loadCellData[i],motorRef[i]);
			strcat (dataSample, dataTemp);
		}
		sprintf(dataTemp,",%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\n",loadCellData[MUSCLE_NUM],loadCellData[MUSCLE_NUM+1],loadCellData[MUSCLE_NUM+2],loadCellData[MUSCLE_NUM+3],loadCellData[MUSCLE_NUM+4],loadCellData[MUSCLE_NUM+5]);
		strcat (dataSample, dataTemp);
		if (newCommand)
			newCommand = 0;
        fprintf(dataFile,dataSample);
        tick = timeData.getCurrentTime();
    }
    for (int i=0;i<MUSCLE_NUM;i++)
        motorCommand[i] = 0;
    DAQmxErrChk (DAQmxWriteAnalogF64(motorTaskHandle,1,FALSE,10,DAQmx_Val_GroupByChannel,motorCommand,NULL,NULL));
    DAQmxStopTask(motorTaskHandle);
    DAQmxStopTask(motorEnableHandle);
    isControlling = FALSE;
    fclose(dataFile);
Error:
	if( DAQmxFailed(error) ) {
		DAQmxGetExtendedErrorInfo(errBuff,2048);
		/*********************************************/
		// DAQmx Stop Code
		/*********************************************/
		DAQmxStopTask(loadCelltaskHandle);
		DAQmxClearTask(loadCelltaskHandle);
        DAQmxStopTask(motorTaskHandle);
		DAQmxClearTask(motorTaskHandle);
        DAQmxStopTask(motorEnableHandle);
		DAQmxClearTask(motorEnableHandle);
		printf("DAQmx Error: %s\n",errBuff);
        printf("Motor control Error\n");
	}
}

void motorControl::updateMotorRef(float64 *a){
    for (int i =0;i<MUSCLE_NUM;i++)
    {
        motorRef[i] = (a[i]);
    }
}

int motorControl::motorControllerStart()
{
    if ((isEnable) && (isWindUp))
    {
        isControlling = TRUE;
        live = TRUE;
        hIOMutex = CreateMutex(NULL, FALSE, NULL);
		isControlling = TRUE;
		_beginthread(motorControl::motorControlLoop,0,this);
    }else
    {
        isControlling = FALSE;
        printf("Motors must be first enabled or wind up before closed-loop control.\n");
    }
    return 0;
}
int motorControl::motorControllerEnd()
{
    live = FALSE;
    motorControl::motorDisable();
    isControlling = FALSE;
    DAQmxStopTask(motorTaskHandle);
	DAQmxClearTask(motorTaskHandle);
    DAQmxStopTask(loadCelltaskHandle);
	DAQmxClearTask(loadCelltaskHandle);
    return 0;
}

int motorControl::motorDisable()
{
	int32       error=0;
	char        errBuff[2048] = {'\0'};
    uInt32      dataDisable=0x00000000;
    int32		written;
    float64 zeroVoltages[MUSCLE_NUM]={0.0};
    while(isControlling){
    }
    DAQmxErrChk (DAQmxStartTask(motorEnableHandle));
    DAQmxErrChk (DAQmxStartTask(motorTaskHandle));
    DAQmxErrChk (DAQmxWriteDigitalU32(motorEnableHandle,1,1,10.0,DAQmx_Val_GroupByChannel,&dataDisable,NULL,NULL));
    DAQmxErrChk (DAQmxWriteAnalogF64(motorTaskHandle,1,FALSE,10,DAQmx_Val_GroupByChannel,zeroVoltages,NULL,NULL));
    Sleep(500);
    DAQmxStopTask(motorTaskHandle);
    DAQmxStopTask(motorEnableHandle);
    isControlling = FALSE;
    isWindUp = FALSE;
    live = FALSE;
    isEnable = FALSE;

Error:
	if( DAQmxFailed(error) ){
		printf("DisableMotor Error: %s\n",errBuff);
        DAQmxGetExtendedErrorInfo(errBuff,2048);
        printf("DAQmx Error: %s\n",errBuff);
        printf("Motor Disable Error\n");
		DAQmxStopTask(motorEnableHandle);
        DAQmxStopTask(motorTaskHandle);

        DAQmxClearTask(motorEnableHandle);
        DAQmxClearTask(motorTaskHandle);
    }
	return 0;
}

TimeData::TimeData(void)
{
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&initialTick);
}


TimeData::~TimeData(void)
{
}

// Reset the timer
int TimeData::resetTimer(void)
{
    QueryPerformanceCounter(&initialTick);
    return 0;
}


// Get current time in seconds
double TimeData::getCurrentTime(void)
{
    QueryPerformanceCounter(&currentTick);
    actualTime = (double)(currentTick.QuadPart - initialTick.QuadPart);
    actualTime /= (double)frequency.QuadPart;
    return actualTime;
}