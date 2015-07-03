#include "Client.h"
#include <iostream>
#include <cstring>

using namespace std;

using namespace Aris::Core;


CONN ControlSysClient;

//CONN callback functions
int OnConnDataReceived(Aris::Core::CONN *pConn, Aris::Core::MSG &data)
{
	Aris::Core::PostMsg(Aris::Core::MSG(ControlCommandNeeded));
	return 0;
}

int OnConnectLost(Aris::Core::CONN *pConn)
{
	PostMsg(Aris::Core::MSG(SystemLost));
	return 0;
}


//MSG callback functions
int OnControlCommandNeeded(Aris::Core::MSG &msg)
{
    
	int cmd;
	cout<<"Commands:"<<endl;
	cout<<"1.PowerOff"<<endl
			<<"2.Stop"<<endl
			<<"3.Enable"<<endl
			<<"4.Running"<<endl
			<<"5.Gohome_1"<<endl
			<<"6.Gohome_2"<<endl
			<<"7.HOme2start_1"<<endl
			<<"8.Home2start_2"<<endl
			<<"9.Go test trajectory"<<endl
			<<"10.Back to home position"<<endl
			<<"11.Read txt file Traj"<<endl
			<<"12.Forward"<<endl
			<<"13.Slow down and stop"<<endl;
			//<<"13.Bward"<<endl;
	cout<<"Please enter your command: ";
	cin>>cmd;
	while(cmd<1||cmd>13){
	  cout<<"Not valid command ID,enter again : ";
	  cin>>cmd;
	}

 	MSG data;
	data.SetMsgID(cmd);
	data.CopyAt(&cmd,sizeof(cmd),0);

	if(cmd==11)
		{
 			cout<<"Please enter the txt file full name(less than 20 char):";
			string File;
			cin>>File;
 			int L=File.length();
	        char TrajFile[L];
			for(int i=0;i<L;i++)
			{
				TrajFile[i]=File[i];
			}



			data.CopyAt(&L,sizeof(L),sizeof(cmd));

			for(int i=0;i<L;i++)
			{
				data.CopyAt(TrajFile+i,sizeof(char), sizeof(L)+sizeof(cmd)+i);
	 		}
		}

	ControlSysClient.SendData(data);
	return 0;
}


int OnSystemLost(Aris::Core::MSG &msg)
{
	cout<<"Control system lost"<<endl;

	return 0;
}

 
