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
#include <map>
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

void *test_1(void*) {
    cout<<"test1 starts\n"<<endl;
    while(true) {
        if(test->wasDataReceived()) {
            Sleep(1000);
            //if IPC gets data from python, process it
            //have to in IPC 
            //int element = stoi(test.testingData)+10;
            //record.insert( std::pair<int, std::vector<int> > (test->getVector_data()[6], test->getVector_element()) );
            std::string textinput = test->generateString(test->getVector_data());
            ofstream myfile ("example.txt", ios_base::app);
            //myfile.open("example.txt", ofstream::app);
            if(myfile.is_open()) {
                std::cout<<textinput<<std::endl;
                myfile << textinput << endl;
                myfile.close();
            }
            //writing text file is done so clear all the data from the vectors and send OK message to IPC
            //so that Python know when file is updated
            test->clearVector_element(); // clear the vector so HR does not get duplicate data.

            test->sendData(10);
        }
    }
    pthread_exit(NULL);
}

void *runServer(void*) {

    cout<<"run server starts\n"<<endl;
    test->startServer();

   pthread_exit(NULL);
   return NULL;
}





int main()
{

      int i, j;
      pthread_t thread_1, thread_2;
      pthread_create(&thread_1, NULL, test_1, NULL);
      pthread_create(&thread_2, NULL, runServer, NULL);
      pthread_join(thread_1, (void **)&i);
      pthread_join(thread_2, (void **)&j);



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

