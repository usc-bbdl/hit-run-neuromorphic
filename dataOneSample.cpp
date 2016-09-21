#include "dataOneSample.h"

dataOneSample::dataOneSample()
{
    char        errBuff[2048]={'\0'};
    int32       error=0;
    double loadCellData[MUSCLE_NUM+6]= {0};
    DAQmxErrChk (DAQmxCreateTask("",&loadCelltaskHandle));
    
    DAQmxErrChk (DAQmxCreateAIVoltageChan(loadCelltaskHandle,"PXI1Slot5/ai0","loadCell1",DAQmx_Val_RSE,loadCellMinVoltage,loadCellMaxVoltage,DAQmx_Val_Volts,NULL));
    DAQmxErrChk (DAQmxCreateAIVoltageChan(loadCelltaskHandle,"PXI1Slot5/ai8","loadCell2",DAQmx_Val_RSE,loadCellMinVoltage,loadCellMaxVoltage,DAQmx_Val_Volts,NULL));
	DAQmxErrChk (DAQmxCreateAIVoltageChan(loadCelltaskHandle,"PXI1Slot5/ai1","loadCell3",DAQmx_Val_RSE,loadCellMinVoltage,loadCellMaxVoltage,DAQmx_Val_Volts,NULL));
    DAQmxErrChk (DAQmxCreateAIVoltageChan(loadCelltaskHandle,"PXI1Slot5/ai9","loadCell4",DAQmx_Val_RSE,loadCellMinVoltage,loadCellMaxVoltage,DAQmx_Val_Volts,NULL));
	DAQmxErrChk (DAQmxCreateAIVoltageChan(loadCelltaskHandle,"PXI1Slot5/ai2","loadCell5",DAQmx_Val_RSE,loadCellMinVoltage,loadCellMaxVoltage,DAQmx_Val_Volts,NULL));
    DAQmxErrChk (DAQmxCreateAIVoltageChan(loadCelltaskHandle,"PXI1Slot5/ai10","loadCell6",DAQmx_Val_RSE,loadCellMinVoltage,loadCellMaxVoltage,DAQmx_Val_Volts,NULL));
	DAQmxErrChk (DAQmxCreateAIVoltageChan(loadCelltaskHandle,"PXI1Slot5/ai3","loadCell7",DAQmx_Val_RSE,loadCellMinVoltage,loadCellMaxVoltage,DAQmx_Val_Volts,NULL));
    
	DAQmxErrChk (DAQmxCreateAIVoltageChan(loadCelltaskHandle,"PXI1Slot5/ai16","JR3-FX",DAQmx_Val_RSE,loadCellMinVoltage,loadCellMaxVoltage,DAQmx_Val_Volts,NULL));
    DAQmxErrChk (DAQmxCreateAIVoltageChan(loadCelltaskHandle,"PXI1Slot5/ai17","JR3-FY",DAQmx_Val_RSE,loadCellMinVoltage,loadCellMaxVoltage,DAQmx_Val_Volts,NULL));
	DAQmxErrChk (DAQmxCreateAIVoltageChan(loadCelltaskHandle,"PXI1Slot5/ai18","JR3-FZ",DAQmx_Val_RSE,loadCellMinVoltage,loadCellMaxVoltage,DAQmx_Val_Volts,NULL));
    DAQmxErrChk (DAQmxCreateAIVoltageChan(loadCelltaskHandle,"PXI1Slot5/ai19","JR3-Tau1",DAQmx_Val_RSE,loadCellMinVoltage,loadCellMaxVoltage,DAQmx_Val_Volts,NULL));
	DAQmxErrChk (DAQmxCreateAIVoltageChan(loadCelltaskHandle,"PXI1Slot5/ai20","JR3-Tau2",DAQmx_Val_RSE,loadCellMinVoltage,loadCellMaxVoltage,DAQmx_Val_Volts,NULL));
	DAQmxErrChk (DAQmxCreateAIVoltageChan(loadCelltaskHandle,"PXI1Slot5/ai21","JR3-Tau3",DAQmx_Val_RSE,loadCellMinVoltage,loadCellMaxVoltage,DAQmx_Val_Volts,NULL));
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
}