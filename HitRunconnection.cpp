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
	float64 *loadCellData, *motorRef;
	loadCellData = motors->loadCellData;
    motorRef = getVector();

    motors->updateMotorRef(motorRef);
	motors->newCommand = 1;
    sendVector();
	Sleep(1000);
    t++;
}
int HitRunconnection::establishConnection() {
	//Check if we can establish a connection to Hit&Run (BRIAN)
	//return 0 if fail
	//return 1 if successful

	return 1;
}

float64 * HitRunconnection::getVector() {
	//get vector from BRIAN
    //store the 7 force values in class's motorReference array
    //then comment out place holder code below

    //------------------
    //BEGIN PLACE HOLDER
    static int t = 0;
	for (int i = 0; i<MUSCLE_NUM;i++){
		motorReference[i] = 4*(1+cos(2*3.14*0.02*t));
	}
    //END PLACE HOLDER
    //----------------

    return motorReference;
}

int HitRunconnection::sendVector() {
	//send vector to BRIAN
    //need to send back forceOut array using API

	return 1;
}