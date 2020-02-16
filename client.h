#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

#define BUFFER_SIZE 1024
#define PORT 55888

using namespace std;

int InitialClient();
void StartService(int sockfd);
void getTimeStr(char* p);
void* recv_thread(void* p);