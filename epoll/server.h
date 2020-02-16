#include <vector>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <unordered_map>
#include <unordered_set>

#define ROOM_SIZE 100
#define BUFFER_SIZE 1024
#define RECV_BUF_SIZE 400
#define PORT 55888

using namespace std;

int init();
void service(int& sockfd);
void SendMsgToAll(char* msg);
void* service_thread(void* p);