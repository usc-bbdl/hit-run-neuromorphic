#pragma once
#ifndef IPC_H_
#define IPC_H_
#include <NIDAQmx.h>
//include these for socket programming
#include "zmq.hpp"
#include "utilities.h"
#include <pthread.h>
#include <stdio.h>

class IPC {

public:

	IPC() : context(1), socket(context, ZMQ_REP) {
        isReceieved = false;
        toPython = " ";
    }
	void *startServer();

    bool isServerWorking();

    bool wasDataReceived();

    float64 * getData(); // should the return type of this be vector instead or does
                         // the vectorization of the data happen in the HitRun class
    std::string sendData(float64 data_to_python[MUSCLE_NUM]);
	
	float64 * generateVector(std::string python_data);

	std::string IPC::generateString(float64 vector_c[MUSCLE_NUM]);

	std::string testingData;

    float64 * getVector_element();

    void clearVector_element();

    float64 * getVector_data();

    void false_isReceived();
    
    static void *StartServer_helper(void * context){
        return ((IPC *)context)->startServer();
    }
 
    private:
	zmq::context_t context;
	zmq::socket_t socket;
	zmq::message_t request;
	float64 vector_data[MUSCLE_NUM]; //this is vector data file that is sent to hand c++ so it can process it.
	std::vector<int> csv_vector; //it can be used in order to generate csv file.
	bool isReceieved;
	std::string toPython;
    float64 vector_element[MUSCLE_NUM];
    //static pthread_mutex_t IPC::lock = PTHREAD_MUTEX_INITIALIZER;
    //static pthread_cond_t IPC::cond = PTHREAD_COND_INITIALIZER;
};
#endif
