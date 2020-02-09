#include <vector>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define ROOM_SIZE 100
#define BUFFER_SIZE 1024
#define PORT 55888

using namespace std;

void init();
void service();
void SendMsgToAll(char* msg);
void* service_thread(void* p);