/**
 *  Read the data file and parse it to a readable file
 */

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>


#define AXIS_NUMBER 18


extern char ServoStateName[7][20];


enum EServoState
{
    EMSTAT_NONE=0,
	EMSTAT_POWEREDOFF=1,
	EMSTAT_STOPPED=2,
	EMSTAT_ENABLED=3,
	EMSTAT_RUNNING=4,
	EMSTAT_HOMING=5,
	EMSTAT_FAULT,
	EMSTAT_INVALID,
	EMSTAT_EMERGE,
};
const int N_MOTOR_STATE=9;

enum EOperationMode
{
    OM_NOMODE     = -1,
    OM_PROFILEPOS = 1,
    OM_CYCLICPOS  = 8,
    OM_CYCLICVEL  = 9,
    OM_CYCLICTORQ = 10,
    OM_OTHER      = 18
};

enum EOperationModeDisplay
{
    OMD_NOMODE     = -1,
    OMD_HOMING     = 6,
    OMD_PROFILEPOS = 1,
    OMD_CYCLICPOS  = 8,
    OMD_CYCLICVEL  = 9,
    OMD_CYCLICTORQ = 10,
    OMD_OTHER      = 18
};
enum EServoCommand
{
	EMCMD_NONE=0,
	EMCMD_POWEROFF=1,
	EMCMD_STOP=2,
	EMCMD_ENABLE=3,
	EMCMD_RUNNING=4,
	EMCMD_GOHOME=5,
};

const int N_MOTOR_CMD=6;

enum EControl_Msg
{
	CM_PUSH_CMD_TO_MOTORS=100,
	CM_GO_TRAJ,
	CM_CUS_MESSAGE,
	CM_NONE,
};



enum EMachineState
{
	CS_UNINITED,
	CS_INITED,
	CS_COMM_INITED,
 	CS_RTTASK_STARTED,
 	CS_STOPPED,
};





/*
 * CSysInitParameters should be used by SetSysInitializer
 */
class CSysInitParameters
{

public:
	CSysInitParameters();
	~CSysInitParameters();
//	CSysInitParameters& operator=(const CSysInitParameters& other);
	int motorNum;
    int homeMode;
    int homeAccel;
    int homeLowSpeed;
    int homeHighSpeed;
    int p2pMaxSpeed;
    int p2pSpeed;
    int nsPerCyclePeriod;
    int* homeOffsets;
    int* driverIDs;
};


/*
 * CMotorData struct contain one single motor's data, used by CMachineData
 */
class CMotorData
{
public:
	CMotorData& operator=(const CMotorData& other);
	int Position;
	int Velocity;
	int Torque;
};
/*
 * CWorkData struct should be used by TrajectoryGenerator and Logger as Input Data
 *
 * may include sensor data, and other data
 *
 */

class CMachineData
{
public:
	int motorNum;
	EMachineState machinestate;

	long long int time;
	//Motor Data
	CMachineData& operator=(const CMachineData& other);

	bool isMotorHomed[AXIS_NUMBER];
    EServoState motorsStates[AXIS_NUMBER];
    EServoCommand motorsCommands[AXIS_NUMBER];
    EOperationMode motorsModes[AXIS_NUMBER];
    EOperationModeDisplay motorsModesDisplay[AXIS_NUMBER];
	CMotorData feedbackData[AXIS_NUMBER];//currentFeedback,collected after read()
	CMotorData commandData[AXIS_NUMBER];//lastCommand,collected before write()
	//sensor data

};





//using namespace ElmoMotor;

int main(int argc, char *argv[])
{
    int rawDataFileDesc;
    CMachineData logData;

    rawDataFileDesc = open("Result.dat", O_RDONLY);

    //rawDataFileDesc = open(argv[1], O_RDONLY);

    if ( rawDataFileDesc < 0 )
    {
        printf("Wrong file!\n");
	return 0;
    }
    int rb = 1;
    while(rb > 0){
        rb = read(rawDataFileDesc, &logData, sizeof(logData));
	printf("%lld",logData.time);
	for(int i=0;i<18;i++)
	{
	  printf("\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",
		 logData.feedbackData[i].Position,
		 logData.commandData[i].Position,
		 logData.feedbackData[i].Velocity,
		 logData.commandData[i].Velocity,
		 logData.feedbackData[i].Torque,
		 logData.commandData[i].Torque,
     logData.motorsStates[i],
     logData.motorsModes[i]);
	}
	printf("\n");
    }

    close(rawDataFileDesc);

    return 0;
}
