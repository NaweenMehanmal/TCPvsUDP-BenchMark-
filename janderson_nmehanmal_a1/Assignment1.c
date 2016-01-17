
#include "header.h"





#pragma warning(disable:4996)





int main(int argc, char* argv[])
{
	int returnValue = 0;
	char type = ' '; //client or server?
	char method = ' '; //udp or tcp?
	int packetSize = 0; //the size of packets we want to send/receive
	char ip[20]; //the ip of the server we want to connect to

	if (argc == 4 || argc == 5) //Proper number of command line arguments (4 plus file name)
	{
		if (argc == 4)
		{
			strcpy(ip, "0.0.0.0");
		}
		if ((strcmp(argv[1], "-C") == 0) || (strcmp(argv[1], "-S") == 0)) //Proper flag use for server/client
		{
			type = argv[1][1]; //store 'C' or 'S' into type

			if ((strcmp(argv[2], "-T") == 0) || (strcmp(argv[2], "-U") == 0)) //Proper flag use for TCP/UDP
			{
				method = argv[2][1]; //store 'T' or 'U' into method
				packetSize = atoi(argv[3]); //store entered packetsize
				if (argc == 5)
				{
					strcpy(ip, argv[4]); //store entered ip
				}
				//Now, we need to call the appropriate function and pass in our parsed information
				if ((type == 'C') && (method == 'T'))
				{
					returnValue = launchTCPClient(packetSize, ip); //Launch TCP Client
				}
				else if ((type == 'C') && (method == 'U'))
				{
					returnValue = launchUDPClient(packetSize, ip); //Launch UDP Client
				}
				else if ((type == 'S') && (method == 'T'))
				{
					returnValue = launchTCPServer(packetSize, ip); //Launch TCP Server
				}
				else if ((type == 'S') && (method == 'U'))
				{
					returnValue = launchUDPServer(packetSize, ip); //Launch UDP Server
				}
				else //I don't know how we got here but something didn't work.
				{
					printf("Something went wrong!");
					returnValue = 1;
				}
			}
			else //Improper command line usage
			{
				printf("Error: Proper command line usage: A...\n");
				returnValue = 1;
			}
		}
		else //Improper command line usage
		{
			printf("Error: Proper command line usage: B...\n");
			returnValue = 1;
		}
	}
	else //Improper command line usage
	{
		printf("Error: Proper command line usage: C...");
		returnValue = 1;
	}
	return returnValue;
}