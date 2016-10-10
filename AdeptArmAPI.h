// AdeptArmAPI.h
#ifndef ADEPTARMAPI_H
#define ADEPTARMAPI_H

#include <string>

struct PPoint
{
	long double x;
	long double y;
	long double z;
	long double a;
	long double b;
	long double c;

	std::string toString() {
		return std::to_string(x) + ","
                + std::to_string(y) + ","
                + std::to_string(z) + ","
                + std::to_string(a) + ","
                + std::to_string(b) + ","
                + std::to_string(c);
	}
};

enum VelocityType {
	MONITOR,
	INCHES_PER_SECOND,
	MILLIMETERS_PER_SECOND
};

class AdeptArmAPI
{
public:
    // Constructors
	AdeptArmAPI();
	AdeptArmAPI(int);
	AdeptArmAPI(std::string, int);

	// Variables keeping portNumber and connect status  
	bool connected;
    SOCKET ConnectSocket;

    // Control functions
	bool connectToController();
	bool disconnectFromServer();

	// Action functions
  	bool move(PPoint position);
	void moveToOrigin();
	
	// Set functions
  	void setVelocity(long double velocity, long double rotationVelocity, VelocityType type, bool always);
    void setOrigin(PPoint position);
	void setBlocking(bool shouldBlock);

	// Helper functions
    bool inputCheck(std::string);
    void sendStr(std::string);

private:
	std::string address;
	int portNumber;
	PPoint origin;
	double velocity;
	double rotationalVelocity;
	bool blockingFlag;
};

#endif