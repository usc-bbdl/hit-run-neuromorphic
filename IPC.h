#pragma once
#ifndef IPC_H_
#define IPC_H_
#include "ipc.h"
#include <pthread.h>


//include these for socket programming
#include "zmq.hpp"
#include <vector>
#ifndef _WIN32
#include <unistd.h>
#else
#define sleep(n)  Sleep(n)
#endif


class IPC {

public:

	IPC() : context(1), socket(context, ZMQ_REP) {}

	void startServer();

    bool isServerWorking();

    bool wasDataReceived();

    std::vector<int> getData(); // should the return type of this be vector instead or does
                    // the vectorization of the data happen in the HitRun class
    int sendData(int integerData); // not sure about return type on this either
	
	std::vector<int> generateVector(std::string python_data);

	std::string generateString(std::vector<int> vector_c);

	void generateCSV(std::string path, std::vector<int> new_data);

	std::string testingData;

    std::vector<int> getVector_element();

    void clearVector_element();

    std::vector<int> getVector_data();
    void false_isReceived();

    private:
	zmq::context_t context;
	zmq::socket_t socket;
	zmq::message_t request;
	std::vector<int> vector_data; //this is vector data file that is sent to hand c++ so it can process it.
	std::vector<int> csv_vector; //it can be used in order to generate csv file.
	bool isReceieved;
	int toPython;
    std::vector<int> vector_element;
};
#endif
