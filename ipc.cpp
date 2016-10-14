#include <iostream>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include "ipc.h"
#include <string>
#include <fstream>
#include <random>
#include <conio.h>
#include <motorControl.h>
#include <math.h>

#ifndef _WIN32
#include <unistd.h>
#else

#define sleep(n)  Sleep(n)
#endif

void* IPC::startServer(){
    std::cout<<"IPC starts running"<<std::endl;
    socket.bind ("tcp://*:5555");
    while (true) {
      zmq::message_t request;
      
      socket.recv (&request);
      //IPC gets message from python and make it vector so that HR can process the data.
      generateVector(std::string(static_cast<char*>(request.data()), request.size()));
      isReceived = true; //ipc gets data and makes it vector and HR gets it after that

      //this is where you have to wait until  so IPC can process and sending the data.
      
      pthread_mutex_lock(&lock);       //Critical section starts
      pthread_cond_wait(&cond, &lock); // wait for HR sending vector for Python
      zmq::message_t reply (toPython.size());
      const void * a = toPython.c_str();
      memcpy (reply.data(), a, toPython.size());
      //Sleep(1000);
      socket.send(reply);
      isReceived = false; // reset it false  
      toPython = " "; // initialize string to avoid duplicate data to python
      
      pthread_mutex_unlock(&lock); // get out of critical section
    }
}
/**
  * isServerWorking
  * Return true if we are connected to the Python server then send
  * that answer to establishConnection in HitRun.
  */ 
bool IPC::isServerWorking(){
    return true;
}
/**
  * wasDataReceived
  * Return true if we have received our data from the Python server.
  */
bool IPC::wasDataReceived(){
    return isReceived;
}
/**
  * getData
  * Get the data from the Python server and store it as a vector.
  * Possibly return the vector.......???
  */
float64 * IPC::getData(){
  //when IPC get the message(data) from python,
  //IPC also process it and send it to HIT AND RUN
  return vector_data;
}

/**
  * sendData
  * Gets the data from sendVector in HitRun and sends it to the 
  * Python server. 
  */
std::string IPC::sendData(float64 data_to_python[MUSCLE_NUM]){
  //when HIT AND RUN send the data
    //formulate string form to send data through socket
    //TODO: try to formulate json object
    int counter = 1;
    pthread_mutex_lock(&lock);

    std::stringstream ss;
    ss << "{";

    for(int i = 0; i < MUSCLE_NUM+6; i++) {
       if(i != MUSCLE_NUM-1){
           ss << data_to_python[i] << ",";
       }else{
           ss << counter; 
           ++counter;
           ss << data_to_python[i];
       }
    }

    ss << "}";

    std::string outMessage;
    ss >> outMessage;
    toPython = outMessage;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&lock);
    return outMessage;
}
float64 * IPC::generateVector(std::string python_data) {
    
    int index = 0;
    std::stringstream ss;
    std::string actualData;
    for(std::string::iterator it = python_data.begin(); it != python_data.end(); ++it) {
        if(*it != '[' && *it !=']') {
            ss<<*it;
        }
    }
    actualData = ss.str();

    std::istringstream ssi(actualData);
    std::string token;
    while(std::getline(ssi, token, ',')) {
        vector_data[index] = std::stof(token);
        index++;
    }
    return vector_data;
}
std::string IPC::generateString(float64 vector_c[MUSCLE_NUM]) {
    
    std::stringstream ss;
     for(unsigned int i = 0; i < MUSCLE_NUM; i++) {
       if(i != MUSCLE_NUM-1){
           ss << vector_c[i] << ",";
       }else{
           ss << vector_c[i];
       }
     }
     std::string outMessage;
     ss >> outMessage;
     return outMessage;
}
float64 * IPC::getVector_element() {
    return vector_data;
}
void IPC::clearVector_element() {
    for(int i = 0; i < MUSCLE_NUM; i++) {
        vector_data[i] = 0.0;
    }
}
float64 * IPC::getVector_data() {
    return vector_data;
}
void IPC::false_isReceived() {
    isReceived = false;
}