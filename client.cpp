#include "client.h"

int sockfd; //客户端socket
typedef struct sockaddr SA;
char name[30];

int main() {
    InitialClient();
    cout << "请输入您的名字：" << endl;
    scanf("%s", name);
    StartService();
    return 0;
}

void InitialClient() {
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = PORT;
    addr.sin_addr.s_addr = INADDR_ANY;
    if (connect(sockfd, (SA *)&addr, sizeof(addr)) == -1) {
        perror("无法连接到服务器");
        exit(-1);
    }
    printf("客户端启动成功\n");
}

void StartService() {
    pthread_t tid;
    // void *recv_thread(void *);
    pthread_create(&tid, 0, recv_thread, 0);
    char buf2[100] = {};
    sprintf(buf2, "%s进入了聊天室", name);
    send(sockfd, buf2, strlen(buf2), 0);
    while (1) {
        char buf[100] = {};
        scanf("%s", buf);
        char msg[131] = {};
        getTimeStr(msg);
        sprintf(msg, "%s%s:\n%s", msg, name, buf);
        if (strcmp(buf, "/QUIT") == 0)
        {
            memset(buf2, 0, sizeof(buf2));
            sprintf(buf2, "%s退出了聊天室", name);
            send(sockfd, buf2, strlen(buf2), 0);
            break;
        }
        send(sockfd, msg, strlen(msg), 0);
    }
    close(sockfd);
}

void* recv_thread(void *p) {
    while (1) {
        char buf[1024] = {}; // ********
        if ( recv(sockfd, buf, sizeof(buf), 0) <= 0) {
            return nullptr;
        }   
        cout << buf << endl;
    }
}

void getTimeStr(char* p) {
    struct tm *t;
    time_t tt;
    time(&tt);
    t = localtime(&tt);
    sprintf(p, "[%02d:%02d:%02d]", t->tm_hour, t->tm_min, t->tm_sec);
}