#ifndef HIT_RUN_CONNECTION_H
#define HIT_RUN_CONNECTION_H

#include <motorControl.h>
#include <windows.h>
#include <process.h>
class HitRunconnection
{
	bool live;
    motorControl *motors;
    HANDLE hIOMutex;
    static void HitRunconnectionControlLoop(void*);
    void controlLoop();
    void update();
public:
    HitRunconnection(motorControl*);
    ~HitRunconnection(void);
    void startConnection();
    int establishConnection();
    
};
#endif
