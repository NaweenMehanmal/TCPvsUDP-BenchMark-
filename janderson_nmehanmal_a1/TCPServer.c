#include "header.h"
#ifdef WINDOWS

#undef UNICODE
#define WIN32_LEAN_AND_MEAN

#define MSG_WAITALL 0x8

#pragma warning(disable:4996)

//Winsock libraries
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib,"ws2_32.lib")

#else

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>  // getaddrinfo() and freeaddrinfo()
#include <unistd.h> // close()

#endif

int launchTCPServer(int packetSize, char* ip)
{
	printf("TCP SERVER LAUNCHED\n");//did it work?

	int retStatus = 0; //Overall status of the program
	int connectionSuccess = 1; //Keep the loop infinite
	int resultOfConnection = 0; //Variable checking for status of function return values

	struct sockaddr_in server, client;

	//Read data variables

	int recvResult = 0;
	int sendResult = 0;
	int clientDisconnected = 1;

	char recvBuffer[BUFF_LENGTH] = "";

#ifdef WINDOWS
	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;
	WSADATA wsa;

#else
	int ListenSocket = 0;
	int ClientSocket = 0;

#endif 



		do
		{

#ifdef WINDOWS
			if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) //Used to start and initiliaze the Winsock library
			{
				printf("WSAStartup failed!");
				retStatus = 1;
				break;
			}
#endif			
			//Create the listen socket
			ListenSocket = socket(AF_INET, SOCK_STREAM, 0); //Socket for listening

			if (ListenSocket < 0)
			{
				printf("Creating TCP Socket failed!\n");
#ifdef WINDOWS
				WSACleanup();
#endif
				retStatus = 1;
				break;
			}

			printf("TCP Socket created!\n");

			server.sin_family = AF_INET; //IPv4 style
			server.sin_addr.s_addr = INADDR_ANY; //Address
			server.sin_port = htons(defaultPort); //port #

												  //Bind socket
			if (bind(ListenSocket, (struct sockaddr *)&server, sizeof(server)) < 0)
			{
#ifdef WINDOWS
				closesocket(ListenSocket);
				WSACleanup();
#else
				close(ListenSocket); //Close socket stream
#endif
				printf("Bind failed!\n");
				retStatus = 1;
				break;
			}

			printf("Bind successful!\nListening in on connection..\n");

			//Listen in on Client Connection
			listen(ListenSocket, 3);

			int c = sizeof(struct sockaddr_in);

			ClientSocket = accept(ListenSocket, (struct sockaddr *)&client, (socklen_t*)&c);
			if (ClientSocket < 0)
			{
#ifdef WINDOWS
				closesocket(ListenSocket);
				WSACleanup();
#else
				close(ListenSocket); //Close socket stream
#endif
				printf("Client connection failed!\n");
				retStatus = 0;
				break;
			}

			printf("Connection made!\n");
			connectionSuccess = 0; //Break out of loop 

		} while (connectionSuccess);

		//Now read data
		if (connectionSuccess == 0)
		{
			int counter = 0;
			int packetRecv = 0;
			int packetLost = 0;
			int previousRecv = -1;
			int currentRecv = 0; 

			char matchBuffer[BUFF_LENGTH] = "";

			while (1)
			{
				recvResult = recv(ClientSocket, recvBuffer, packetSize, MSG_WAITALL);

				if (recvResult > 0)
				{
					//sprintf(matchBuffer, "%d", counter);
					//printf("%s\n", recvBuffer); //Print out result

					currentRecv = atoi(recvBuffer);

					if (currentRecv == (previousRecv + 1))
					{
						packetRecv++;
						counter++;
					}
					else
					{						
						packetLost++;
						counter++;
					}

					if (counter == 99999) //All of the packets sent 
					{
						counter++;
						break;
					}

					previousRecv = currentRecv;

				}
				else if (recvResult == 0)
				{
					printf("Connection closing...\n");
#ifdef WINDOWS
					//Close the entire connection
					recvResult = shutdown(ClientSocket, SD_SEND);

					if (recvResult == SOCKET_ERROR)
					{
						printf("Socket shutdown failed!\n");
						retStatus = 1;
						break;
					}
#endif
					break;
				}
				else
				{
					printf("recv failed, client disconnected!\n");
					retStatus = 1;
					break;
				}
			}

		/*	if ((packetSuccess + packetOutOfOrder + packetsLost) != 1000000)
			{
				int addRemaining = 1000000 - (packetSuccess + packetOutOfOrder + packetsLost);
				packetsLost += addRemaining;
			}*/

			printf("=======\nRecieved: %d\nPacket Lost: %d\n", packetRecv, packetLost);

#ifdef WINDOWS
			closesocket(ClientSocket);
			WSACleanup();
#else
			close(ClientSocket);
#endif
		}	
	return retStatus;
}