#include "Server.h"
#include "iostream"
#include <cstring>
#include <fstream>
#include <iostream>
using namespace std;

using namespace Aris::Core;


double GaitTxtTraj[GAIT_RANDOM_LEN][GAIT_WIDTH];
double GAIT_TXT_TRAJ_LEN;


//static int LastCMD=-1;

CONN ControlSystem, VisualSystem;

//#include "Hexapod_Robot.h"

// CONN call back functions
int On_CS_ConnectionReceived(Aris::Core::CONN *pConn, const char* addr,int port)
{
	Aris::Core::MSG msg;

		msg.SetMsgID(CS_Connected);
		msg.SetLength(sizeof(port));
		msg.Copy(&port,sizeof(port));
		msg.CopyMore(addr,strlen(addr));
		PostMsg(msg);
	return 0;

}
int On_CS_DataReceived(Aris::Core::CONN *pConn, Aris::Core::MSG &data)
{
	    int cmd;

	    data.PasteAt(&cmd,sizeof(int),0);

        MSG CMD(data);

         CMD.SetMsgID(CS_CMD_Received);
         bool IsCMDExecutable=true;

 		cout<<"received CMD is"<<cmd<<endl;
		PostMsg(CMD);
 		return 0;
}
int On_CS_ConnectionLost(Aris::Core::CONN *pConn)
{
	PostMsg(Aris::Core::MSG(CS_Lost));
	return 0;
}

//MSG call back functions
int On_CS_Connected(Aris::Core::MSG &msg)
{
	cout<<"Received Connection from Control System:"<<endl;
	cout<<"   Remote IP is: "<<msg.GetDataAddress()+sizeof(int)<<endl;
	cout<<"   Port is     : "<<*((int*)msg.GetDataAddress()) << endl << endl;

	Aris::Core::MSG data(0,0);
	ControlSystem.SendData(data);
	return 0;
}

int On_CS_CMD_Received(Aris::Core::MSG &msg)
{
	bool IsCMDOK=true;


	MSG Command(msg);
	Command.SetMsgID(GetControlCommand);
	int cmd;
	Command.PasteAt(&cmd,sizeof(int),0);
	if(cmd==11)//read traj from a txt file
	{
		int NameLength;
		Command.PasteAt(&NameLength,sizeof(int),sizeof(int));
    	cout<<"NameLength: "<<NameLength<<endl;

 	   char TrajFile[NameLength+20];


       const char pathHeader[21]="../../resource/gait/";
       memcpy(TrajFile,pathHeader,20);

       for(int i=0;i<NameLength;i++)
       {
    	   Command.PasteAt(TrajFile+i+20,sizeof(char),sizeof(NameLength)+sizeof(cmd)+i);
       }

       TrajFile[NameLength+20]='\0';

       ifstream fin;

       fin.open(TrajFile);

       if(!fin.is_open())
       {
    	   IsCMDOK=false;
           cout<<"FILE NAME DOES NOT EXIST."<<endl;
           fin.close();
           GAIT_TXT_TRAJ_LEN=0;
       }
       else
       {
           cout<<"TrajFile path get:";

           for(int i=0;i<NameLength+20;i++)
           {
         	  cout<<TrajFile[i];
           }
           cout<<endl;


       	   IsCMDOK=true;
       	   cout<<"File opened."<<endl;
       	   int Line_Num;
       	   double temp;

       	   int Length=0;

       	   for(int i=0; i<GAIT_RANDOM_LEN;i++)
       	   {
       		   fin>>Line_Num;

        		if(fin.eof()==false)
       		    {
        			for(int j=0;j<GAIT_WIDTH;j++)

       		        {
       		             if(fin.eof()==false)
       		            {
       		             fin>>temp;
       		             if(i==0)
       		             {
       		               cout<<"gait first line"<<temp<<endl;
       		             }
        		                 GaitTxtTraj[i][j]=(int)temp;
       		             }
       		         }
        		     Length+=1;
       		      }
       		}

           GAIT_TXT_TRAJ_LEN=Length;
           cout<<"gait Length:"<<GAIT_TXT_TRAJ_LEN<<endl;
      	   fin.close();
       }
	}

    if(IsCMDOK)
    {
    	//cout<<"From cmd received call back function. Gait Needed is: "<<Command.GetMsgID()<<endl;
    	PostMsg(Command);
    }



	MSG data(0,0);
	ControlSystem.SendData(data);
	return 0;
}

int On_CS_Lost(Aris::Core::MSG &msg)
{
		cout << "Control system connection lost" << endl;
		ControlSystem.StartServer("5690");
		return 0;
}


