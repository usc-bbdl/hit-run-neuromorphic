// AdeptArmAPI.cpp
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x501
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <io.h>
#include <regex>
#include "AdeptArmAPI.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512

// Error printing helper function
void error(const char *msg){
    perror(msg);
    exit(0);
}


// Input check function
bool AdeptArmAPI::inputCheck (std::string s) {
    /*std::string delimiter = ",";

    // Error check
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        // token = trim(token);

        regex num_regex("(?!-)\\W|[a-z]");
        if (regex_search(token, num_regex)) {
            std::string str = "Format error! String contains something like: " + token + "\n";
            const char * c = str.c_str();
            error(c);
            return false;
        }
        s.erase(0, pos + delimiter.length());
    }
	*/
    return true;
}
        
// Constructors 
// Constructor for default(localhost:1234)
AdeptArmAPI::AdeptArmAPI () {
    connected  = false;
    address = "172.16.184.121";
    portNumber = 1234;
	blockingFlag = true;
}
// Constructor for localhost:givenPortNumber
AdeptArmAPI::AdeptArmAPI (int inputPortNumber) {
    connected  = false;
    address = "172.16.184.121";
    portNumber = inputPortNumber;
	blockingFlag = true;
}
// Constructor for givenAddress:givenPortNumber
AdeptArmAPI::AdeptArmAPI (std::string address, int inputPortNumber) {
    connected  = false;
    address = address;
    portNumber = inputPortNumber;
	blockingFlag = true;
}

// Function to start connection and initialize buffer
bool AdeptArmAPI::connectToController() {
    WSADATA wsaData;
    ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                      *ptr = NULL,
                      hints;

    //char recvbuf[DEFAULT_BUFLEN];
    //int recvbuflen = DEFAULT_BUFLEN;
    
    // Initialize Winsock
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return false;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    
    char* portNumberBuf = new char[6];
    sprintf(portNumberBuf, "%d", portNumber);
    
    // Resolve the server address and port
    iResult = getaddrinfo(address.c_str(), portNumberBuf, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return false;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {
        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
                                ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return false;
        }

        // Connect to server.
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return false;
    }
    
    return true;
}


// Function Move
bool AdeptArmAPI::move (PPoint position) {
    std::string move_command_string = "1,";
    move_command_string.append(position.toString());
    move_command_string.append("!");

    bool check = inputCheck(move_command_string);
    if (check) {
        sendStr(move_command_string);
        return true;
    } else {
        return false;
    }
}


// Function set velocity
void AdeptArmAPI::setVelocity (long double velocity, long double rotationVelocity, VelocityType type, bool always) {
    this->velocity = velocity;
	this->rotationalVelocity = rotationVelocity;
	std::string set_velocity_command_string = "2," + std::to_string(velocity) + "," + std::to_string(rotationVelocity) + ",";

	switch (type) {
	case MONITOR :
		set_velocity_command_string.append("0,");
		break;
	case INCHES_PER_SECOND :
		set_velocity_command_string.append("1,");
		break;
	case MILLIMETERS_PER_SECOND :
		set_velocity_command_string.append("2,");
		break;
	default :
		break;
	}

	if (always) {
		set_velocity_command_string.append("0!");
	}
	else {
		set_velocity_command_string.append("1!");
	}

	bool check = inputCheck(set_velocity_command_string);
	if (check) {
		sendStr(set_velocity_command_string);
	}
}


// Function set origin
void AdeptArmAPI::setOrigin (PPoint position) {
    if (this->move(position)) {
        origin = position;
    }
}

void AdeptArmAPI::moveToOrigin () {
	move(origin);
}

void AdeptArmAPI::setBlocking(bool shouldBlock) {
	this->blockingFlag = shouldBlock;
	std::string command = "3,";
	std::string block = shouldBlock ? "1" : "0";
	command.append(block);
	sendStr(command);
}

void AdeptArmAPI::sendStr(std::string input) {
    const char* sendbuf = input.c_str();
                
    int iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return;
    }
    printf("Bytes Sent: %ld\n", iResult);
	
	if (blockingFlag) {
		char recvbuf[10];
		iResult = recv(ConnectSocket, recvbuf, 10, 0);
		if ( iResult > 0 ) {
			printf("Bytes received: %d\n", iResult);
			std::cout << "Received: " << recvbuf << std::endl;
		} else if ( iResult == 0) {
			printf("No bytes received\n");
		} else {
			printf("Recv failed with error: %d\n", WSAGetLastError());
		}
	}
}

bool AdeptArmAPI::disconnectFromServer () {
    
    // send a empty string to server and server will disconnect 
    // (in Jerry's testing server code)
    std::string disconnectSignal = "0";
    const char* sendbuf = disconnectSignal.c_str();
    int iResult = send(ConnectSocket, sendbuf, 1, 0 );
    
    // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return false;
    }
    
    return true;
}


// Main function to test the calls

int main(int argc, char **argv) 
{
    AdeptArmAPI client;
    bool success = client.connectToController();
    

    std::string sendbufStr;
    char *sendbuf;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    int iResult;

    if (success){
        do {
            std::cout << "Please enter the message: ";
            getline(std::cin, sendbufStr);        
            std::cout << "Input is:" << sendbufStr << std::endl;

			if (sendbufStr == "0") {
				break;
			}
    
			PPoint point;
			point.x = 0;
			point.y = 0;
			point.z = 0;
			point.a = 0;
			point.b = -5;
			point.c = 0;
            //client.move(point);

			PPoint point2;
			point2.x = 0;
			point2.y = -30;
			point2.z = 30;
			point2.a = 0;
			point2.b = -5;
			point2.c = 0;
			client.move(point2);

			PPoint point3;
			
			point3.x = 10;
			point3.y = -40;
			point3.z = 0;
			point3.a = 0;
			point3.b = -5;
			point3.c = 0;

			client.move(point3);

			client.move(point);
            
        } while ( sendbufStr!="quit");
        
        client.disconnectFromServer();
    }
    
    // cleanup
    closesocket(client.ConnectSocket);
    WSACleanup();

    return 0;
}