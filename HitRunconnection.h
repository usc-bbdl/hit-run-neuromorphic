#ifndef HIT_RUN_CONNECTION_H
#define HIT_RUN_CONNECTION_H

#include <motorControl.h>
#include <process.h>
#include <pthread.h>



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
    IPC * test;
    zmq::context_t context;
    zmq::socket_t socket_1;
    typedef void* (HitRunconnection::*HRptr)(void);
    typedef void* (*Pthreadptr)(void*);

public:
        HitRunconnection(motorControl* temp): context(1), socket_1(context, ZMQ_REP){
        	motors = temp;
            live = FALSE;
            //File IO
            char *header[200];
            dataFile = fopen("seven_muscles.csv","r");
            if (dataFile == NULL) 
                printf("Could not open data file");
            else{
            fscanf(dataFile,"%s\n",&header);
            fscanf(dataFile,"%d,%d\n",&numTrials);
            }
            trialIndex = 0;
            //end File IO

            gain = 8;
            bias = 2;

            test = new IPC();
            //int j;
            //pthread_t thread_2;
            //pthread_create(&thread_2, NULL, HitRunconnection::runServer, this);
            //pthread_join(thread_2, (void **)&j); //thread 2 : IPC

    };
    ~HitRunconnection(void);
    void startConnection();
    int establishConnection();
    float64 * getVector();
    int scaleVector();
    int sendVector();
    void *runServer(void*);
    void startThread(void);

};
#endif
