/*
 * ac_test.cpp

 *
 *  Created on: Nov 26, 2014
 *      Author: leo
 */
#include <iostream>
//#include "Aris_ControlData.h"
#include "Aris_Control.h"
#include "Aris_Message.h"
#include "Aris_Thread.h"
#include "Aris_Socket.h"
#include "Server.h"

using namespace std;
using namespace Aris::RT_CONTROL;

static CGait gait;
static EGAIT gaitcmd[AXIS_NUMBER];
static EGAIT gaitcmdtemp[AXIS_NUMBER];

Aris::RT_CONTROL::ACTUATION cs;
//CSysBase sysbase;
Aris::RT_CONTROL::CSysInitParameters initParam;

 enum MACHINE_CMD
 {
     NOCMD=1000,
	 POWEROFF=1001,
	 STOP=1002,
	 ENABLE=1003,
	 RUNNING=1004,
	 GOHOME_1=1005,
	 GOHOME_2=1006,
	 HOME2START_1=1007,
	 HOME2START_2=1008,
 	 TEST_TRAJ=1009,
 	 BACK_TO_HOME=1010,
 	 GO_TXT_TRAJ=1011,
 	 FORWARD=1012,
 	 SLOW_AND_STOP=1013,
 	 BACKWARD=1014,

 };

 int count;

/*
 * Trajectory Generator
 */

 void* Thread2(void *)
 {
	 cout<<"running msgloop"<<endl;
 	Aris::Core::RunMsgLoop();
 	return NULL;
 };


int initFun(Aris::RT_CONTROL::CSysInitParameters& param)
{
	gait.InitGait(param);
	return 0;
};

