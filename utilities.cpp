#include <utilities.h>
#include <conio.h>
#include <dataOneSample.h>
#include <motorControl.h>
#include <HitRunconnection.h>

int proceedState(int *state)
{
    static dataOneSample loadCellOffsets;
    static motorControl motors(loadCellOffsets.loadCellOffset);
	static HitRunconnection h_r_Con(& motors);
    static int connectionPass;
    switch(*state)
    {
    case MOTOR_STATE_INIT:
        printf("Motors Winding Up; Next stage is Closed-Loop\n");
        motors.motorEnable();
        motors.motorWindUp();
        *state = MOTOR_STATE_WINDING_UP;
        break;
    case MOTOR_STATE_WINDING_UP:
        //Start controlling muscle force
        motors.motorControllerStart();
        Sleep(1000);
        printf("Closed-Loop ; Next stage is Establishing Connection to Hit and Run\n");
        *state = MOTOR_STATE_CLOSED_LOOP;
        break;
    case MOTOR_STATE_CLOSED_LOOP:
        printf("Establishing Connection to Hit and Run.\n");
		connectionPass = h_r_Con.establishConnection();
        if (connectionPass){
			printf("Connection successful.\n");
			printf("Next Stage is to run the experiment.\n");
			*state = MOTOR_STATE_RUN_EXPERIMENT;
		}
		else{
			printf("Connection failed.\n");
			*state = MOTOR_STATE_SHUTTING_DOWN;
		}
        Sleep(500);
        break;
	case MOTOR_STATE_RUN_EXPERIMENT:
        //Start controlling muscle force
        h_r_Con.startConnection();
        Sleep(1000);
        printf("Running the Experiment.\n");
		printf("Next Stage is to Shut Down.\n");
        *state = MOTOR_STATE_SHUTTING_DOWN;
        break;
    case MOTOR_STATE_SHUTTING_DOWN:
        printf("Shutting Down\n");
        motors.motorControllerEnd();
        printf("Press Enter to Exit\n");
        *state = EXIT;
        exit(0);   // Exit The Program
        break;
    }
    return 0;
}
