
#include "header.h"
#ifdef WINDOWS

#define WIN32_LEAN_AND_MEAN

#pragma warning(disable:4996)

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#else

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
#include <unistd.h> /* Needed for close() */

#endif //Cross platform socket handling

int launchTCPClient(int packetSize, char* ip)
{
	printf("TCP CLIENT LAUNCHED\n");//did it work?

	int retClientStatus = 0;
	int connectionSuccess = 1; //Keep the loop infinite
	int sendResult = 0;
	//int isNumber = 1;

	char testSend[BUFF_LENGTH] = "";
	struct sockaddr_in server;

#ifdef WINDOWS
	SOCKET ConnectSocket = INVALID_SOCKET;
	WSADATA wsaData;
#else
	int ConnectSocket = 0;
#endif

	do
	{
	#ifdef WINDOWS
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //Used to start and initiliaze the Winsock library
		{
			printf("WSAStartup failed!");
			retClientStatus = 1;
			break;
		}
	#endif
		ConnectSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (ConnectSocket < 0)
		{
			printf("Creating socket failed!\n");
			retClientStatus = 1;
			break;
		}

		printf("Socket was created!\n");

		server.sin_addr.s_addr = inet_addr(ip); //The ip address specific in the command line
		server.sin_family = AF_INET; //IPv4 style
		server.sin_port = htons(defaultPort); //port #

												//Connect to a remote server
		int ret = connect(ConnectSocket, (struct sockaddr*)&server, sizeof(server));

		if (ret < 0)
		{
			printf("Connection failed!\n");
			retClientStatus = 1;
			break;
		}

		printf("Connected!\n");
		connectionSuccess = 0;

	} while (connectionSuccess);

	//Now send data to the server
	if (connectionSuccess == 0)
	{
		int i = 0;
		//int packetSize = atoi(argv[2]);

		for (i = 0; i <= 999999; i++) //Dependent test case, 1000 packets sent
		{
			//Sleep(5); //Short pause

			sprintf(testSend, "%004d", i);

			sendResult = send(ConnectSocket, testSend, packetSize, 0);

			memset(testSend, 0, BUFF_LENGTH);

			if (sendResult < 0)
			{
				printf("Send failed, disconnecting from server!\n");
				retClientStatus = 1;
				break;
			}
		}

		//Close the sockets gracefully within its specific OS environment
	#ifdef WINDOWS

		closesocket(ConnectSocket);
		WSACleanup();
	#else
		close(ConnectSocket); //Close socket gracefully
	#endif
	}


	printf("Officially disconnected!\n");

	return retClientStatus; //Overall Status return

	//do some tcp client stuff here
}
