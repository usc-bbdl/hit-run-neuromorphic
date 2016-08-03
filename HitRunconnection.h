#ifndef HIT_RUN_CONNECTION_H
#define HIT_RUN_CONNECTION_H

#include <motorControl.h>
#include <process.h>

class HitRunconnection
{
    //for file io
    int numTrials, trialIndex;
    FILE *dataFile;
    //file io
    float ref1,ref2,ref3,ref4,ref5,ref6,ref7;
	bool live;
    motorControl *motors;
    HANDLE hIOMutex;
    float64 motorReference[MUSCLE_NUM];
    float64 forceOut[MUSCLE_NUM+6];
    float gain, bias;
    static void HitRunconnectionControlLoop(void*);
    void controlLoop();
    void update();
public:
    HitRunconnection(motorControl*);
    ~HitRunconnection(void);
    void startConnection();
    int establishConnection();
    float64 * getVector();
    int scaleVector();
    int sendVector();
    
};
#endif
