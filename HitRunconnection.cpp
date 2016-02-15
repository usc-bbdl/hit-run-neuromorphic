#include "HitRunconnection.h"
#include "utilities.h"
#include <NIDAQmx.h>
#include <random>
#include <motorControl.h>
#include <math.h>
HitRunconnection::HitRunconnection(motorControl *temp)
{
	motors = temp;
    live = FALSE;
}
HitRunconnection::~HitRunconnection() {
	live = FALSE;
}

void HitRunconnection::startConnection()
{
    live = TRUE;
    hIOMutex = CreateMutex(NULL, FALSE, NULL);
	_beginthread(HitRunconnection::HitRunconnectionControlLoop,0,this);
}
void HitRunconnection::HitRunconnectionControlLoop(void* a)
{
	((HitRunconnection*)a)->controlLoop();
}
void HitRunconnection::controlLoop(void){
    while (live)
    {
        update();
    }
}
void HitRunconnection::update() { //This is the function called in the thread
	float64 motorReference[MUSCLE_NUM], *loadCellData;
	loadCellData = motors->loadCellData;
	//Pass loadCellData to Hit&Run
	//Recieve motorRef from Hit&Run
	//std::default_random_engine generator;
	//std::normal_distribution<double> distribution(10,1.0);
    static int t = 0;
	for (int i = 0; i<MUSCLE_NUM;i++){
		motorReference[i] = 4*(1+cos(2*3.14*0.02*t));
	}
	//these are replaced by random numbers here
    motors->updateMotorRef(motorReference);
	motors->newCommand = 1;
	Sleep(1000);
    t++;
}
int HitRunconnection::establishConnection() {
	//Check if we can establish a connection to Hit&Run
	//return 0 if fail
	//return 1 if successful
	return 1;
}