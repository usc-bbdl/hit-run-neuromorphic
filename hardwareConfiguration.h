#ifndef HARDWARE_CONFIG
#define HARDWARE_CONFIG
#include <iostream>
class hardwareConfiguration
{
    bool fileOpen;
    FILE *dataFile;
public:
    char loadCellChannels[20][50];
    char JR3Channels[20][50];
    char motorChannels[20][50];
    char encoderChannels[20][50];
    int musclesAvailable;
    int loadChannels();
    hardwareConfiguration();
    ~hardwareConfiguration(void);
};
#endif
