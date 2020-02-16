#include "client.h"

int main() {
    int sockfd = InitialClient();
    StartService(sockfd);
    return 0;
}

int InitialClient() {
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = PORT;
    addr.sin_addr.s_addr = INADDR_ANY;
    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("无法连接到服务器");
        exit(-1);
    }
    printf("客户端启动成功\n");
    return sockfd;
}

void StartService(int sockfd) {
    char notic_buf[100];
    char input_buf[BUFFER_SIZE - 32];
    char send_buf[BUFFER_SIZE];
    char recv_buf[BUFFER_SIZE];
    char name[30];
    pthread_t tid;
    pthread_create(&tid, 0, recv_thread, (void*)&sockfd);
    cout << "请输入用户名：" << endl;
    cin >> name;
    sprintf(notic_buf, "%s进入了聊天室", name);
    send(sockfd, notic_buf, strlen(notic_buf), 0);
    while (1) {
        cin.getline(input_buf, sizeof(input_buf));
        if (strcmp(input_buf, "/QUIT") == 0) {
            sprintf(notic_buf, "%s退出了聊天室", name);
            send(sockfd, notic_buf, strlen(notic_buf), 0);
            break;
        }
        while (strlen(input_buf)) {
            getTimeStr(send_buf);
            sprintf(send_buf, "%s%s:\n%s", send_buf, name, input_buf);
            send(sockfd, send_buf, strlen(send_buf), 0);
            memset(input_buf, 0, sizeof(input_buf));
            memset(send_buf, 0, sizeof(send_buf));
        }
    }
    close(sockfd);
}

void* recv_thread(void *p) {
    int sockfd = *(int *)p;
    char recv_buf[BUFFER_SIZE] = {};
    while (recv(sockfd, recv_buf, sizeof(recv_buf), 0) > 0) {
        cout << recv_buf << endl;
        memset(recv_buf, 0, sizeof(recv_buf));
    }
    return nullptr;
}

void getTimeStr(char* p) {
    struct tm *t;
    time_t tt;
    time(&tt);
    t = localtime(&tt);
    sprintf(p, "[%02d:%02d:%02d]", t->tm_hour, t->tm_min, t->tm_sec);
}