int tg(Aris::RT_CONTROL::CMachineData& machineData,Aris::Core::RT_MSG& msgRecv,Aris::Core::RT_MSG& msgSend)
{
	static int tg_count;
	if(tg_count%1000==0)
	{
		msgSend.SetMsgID(DataFromRT);
	    msgSend.SetLength(sizeof(int)*5*18);

	    for(int i=0;i<18;i++)
	    {
	    	msgSend.CopyAt(&machineData.motorsStates[i],sizeof(int),sizeof(int)*(5*i));
	    	msgSend.CopyAt(&machineData.motorsModesDisplay[i],sizeof(int),sizeof(int)*(5*i+1));
	    	msgSend.CopyAt(&machineData.feedbackData[i].Position,sizeof(int),sizeof(int)*(5*i+2));
	    	msgSend.CopyAt(&machineData.feedbackData[i].Velocity,sizeof(int),sizeof(int)*(5*i+3));
	    	msgSend.CopyAt(&machineData.feedbackData[i].Torque,sizeof(int),sizeof(int)*(5*i+4));
	    }

	    cs.RT_PostMsg(msgSend);
	}

	tg_count++;
 	int CommandID;

	 CommandID=msgRecv.GetMsgID();
 	switch(CommandID)
	{
	case NOCMD:
		for(int i=0;i<18;i++)
		{
	    	machineData.motorsCommands[i]=EMCMD_NONE;

		}
		rt_printf("NONE Command Get in NRT\n" );

	break;

	case ENABLE:
		 for(int i=0;i<18;i++)
		 {
			machineData.motorsCommands[i]=EMCMD_ENABLE;
		 }
 		rt_printf("ENABLE Command Get in NRT\n" );

		break;
	case POWEROFF:
		 for(int i=0;i<18;i++)
		 {
			machineData.motorsCommands[i]=EMCMD_POWEROFF;
			gait.IfReadytoSetGait(false,i);
		 }
		rt_printf("POWEROFF Command Get in NRT\n" );

		break;
	case STOP:
		 for(int i=0;i<18;i++)
		{
			machineData.motorsCommands[i]=EMCMD_STOP;
 		}
 		 rt_printf("STOP Command Get in NRT\n" );

		break;
	case RUNNING:
	 	for(int i=0;i<18;i++)
	 	{
			machineData.motorsCommands[i]=EMCMD_RUNNING;
 			gait.IfReadytoSetGait(true,i);
	 	}
		rt_printf("RUNNING Command Get in NRT\n" );
		break;

	case GOHOME_1:

			machineData.motorsCommands[MapAbsToPhy[0]]=EMCMD_GOHOME;
			machineData.motorsCommands[MapAbsToPhy[1]]=EMCMD_GOHOME;
			machineData.motorsCommands[MapAbsToPhy[2]]=EMCMD_GOHOME;
			machineData.motorsCommands[MapAbsToPhy[6]]=EMCMD_GOHOME;
			machineData.motorsCommands[MapAbsToPhy[7]]=EMCMD_GOHOME;
			machineData.motorsCommands[MapAbsToPhy[8]]=EMCMD_GOHOME;
			machineData.motorsCommands[MapAbsToPhy[12]]=EMCMD_GOHOME;
			machineData.motorsCommands[MapAbsToPhy[13]]=EMCMD_GOHOME;
			machineData.motorsCommands[MapAbsToPhy[14]]=EMCMD_GOHOME;

 			gaitcmd[MapAbsToPhy[0]]=EGAIT::GAIT_HOME;
 			gaitcmd[MapAbsToPhy[1]]=EGAIT::GAIT_HOME;
 			gaitcmd[MapAbsToPhy[2]]=EGAIT::GAIT_HOME;
 			gaitcmd[MapAbsToPhy[6]]=EGAIT::GAIT_HOME;
 			gaitcmd[MapAbsToPhy[7]]=EGAIT::GAIT_HOME;
 			gaitcmd[MapAbsToPhy[8]]=EGAIT::GAIT_HOME;
 			gaitcmd[MapAbsToPhy[12]]=EGAIT::GAIT_HOME;
 			gaitcmd[MapAbsToPhy[13]]=EGAIT::GAIT_HOME;
 			gaitcmd[MapAbsToPhy[14]]=EGAIT::GAIT_HOME;

		rt_printf("GOHOME_1 Command Get in NRT\n" );

		break;

	case GOHOME_2:

		machineData.motorsCommands[MapAbsToPhy[3]]=EMCMD_GOHOME;
		machineData.motorsCommands[MapAbsToPhy[4]]=EMCMD_GOHOME;
		machineData.motorsCommands[MapAbsToPhy[5]]=EMCMD_GOHOME;
		machineData.motorsCommands[MapAbsToPhy[9]]=EMCMD_GOHOME;
		machineData.motorsCommands[MapAbsToPhy[10]]=EMCMD_GOHOME;
		machineData.motorsCommands[MapAbsToPhy[11]]=EMCMD_GOHOME;
		machineData.motorsCommands[MapAbsToPhy[15]]=EMCMD_GOHOME;
		machineData.motorsCommands[MapAbsToPhy[16]]=EMCMD_GOHOME;
		machineData.motorsCommands[MapAbsToPhy[17]]=EMCMD_GOHOME;

			gaitcmd[MapAbsToPhy[3]]=EGAIT::GAIT_HOME;
			gaitcmd[MapAbsToPhy[4]]=EGAIT::GAIT_HOME;
			gaitcmd[MapAbsToPhy[5]]=EGAIT::GAIT_HOME;
			gaitcmd[MapAbsToPhy[9]]=EGAIT::GAIT_HOME;
			gaitcmd[MapAbsToPhy[10]]=EGAIT::GAIT_HOME;
			gaitcmd[MapAbsToPhy[11]]=EGAIT::GAIT_HOME;
			gaitcmd[MapAbsToPhy[15]]=EGAIT::GAIT_HOME;
			gaitcmd[MapAbsToPhy[16]]=EGAIT::GAIT_HOME;
			gaitcmd[MapAbsToPhy[17]]=EGAIT::GAIT_HOME;


		rt_printf("GOHOME_2 Command Get in NRT\n" );

		break;

	case HOME2START_1:

		if(gait.m_gaitState[MapAbsToPhy[0]]==GAIT_STOP)
		{
		    for(int i=0;i<18;i++)
		    {
				machineData.motorsModes[i]=EOperationMode::OM_CYCLICVEL;
		    }
				gaitcmd[MapAbsToPhy[0]]=EGAIT::GAIT_HOME2START;
				gaitcmd[MapAbsToPhy[1]]=EGAIT::GAIT_HOME2START;
				gaitcmd[MapAbsToPhy[2]]=EGAIT::GAIT_HOME2START;
				gaitcmd[MapAbsToPhy[6]]=EGAIT::GAIT_HOME2START;
				gaitcmd[MapAbsToPhy[7]]=EGAIT::GAIT_HOME2START;
				gaitcmd[MapAbsToPhy[8]]=EGAIT::GAIT_HOME2START;
				gaitcmd[MapAbsToPhy[12]]=EGAIT::GAIT_HOME2START;
				gaitcmd[MapAbsToPhy[13]]=EGAIT::GAIT_HOME2START;
				gaitcmd[MapAbsToPhy[14]]=EGAIT::GAIT_HOME2START;

			rt_printf("HOME2START_1 Command Get in NRT\n" );
		}

		break;

	case HOME2START_2:

		if(gait.m_gaitState[MapAbsToPhy[3]]==GAIT_STOP)
		{
		    for(int i=0;i<18;i++)
		    {
				machineData.motorsModes[i]=EOperationMode::OM_CYCLICVEL;
		    }

			gaitcmd[MapAbsToPhy[3]]=EGAIT::GAIT_HOME2START;
			gaitcmd[MapAbsToPhy[4]]=EGAIT::GAIT_HOME2START;
			gaitcmd[MapAbsToPhy[5]]=EGAIT::GAIT_HOME2START;
			gaitcmd[MapAbsToPhy[9]]=EGAIT::GAIT_HOME2START;
			gaitcmd[MapAbsToPhy[10]]=EGAIT::GAIT_HOME2START;
			gaitcmd[MapAbsToPhy[11]]=EGAIT::GAIT_HOME2START;
			gaitcmd[MapAbsToPhy[15]]=EGAIT::GAIT_HOME2START;
	 		gaitcmd[MapAbsToPhy[16]]=EGAIT::GAIT_HOME2START;
			gaitcmd[MapAbsToPhy[17]]=EGAIT::GAIT_HOME2START;

			rt_printf("HOME2START_2 Command Get in NRT\n" );
		}

		break;

	case TEST_TRAJ:

		if(gait.m_gaitState[MapAbsToPhy[0]]==GAIT_STOP)
		{
		    for(int i=0;i<18;i++)
		    {
		      machineData.motorsModes[i]=EOperationMode::OM_CYCLICVEL;
		      gaitcmd[MapAbsToPhy[i]]=EGAIT::GAIT_TEST_TRAJ;
		      machineData.motorsCommands[i]=EMCMD_RUNNING;

		    }
		}
		break;

	case BACK_TO_HOME:
		if(gait.m_gaitState[MapAbsToPhy[0]]==GAIT_STOP)
		  {
		    for(int i=0;i<18;i++)
		      {
			machineData.motorsModes[i]=EOperationMode::OM_CYCLICVEL;
			gaitcmd[MapAbsToPhy[i]]=EGAIT::GAIT_BACK_TO_HOME;
			machineData.motorsCommands[i]=EMCMD_RUNNING;
		      }
		  }
		break;
	case GO_TXT_TRAJ:
		if(gait.m_gaitState[MapAbsToPhy[0]]==GAIT_STOP)
		  {
		    for(int i=0;i<18;i++)
		      {
			machineData.motorsModes[i]=EOperationMode::OM_CYCLICVEL;
			gaitcmd[MapAbsToPhy[i]]=EGAIT::GAIT_GO_TXT_TRAJ;
			machineData.motorsCommands[i]=EMCMD_RUNNING;
		      }
		  }
		break;

	case FORWARD:
		if(gait.m_gaitState[MapAbsToPhy[0]]==GAIT_STOP)
		  {
		    for(int i=0;i<18;i++)
		      {
			machineData.motorsModes[i]=EOperationMode::OM_CYCLICVEL;
			gaitcmd[MapAbsToPhy[i]]=EGAIT::GAIT_FORWARD_TRAJ;
			machineData.motorsCommands[i]=EMCMD_RUNNING;
		      }
		  }
		break;


	case SLOW_AND_STOP:

		if(gait.m_gaitState[MapAbsToPhy[0]]!=GAIT_STOP)
		{
			for(int i=0;i<18;i++)
			{
				gait.IsConsFinished[i]=true;

			}
		}

		break;


	case BACKWARD:
		if(gait.m_gaitState[MapAbsToPhy[0]]==GAIT_STOP)
		  {
		    for(int i=0;i<18;i++)
		      {
			machineData.motorsModes[i]=EOperationMode::OM_CYCLICVEL;
			gaitcmd[MapAbsToPhy[i]]=EGAIT::GAIT_BACKWARD_TRAJ;
			machineData.motorsCommands[i]=EMCMD_RUNNING;
		      }
		  }
		break;




	   // rt_printf("driver 0 gaitcmd:%d\n",gaitcmd[0]);

 	/*case BACKWARD:
	    for(int i=0;i<18;i++)
		 {
 			machineData.motorsModes[i]=EOperationMode::OM_CYCLICVEL;
			gaitcmdtemp[i]=EGAIT::GAIT_MOVE_BACK;
			machineData.motorsCommands[i]=EMCMD_RUNNING;


		 if(gait.m_gaitState[i]!=GAIT_STOP)
		 {
     	   gait.Gait_iter[i]=gait.Gait_iter[i]+1;
		 }
		 else
		 {
				gaitcmd[i]=gaitcmdtemp[i];
				gait.Gait_iter[i]=1;
			}
		 }

		break;

 	case FAST_FORWARD:
	    for(int i=0;i<18;i++)
		 {
 			machineData.motorsModes[i]=EOperationMode::OM_CYCLICVEL;
			gaitcmdtemp[i]=EGAIT::GAIT_FAST_MOVE;
			machineData.motorsCommands[i]=EMCMD_RUNNING;


		 if(gait.m_gaitState[i]!=GAIT_STOP)
		 {
     	   gait.Gait_iter[i]=gait.Gait_iter[i]+1;
		 }
		 else
		 {
				gaitcmd[i]=gaitcmdtemp[i];
				gait.Gait_iter[i]=1;
			}

		 }

	   // rt_printf("driver 0 gaitcmd:%d\n",gaitcmd[0]);


		break;
 	case FAST_BACKWARD:
	    for(int i=0;i<18;i++)
		 {
 			machineData.motorsModes[i]=EOperationMode::OM_CYCLICVEL;
			gaitcmdtemp[i]=EGAIT::GAIT_FAST_MOVE_BACK;
			machineData.motorsCommands[i]=EMCMD_RUNNING;


		 if(gait.m_gaitState[i]!=GAIT_STOP)
		 {
     	   gait.Gait_iter[i]=gait.Gait_iter[i]+1;
		 }
		 else
		 {
				gaitcmd[i]=gaitcmdtemp[i];
				gait.Gait_iter[i]=1;
			}

		 }


		break;
	case LEGUP:

		if(gait.m_gaitState[MapAbsToPhy[0]]==GAIT_STOP)
		{
		    for(int i=0;i<18;i++)
		    {
				machineData.motorsModes[i]=EOperationMode::OM_CYCLICVEL;
				gaitcmd[MapAbsToPhy[i]]=EGAIT::GAIT_LEGUP;
				machineData.motorsCommands[i]=EMCMD_RUNNING;

		    }
		}
		break;
	case TURNLEFT:

		if(gait.m_gaitState[MapAbsToPhy[0]]==GAIT_STOP)
		{
		    for(int i=0;i<18;i++)
		    {
				machineData.motorsModes[i]=EOperationMode::OM_CYCLICVEL;
				gaitcmd[MapAbsToPhy[i]]=EGAIT::GAIT_TURN_LEFT;
				machineData.motorsCommands[i]=EMCMD_RUNNING;

		    }
		}
		break;
	case TURNRIGHT:

		if(gait.m_gaitState[MapAbsToPhy[0]]==GAIT_STOP)
		{
		    for(int i=0;i<18;i++)
		    {
				machineData.motorsModes[i]=EOperationMode::OM_CYCLICVEL;
				gaitcmd[MapAbsToPhy[i]]=EGAIT::GAIT_TURN_RIGHT;
				machineData.motorsCommands[i]=EMCMD_RUNNING;

		    }
		}
		break;
	case ONLINEGAIT:

		if(gait.m_gaitState[MapAbsToPhy[0]]==GAIT_STOP)
		{
		    for(int i=0;i<18;i++)
		    {
				machineData.motorsModes[i]=EOperationMode::OM_CYCLICVEL;
				gaitcmd[MapAbsToPhy[i]]=EGAIT::GAIT_ONLINE;
				machineData.motorsCommands[i]=EMCMD_RUNNING;
		    }
		}
		break;*/

	default:
		//DO NOTHING, CMD AND TRAJ WILL KEEP STILL
 		break;
	}

     gait.RunGait(gaitcmd,machineData);

return 0;

};

