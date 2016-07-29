#include "hardwareConfiguration.h"

hardwareConfiguration::hardwareConfiguration()
{
    fileOpen = false;
    dataFile = fopen("channelNumber.txt","r");
    if (dataFile == NULL) 
    {
        printf("Could not open data file.\n");
        printf("Place channelNumber.txt in the solution directory.\n");
    }
    else
    {
        printf("Hardware configuration file successfully found.\n");
        fileOpen = true;
    }
}
int hardwareConfiguration::loadChannels(){
    if (fileOpen)
    {
        fscanf(dataFile,"%d\n",&musclesAvailable);
        char header[200];
        fscanf(dataFile,"%s\n",&header);
        for(int i=0;i<musclesAvailable;i++)
        {
            fscanf(dataFile,"%s\n",&loadCellChannels[i]);
            printf("%s\n",loadCellChannels[i] );
        }
    
        fscanf(dataFile,"%s\n",&header);
        for(int i=0;i<6;i++)
            fscanf(dataFile,"%s\n",&JR3Channels[i]);

        fscanf(dataFile,"%s\n",&header);
        for(int i=0;i<musclesAvailable;i++)
            fscanf(dataFile,"%s\n",&motorChannels[i]);

        fscanf(dataFile,"%s\n",&header);
        for(int i=0;i<musclesAvailable;i++)
            fscanf(dataFile,"%s\n",&encoderChannels[i]);
        printf("The hardware configuration file successfully loaded.\n");
    }
    else
    {
        printf("The hardware configuration file is not loaded successfully.\n");
        return 0;
    }
    return 1;
}
hardwareConfiguration::~hardwareConfiguration(){
}