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





int launchUDPClient(int packetSize, char* ip)
{
	printf("UDP CLIENT LAUNCHED\n");//did it work?
								  //do some tcp client stuff here

	int retStatus = 0; //Overall status of the program
	int funcStatus = 0;
	int connectionStatus = 1; //Keep loop infinite
	int isNumber = 1;

	char sendBuffer[BUFF_LENGTH] = "";

	struct sockaddr_in RecvAddr;

#ifdef WINDOWS
	WSADATA wsaData;
	SOCKET SendSocket;
#else
	int SendSocket;
#endif

	do
	{

#ifdef WINDOWS
		funcStatus = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (funcStatus < 0)
		{
			printf("WSAStartup failed!\n");
			retStatus = 1;
			break;
		}
#endif

		//Create the socket to send data, UDP socket
		SendSocket = socket(AF_INET, SOCK_DGRAM, 0);
		if (SendSocket < 0)
		{
			printf("Creating socket failed!\n");
#ifdef WINDOWS
			WSACleanup();
#endif
			retStatus = 1;
			break;
		}

		printf("Socket created succesfully!\n");

		//Set up the connection
		memset((char *)&RecvAddr, 0, sizeof(RecvAddr));
		RecvAddr.sin_family = AF_INET;
		RecvAddr.sin_port = htons(defaultPort);
		RecvAddr.sin_addr.s_addr = inet_addr(ip);//IP Address given in the command line
		connectionStatus = 0; //Break out of loop
	} while (connectionStatus);

	if (connectionStatus == 0) //UDP socket succesfully implemented
	{
		int i = 0;
		//int loopCounter = 999999; 
		int loopCounter = 9999;

		for (; i <= loopCounter; i++)
		{
			//Sleep(1);

			sprintf(sendBuffer, "%d", i); 

			funcStatus = sendto(SendSocket, sendBuffer, packetSize, 0, (struct sockaddr*)&RecvAddr, sizeof(RecvAddr));

			if (funcStatus < 0)
			{
				printf("Send failed!\nDisconnecting now!\n");
#ifdef WINDOWS
				closesocket(SendSocket);
				WSACleanup();
#else
				close(SendSocket);
#endif
				retStatus = 1;
				break;
			}

			memset(sendBuffer, 0, packetSize); //Clear buffer
		}

		//funcStatus = sendto(SendSocket, "exit", atoi(packetSize), 0, (struct sockaddr*)&RecvAddr, sizeof(RecvAddr));
	}

#ifdef WINDOWS
	closesocket(SendSocket);
	WSACleanup();
#else
	close(SendSocket);
#endif
	printf("Disconnected!\n");


	return retStatus;
}