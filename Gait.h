/*
 * Gait.h
 *
 *  Created on: Nov 28, 2014
 *      Author: hex
 */
#ifndef GAIT_H_
#define GAIT_H_
#include "Aris_Control.h"
#include "Aris_ControlData.h"

#include <string>
using namespace std;

#define GAIT_WIDTH 18
//acc 2001 con 4000 dec2000
/*#define GAIT_ACC_LEN 2001
#define GAIT_CON_LEN 4000
#define GAIT_DEC_LEN 2000

#define GAIT_FAST_ACC_LEN 1850
#define GAIT_FAST_CON_LEN 3700
#define GAIT_FAST_DEC_LEN 1850
//2.3s
#define GAIT_TROT_ACC_LEN 3150
#define GAIT_TROT_CON_LEN 2300
#define GAIT_TROT_DEC_LEN 3150


#define GAIT_MOVE_LEN 8001
#define GAIT_MOVEBACK_LEN 8001

#define GAIT_FAST_MOVE_LEN 7400
#define GAIT_FAST_MOVE_BACK_LEN 7400

#define GAIT_HOME2START_LEN 4001
#define GAIT_LEGUP_LEN 12201
#define GAIT_TURN_LEN 6001*/

#define GAIT_HOME2START_LEN 2000
#define GAIT_TESTTRAJ_LEN  9001

#define GAIT_RANDOM_LEN 50000


#define GAIT_FORWARD_ACC_LEN 20000
#define GAIT_FORWARD_CON_LEN 2000
#define GAIT_FORWARD_DEC_LEN 20000


#define GAIT_BACKWARD_ACC_LEN 5000
#define GAIT_BACKWARD_CON_LEN 5000
#define GAIT_BACKWARD_DEC_LEN 5000




extern double GAIT_TXT_TRAJ_LEN;
extern double GaitTxtTraj[GAIT_RANDOM_LEN][GAIT_WIDTH];


//
#define MOTOR_NUM 18


enum EGaitState
{
	NONE,
	GAIT_START,
	GAIT_RUN,
	GAIT_STOP,
};

enum EGAIT
{
	GAIT_NULL=0,
	GAIT_STANDSTILL=1,
	GAIT_HOME=2,
	GAIT_HOME2START=3,
	GAIT_TEST_TRAJ=4,
	GAIT_BACK_TO_HOME=5,
	GAIT_FORWARD_TRAJ=6,
	GAIT_BACKWARD_TRAJ=7,
	GAIT_GO_TXT_TRAJ=8,



	/*GAIT_MOVE=3,
	GAIT_MOVE_BACK=4,
    GAIT_FAST_MOVE=5,
	GAIT_FAST_MOVE_BACK=6,
	GAIT_TROT=7,
	GAIT_LEGUP=8,
	GAIT_TURN_LEFT=9,
	GAIT_TURN_RIGHT=10,
	GAIT_HOME=11,
	GAIT_ONLINE=12*/
};


class CGait
{
public:
	CGait();
	~CGait();
	//read txt to array
	static int InitGait(Aris::RT_CONTROL::CSysInitParameters& param);
 	static int RunGait(EGAIT* p_gait,Aris::RT_CONTROL::CMachineData& p_data);
	static bool IsGaitFinished();
	static bool IsHomeStarted[AXIS_NUMBER];
 	static bool IsConsFinished[AXIS_NUMBER];
	static int Gait_iter[AXIS_NUMBER];
	static int Gait_iter_count[AXIS_NUMBER];
     static void  IfReadytoSetGait(bool b, int driverID);
	static EGaitState m_gaitState[AXIS_NUMBER];


private:
	static int  motorNumHomed;
    static bool isReadytoSetGait[AXIS_NUMBER];
	static EGAIT m_currentGait[AXIS_NUMBER];
	static long long int m_gaitStartTime[AXIS_NUMBER];
	static int m_gaitCurrentIndex[AXIS_NUMBER];
	static Aris::RT_CONTROL::CMotorData m_standStillData[AXIS_NUMBER];
	static Aris::RT_CONTROL::CMotorData m_commandDataMapped[AXIS_NUMBER];
	static Aris::RT_CONTROL::CMotorData m_feedbackDataMapped[AXIS_NUMBER];
	static void MapFeedbackDataIn(Aris::RT_CONTROL::CMachineData& p_data );
	static void MapCommandDataOut(Aris::RT_CONTROL::CMachineData& p_data );
};



#endif /* GAIT_H_ */
