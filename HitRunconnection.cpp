#include "HitRunconnection.h"
#include "utilities.h"
#include <NIDAQmx.h>
#include <random>
#include <conio.h>
#include <motorControl.h>
#include <math.h>

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
    int key = 0;
    while (live)
    { 
        if (kbhit()!=0){
            key = getch();
            if(key == 27) live = FALSE;
        }
        update();
    }
}

int HitRunconnection::scaleVector() {
    
    int i;
    for( i = 0; i < MUSCLE_NUM; i++) {
        motorReference[i] = (motorReference[i]*gain) + bias;
    }
    return 1;
}
void HitRunconnection::update() { //This is the function called in the thread
	
   // float64 *loadCellData;
    for(int i = 0; i < MUSCLE_NUM+6; i++) {
        loadCellData[i] = motors->loadCellData[i];
    }
    getVector();
    scaleVector();
    motors->updateMotorRef(motorReference);
	motors->newCommand = 1;
    sendVector();
	Sleep(500);
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
//    static int t = 0;
//	for (int i = 0; i<MUSCLE_NUM;i++, t++){
//		motorReference[i] = 4*(1+cos(2*3.14*0.02*t));
//	}
    /*
    if (trialIndex < numTrials){
        //fscanf(dataFile,"%f,%f,%f,%f,%f,%f,%f\n", &motorReference[0], &motorReference[1], &motorReference[2], &motorReference[3], &motorReference[4], &motorReference[5], &motorReference[6]);
        fscanf(dataFile,"%f,%f,%f,%f,%f,%f,%f\n", &ref1, &ref2, &ref3, &ref4, &ref5, &ref6, &ref7);
        motorReference[0] = ref1;
        motorReference[1] = ref2;
        motorReference[2] = ref3;
        motorReference[3] = ref4;
        motorReference[4] = ref5;
        motorReference[5] = ref6;
        motorReference[6] = ref7;
        trialIndex++;
    }
    else{
        live = FALSE;
        printf("Experiment complete. Press space to shutdown\n");
    }
    return motorReference;
    */
    
    //getting the signal from HR
    //so it can process the work
    bool flag = true;
    do {
        if(ipc_connection.wasDataReceived()) {
            Sleep(500);
            //getting data from Python
            std::string csv_Input = ipc_connection.generateString(ipc_connection.getVector_data());
            for(int i = 0; i < MUSCLE_NUM; i++){
                 motorReference[i] = ipc_connection.getVector_data()[i];
            }
            flag = false;
        }
    }
    while(flag);
    return motorReference;
}

int HitRunconnection::sendVector() {
	//send vector to BRIAN
    //need to send back forceOut array using API
    ipc_connection.clearVector_element(); // clear the vector so HR does not get duplicate data.
    ipc_connection.sendData(loadCellData);//pass in vector of float64, it actually send that vector
    ipc_connection.false_isReceived();
	
    return 1;
}
void *HitRunconnection::runServer(void*) {
   ipc_connection.startServer();
   pthread_exit(NULL);
   return NULL;
}
