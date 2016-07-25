#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <sstream>
#include "IPC.h"
#include "zmq.hpp"
#include <string>
#include <iostream>
#include <fstream>
#ifndef _WIN32
#include <unistd.h>
#else

#define sleep(n)  Sleep(n)
#endif

void IPC::startServer(){

    socket.bind ("tcp://*:5555");
    while (true) {
      zmq::message_t request;

      // Wait for next request from client
      socket.recv (&request);

      //getting string type(json) from python and creating vector of integer
      //so it can get processed with c++
      //vector_data is private vector that can be accessed through hands code
      //vector_data is private and accessed by hand code
      isReceieved = true;
      vector_data = generateVector(std::string(static_cast<char*>(request.data()), request.size()));

      std::string outMessage = generateString(vector_data);
      testingData = outMessage;
      //Communication between IPC and hit and run starts from here
      //getting csv file from hit and run
      //getting the signal so it knows if csv file was updated.
      //eventually it sends string to Hit and Run and get the result from it.
      std::cout<<outMessage<<std::endl;
      
      sleep(1);
      //wait until getting the signal from hit and run with measured data

      outMessage = std::to_string(toPython);

      // Send the string to python
      zmq::message_t reply (outMessage.size());
      const void * a = outMessage.c_str();
      memcpy (reply.data(), a, outMessage.size());
      socket.send(reply);
      isReceieved = false; // reset it false  
    }
}
/**
  * isServerWorking
  * Return true if we are connected to the Python server then send
  * that answer to establishConnection in HitRun.
  */ 
bool IPC::isServerWorking(){
  // running time measurement for recursive
    clock_t Rstart, Rfinish;
    double Rdur;

    Rstart = clock();//start time
   
    //sending message
    std::string heartbeat = "heartbeat";
    zmq::message_t H_Message (heartbeat.size());
    const void * a = heartbeat.c_str();
    memcpy (H_Message.data(), a, heartbeat.size());
    socket.send(H_Message);


    Rfinish = clock();//ending time

    Rdur = (double)(Rfinish - Rstart);
    Rdur /= (CLOCKS_PER_SEC);
    std::cout << "running time in recursive : " << Rdur << std::endl;
}
/**
  * wasDataReceived
  * Return true if we have received our data from the Python server.
  */
bool IPC::wasDataReceived(){
    return isReceieved;
}
/**
  * getData
  * Get the data from the Python server and store it as a vector.
  * Possibly return the vector.......???
  */
std::vector<int> IPC::getData(){
  //when IPC get the message(data) from python,
  //IPC also process it and send it to HIT AND RUN
  return vector_data;
}

/**
  * sendData
  * Gets the data from sendVector in HitRun and sends it to the 
  * Python server. 
  */
int IPC::sendData(int integerData){
  //when HIT AND RUN send the data
  toPython = integerData;
}
std::vector<int> IPC::generateVector(std::string python_data) {

    std::vector<int> data;
    for(std::string::iterator it = python_data.begin(); it != python_data.end(); ++it) {
        if(isdigit(*it)) {
            int element = *it - '0';
             data.push_back(element);
        }
    }
    return data;
}
std::string IPC::generateString(std::vector<int> vector_c) {
    
    std::stringstream ss;
     for(int i = 0; i < vector_c.size(); i++) {
       if(i != vector_c.size()-1){
           ss << vector_c[i] << ",";
       }else{
           ss << vector_c[i];
       }
     }
     // Do some 'work'
     std::string outMessage;
     ss >> outMessage;

     return outMessage;
}
void IPC::generateCSV(std::string path, std::vector<int> new_data) {

      std::ofstream csv_file;
      csv_file.open (path);
      for(int i = 0; i < new_data.size(); i++) {
        csv_file << new_data[i] << ",";
        if(i == (new_data.size()-1)) {
          csv_file << "\n";
        } 
      }

      csv_file.close();
}