//offsets driver order
static int HEXBOT_HOME_OFFSETS_RESOLVER[18] =
{
		0,0,0,
		0,0,0,
		0,0,0,
		0,0,0,
		0,0,0,
		0,0,0
};


int OnGetControlCommand(Aris::Core::MSG &msg)
{
    int CommandID;
    msg.Paste(&CommandID,sizeof(int));
    Aris::Core::MSG data;

    switch(CommandID)
    {
    case 1:
		data.SetMsgID(POWEROFF);
		cs.NRT_PostMsg(data);
		break;
    case 2:
    	data.SetMsgID(STOP);
		cs.NRT_PostMsg(data);
    	break;
    case 3:
    	data.SetMsgID(ENABLE);
		cs.NRT_PostMsg(data);
    	break;
    case 4:
    	data.SetMsgID(RUNNING);
		cs.NRT_PostMsg(data);
    	break;
    case 5:
    	data.SetMsgID(GOHOME_1);
		cs.NRT_PostMsg(data);
    	break;
    case 6:
    	data.SetMsgID(GOHOME_2);
		cs.NRT_PostMsg(data);
    	break;
    case 7:
    	data.SetMsgID(HOME2START_1);
		cs.NRT_PostMsg(data);
    	break;
    case 8:
    	data.SetMsgID(HOME2START_2);
		cs.NRT_PostMsg(data);
    	break;
    case 9:
    	data.SetMsgID(TEST_TRAJ);
		cs.NRT_PostMsg(data);
    	break;
    case 10:
    	data.SetMsgID(BACK_TO_HOME);
		cs.NRT_PostMsg(data);
    	break;
    case 11:
    	data.SetMsgID(GO_TXT_TRAJ);
    	cs.NRT_PostMsg(data);
    	break;
    case 12:
        data.SetMsgID(FORWARD);
    	cs.NRT_PostMsg(data);
        break;
    case 13:
    	data.SetMsgID(SLOW_AND_STOP);
    	cs.NRT_PostMsg(data);
    	break;


    /*case 10:
    	data.SetMsgID(BACKWARD);
		cs.NRT_PostMsg(data);
    	break;
    case 11:
    	data.SetMsgID(FAST_FORWARD);
		cs.NRT_PostMsg(data);
    	break;
    case 12:
    	data.SetMsgID(FAST_BACKWARD);
		cs.NRT_PostMsg(data);
    	break;
    case 13:
    	data.SetMsgID(LEGUP);
		cs.NRT_PostMsg(data);
    	break;
    case 14:
    	data.SetMsgID(TURNLEFT);
		cs.NRT_PostMsg(data);
    	break;
    case 15:
    	data.SetMsgID(TURNRIGHT);
		cs.NRT_PostMsg(data);
    	break;
    case 16:
    	data.SetMsgID(ONLINEGAIT);
    	cs.NRT_PostMsg(data);
    	break;*/


    default:
		printf("Hi! I didn't get validate cmd\n");
        break;

    }
    return CommandID;

};
//static int driverIDs[18]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17};

