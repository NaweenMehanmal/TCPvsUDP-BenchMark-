#ifndef _HEADER_H_
#define _HEADER_H_

#define LINUX //Specify which environment you are working on (LINUX) or (WINDOWS)
#define BUFF_LENGTH 100000

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//constants
static const unsigned short defaultPort = 9000; //Default port #

//function prototypes
int launchTCPClient(int packetSize, char* ip);
int launchTCPServer(int packetSize, char* ip);
int launchUDPClient(int packetSize, char* ip);
int launchUDPServer(int packetSize, char* ip);

#endif
