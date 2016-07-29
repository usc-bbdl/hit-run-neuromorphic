#include "dataOneSample.h"

dataOneSample::dataOneSample(hardwareConfiguration * temp)
{
    hardware = temp;
}
int dataOneSample::readOffset(){
    char        errBuff[2048]={'\0'};
    char channel[50];
    int32       error=0;
    double loadCellData[MUSCLE_NUM+6]= {0};
    DAQmxErrChk (DAQmxCreateTask("",&loadCelltaskHandle));
    char array1[18] = {"abcdefg"};
    size_t destination_size = sizeof (channel);
    for(int i=0;i<MUSCLE_NUM;i++)
        {
            strncpy(channel, hardware->loadCellChannels[i],destination_size);
            DAQmxErrChk (DAQmxCreateAIVoltageChan(loadCelltaskHandle,channel,"loadCell",DAQmx_Val_RSE,loadCellMinVoltage,loadCellMaxVoltage,DAQmx_Val_Volts,NULL));
        }
    for(int i=0;i<6;i++)
        {
            strncpy(channel, hardware->JR3Channels[i],destination_size);
            DAQmxErrChk (DAQmxCreateAIVoltageChan(loadCelltaskHandle,channel,"JR3",DAQmx_Val_RSE,loadCellMinVoltage,loadCellMaxVoltage,DAQmx_Val_Volts,NULL));
        }
    DAQmxErrChk (DAQmxCfgSampClkTiming(loadCelltaskHandle,NULL,100,DAQmx_Val_Rising,DAQmx_Val_FiniteSamps,2));

    DAQmxErrChk (DAQmxStartTask(loadCelltaskHandle));
    DAQmxErrChk (DAQmxReadAnalogF64(loadCelltaskHandle,-1,10.0,DAQmx_Val_GroupByScanNumber,loadCellData,MUSCLE_NUM+6,NULL,NULL));
    for (int i=0; i<MUSCLE_NUM+6; i++) {
        loadCellOffset[i] = loadCellData[i] * loadCellScale[i];
    }
Error:
	if( DAQmxFailed(error) ) {
		DAQmxGetExtendedErrorInfo(errBuff,2048);
		/*********************************************/
		// DAQmx Stop Code
		/*********************************************/
		printf("DAQmx Error: %s\n",errBuff);
        printf("Data one Sample Error: %s\n",errBuff);
	}
    if (loadCelltaskHandle)
    {
        DAQmxStopTask(loadCelltaskHandle);
		DAQmxClearTask(loadCelltaskHandle);
        loadCelltaskHandle = 0;
    }
    return 1;
}