int main(int argc, char** argv)
{

    Aris::Core::RegisterMsgCallback(CS_Connected,On_CS_Connected);
    Aris::Core::RegisterMsgCallback(CS_CMD_Received,On_CS_CMD_Received);
    Aris::Core::RegisterMsgCallback(CS_Lost,On_CS_Lost);
    Aris::Core::RegisterMsgCallback(GetControlCommand,OnGetControlCommand);
    Aris::Core::RegisterMsgCallback(DataFromRT,OnDataFromRT);



//   CONN call back
	/*设置所有CONN类型的回调函数*/
   ControlSystem.SetCallBackOnReceivedConnection(On_CS_ConnectionReceived);
    //VisualSys.SetCallBackOnReceivedConnection(OnConnectReceived_VS);

	ControlSystem.SetCallBackOnReceivedData(On_CS_DataReceived);
      //  VisualSys.SetCallBackOnReceivedData(OnSysDataReceived_VS);

    ControlSystem.SetCallBackOnLoseConnection(On_CS_ConnectionLost);
      //  VisualSys.SetCallBackOnLoseConnection(OnConnectionLost_VS);

    ControlSystem.StartServer("5690");
     // VisualSys.StartServer("5691");
	Aris::Core::THREAD T1,T2,T3 ;
 	 T2.SetFunction(Thread2);
  // cs.Load_XML_PrintMessage();
     T2.Start(0);

	cs.SetSysInitializer(initFun);
	cs.SetTrajectoryGenerator(tg);

	initParam.motorNum=18;
	initParam.homeHighSpeed=100000;
	initParam.homeLowSpeed=10000;
	initParam.homeAccel=100000;
	initParam.homeMode=-1;
	//initParam.homeTorqueLimit=0;


	////necessary steps
	initParam.homeOffsets=HEXBOT_HOME_OFFSETS_RESOLVER;
  	cs.SysInit(initParam);

	cs.SysInitCommunication();

	cs.SysStart();

	printf("Will start\n");
	while(!cs.IsSysStopped())
	{

		count++;
		sleep(1);
	}


	return 0;

};


