// experimentUserInterface.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <conio.h>
#include <utilities.h>
#include <string>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <random>
#include <pthread.h>
#include <conio.h>
#include <motorControl.h>
#include <math.h>
/*
#ifndef _WIN32
#else
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#define sleep(n) Sleep(n)
#endif
*/
#define NUM_THREADS 2
IPC* test = new IPC();
zmq::context_t context(1);
zmq::socket_t socket_1(context, ZMQ_REP);

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

int main()
{
    
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
    
   delete test;
   return 0;
}