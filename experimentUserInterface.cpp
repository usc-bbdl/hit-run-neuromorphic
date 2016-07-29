// experimentUserInterface.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <conio.h>
#include <utilities.h>
#include "zmq.hpp"
#include <string>
#include <pthread.h>
#include "ipc.h"
#include "header.h"
#include <map>
#include <stdlib.h>
#include <string>
#include <fstream>
#include "utilities.h"
#include <NIDAQmx.h>
#include <random>
#include <windows.h>
#include <conio.h>
#include <motorControl.h>
#include <math.h>
#ifndef _WIN32
#else
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>

#define sleep(n) Sleep(n)
#endif

#define NUM_THREADS 2
IPC* test = new IPC();
zmq::context_t context(1);
zmq::socket_t socket_1(context, ZMQ_REP);
std::map<int, std::vector<int> > record;


using namespace std;
void generateCSV(string data);
void *test_1(void*) {
    cout<<"test1 starts\n"<<endl;
    while(true) {
        //EnterCriticalSection(&bufferLock);
        //getting the signal from HR
        //so it can process the work
        if(test->wasDataReceived()) {
        //SleepConditionVariableCS(&Condition_HR, &bufferLock, INFINITE);
            Sleep(1100);

            std::string csv_Input = test->generateString(test->getVector_data());
           
            generateCSV(csv_Input);
    
            test->clearVector_element(); // clear the vector so HR does not get duplicate data.

        //WakeAllConditionVariable(&Condition_Python);
            float64 temp[6];
            test->sendData(temp);//pass in vector of float64, it actually send that vector
            test->false_isReceived();
        }
       //LeaveCriticalSection(&bufferLock); 
    }

    pthread_exit(NULL);
}

void *runServer(void*) {

    cout<<"run server starts\n"<<endl;
    test->startServer();

   pthread_exit(NULL);
   return NULL;
}

void generateCSV(string data) {
     ofstream myfile ("data.csv", ios_base::app);
     if(myfile.is_open()) {
           std::cout<<data<<std::endl;
           myfile << data << endl;
           myfile.close();
    }
}




int main()
{
    //InitializeConditionVariable(&Condition_Python);
    //InitializeConditionVariable(&Condition_HR);
    //InitializeCriticalSection(&bufferLock);

    int i, j;
    pthread_t thread_1, thread_2;
    pthread_create(&thread_1, NULL, test_1, NULL);
    pthread_create(&thread_2, NULL, runServer, NULL);
    pthread_join(thread_1, (void **)&i); //thread 1 : Rest of the code
    pthread_join(thread_2, (void **)&j); //thread 2 : IPC



    /*
    printf("Press Spc to move forward\n\n");
    printf("Press Esc to terminate\n");
    printf("Initialization; Next stage is Motors Winding up\n");
    int gExperimentState = MOTOR_STATE_INIT;
    bool stayInTheLoop = TRUE;
    while(stayInTheLoop)
    {
        char key = 0;
        if (kbhit()!=0){
            key = getch();
            switch ( key ) 
            {
                case 27:        // Terminate Anytime when Escape Is Pressed...
                    stayInTheLoop = FALSE;
                    gExperimentState = MOTOR_STATE_SHUTTING_DOWN;
                    proceedState(&gExperimentState);
                    break;
                case ' ':       // Move forward in the state machine
                    proceedState(&gExperimentState);
                    break;
           }
        }
    }
    */
   delete test;
   return 0;
}