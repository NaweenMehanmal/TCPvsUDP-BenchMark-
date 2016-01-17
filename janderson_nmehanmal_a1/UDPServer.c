#include "header.h"

#ifdef WINDOWS

//Windows related libraries

#undef UNICODE
#define WIN32_LEAN_AND_MEAN

#pragma warning(disable:4996)

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib,"ws2_32.lib")

#else

//Linux related libraries

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>  // getaddrinfo() and freeaddrinfo()
#include <unistd.h> // close()

#endif

int launchUDPServer(int packetSize, char* ip)
{
	printf("UDP SERVER LAUNCHED\n");//did it work?
								  //do some tcp client stuff here
	struct sockaddr_in serverAddr, clientAddr;

	int retStatus = 0; //Overall status of the program
	int funcStatus = 0; //Status of the recvFrom function call
	int connectionStatus = 1; //Keep the loop infinite
	int isNumber = 1;
	int clientAddrSize = sizeof(clientAddr);

	char recvBuffer[BUFF_LENGTH] = "";

	//char* testBuffer = (char*)malloc(10000); 


#ifdef WINDOWS
	SOCKET socketDesc;
	WSADATA wsaData;
#else
	int socketDesc;
#endif


	do
	{

#ifdef WINDOWS
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //Used to start and initiliaze the Winsock library
		{
			printf("WSAStartup failed!");
			retStatus = 1;
			break;
		}
#endif
		//Define Server struct
		serverAddr.sin_family = AF_INET; //IPv4 style
		serverAddr.sin_addr.s_addr = INADDR_ANY; //Address
		serverAddr.sin_port = htons(defaultPort); //port #

		socketDesc = socket(AF_INET, SOCK_DGRAM, 0); //Specify UDP socket

		if (socketDesc < 0)
		{
			printf("Creating UDP Socket failed!\n");
#ifdef WINDOWS
			WSACleanup();
#endif
			retStatus = 1;
			break;
		}

		printf("UDP Socket created!\n");

		//Bind the socket
		if (bind((socketDesc), (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
		{
			printf("Binding failed!\n");
#ifdef WINDOWS
			closesocket(socketDesc);
			WSACleanup();
#else
			close(socketDesc);
#endif
			retStatus = 1;
			break;
		}

		printf("Bind successful!\n");
		connectionStatus = 0; //UDP was succesfully implemented

	} while (connectionStatus);

	if (connectionStatus == 0) //If true, connection was made
	{
		int currentRecv  = 0;
		int previousRecv = -1;

		int packetOutOfOrder = 0;
		int packetsLost = 0;
		int packetSuccess = 0;

		char matchBuffer[BUFF_LENGTH] = "";		

		int numArray[BUFF_LENGTH] = {0};
		int index = 0; 

		clock_t start = clock(); //Start timer
		
		while (1)
		{
			/*
			The general idea and basis behind the select function call and the code generated is based off of www.mathcs.emory.edu
			With a small degree of minor changes to the parameters to accomodate for our send/recieve testing
			*/

			//Timeout related variables
			fd_set select_fds;
			struct timeval timeout;

			//Descriptor for the select function call
			FD_ZERO(&select_fds); //Clear out variables
			FD_SET(socketDesc, &select_fds); //Bit corresponding to the socket descriptor
						
			timeout.tv_sec = 3; //3 second timeout 
			timeout.tv_usec = 0; //0 milliseconds

			//Now wait for message until the time expires, returns a non zero in this case 

			if (select(32, &select_fds, NULL, NULL, &timeout) == 0)
			{		
				break;
			}
			else
			{		
				if (recvfrom(socketDesc, recvBuffer, packetSize, 0, (struct sockaddr*)&clientAddr, &clientAddrSize) > 0)
				{
					numArray[index] = atoi(recvBuffer);
					index++;
					//memset(recvBuffer, 0, packetSize);
				}			
			}
		}

		clock_t end = clock(); //End timer

		
		for (int i = 0; i < index; i++)
		{
			printf("%d\n", numArray[i]);

			if (numArray[i] == (previousRecv + 1)) //In order
			{
				packetSuccess++; //Increase 
			}
			else //Out of order 
			{
				packetOutOfOrder++;
				packetsLost += (numArray[i] - (previousRecv + 1));
			}

			previousRecv = numArray[i];

		}

		double totalTime = (double)(end - start) / CLOCKS_PER_SEC; 

		printf("===\nTotal Time: %.2f s\n", totalTime);
		printf("Recieved Packets: %d\n", index);
		printf("Packets in order  (Recieved): %d\n", packetSuccess);
		printf("Packets out of order (Recieved): %d\n", packetOutOfOrder);
		printf("Packets dropped: %d\n===\n", packetsLost);

#ifdef WINDOWS
		closesocket(socketDesc);
		WSACleanup();
#else
		close(socketDesc);
#endif

	}
	return retStatus;
}