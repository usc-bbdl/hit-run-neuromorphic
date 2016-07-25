#ifndef IPC_H_
#define IPC_H_
#include "IPC.h"


//include these for socket programming
#include "zmq.hpp"
#include <string>
#include <iostream>
#include <fstream>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>

#define sleep(n)  Sleep(n)
#endif
// This is everything that needs to be included if I use a socket
// // Add the "Ws2_32.lib" to the LINKER settings
// #pragma comment(lib, "Ws2_32.lib")

// // STD console header files
// #include <sdkddkver.h>
// #include <conio.h>
// #include <stdio.h>

// // SOCKET header files
// #include <WinSock2.h>
// #include <Windows.h>
// #include <iostream>

// #define SCK_VERSION2 0x0202

class IPC {
private:
	zmq::context_t context;
	zmq::socket_t socket;
	zmq::message_t request;
	std::vector<int> vector_data; //this is vector data file that is sent to hand c++ so it can process it.
	std::vector<int> csv_vector; //it can be used in order to generate csv file.
	bool isReceieved = false;
	int toPython;


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

};
#endif