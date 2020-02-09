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
    // FILE *stream;
    pthread_t id;
    // void *recv_thread(void *);
    pthread_create(&id, 0, recv_thread, 0);
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
        // if (strcmp(buf, "rz") == 0)
        // {
        //     send(sockfd, buf, strlen(buf), 0);
        //     printf("请输入文件完整路径：\n");
        //     char file[1024] = {};
        //     scanf("%s", file);
        //     if ((stream = fopen(file, "r")) == NULL)
        //     {
        //         printf("The file was not opened!\n");
        //         exit(1);
        //     }
        //     printf("正在上传文件...\n");
        //     int len = 0;
        //     if ((len = fread(file, 1, 1024, stream)) > 0)
        //     {
        //         if (send(sockfd, file, len, 0) < 0)
        //         {
        //             printf("Send file error\n");
        //         }
        //         bzero(file, 1024);
        //     }
        //     if (fclose(stream))
        //     {
        //         printf("File close error\n");
        //         exit(1);
        //     }
        //     continue;
        // }
        // if (strcmp(buf, "yes") == 0)
        // {
        //     send(sockfd, buf, strlen(buf), 0);
        //     continue;
        // }
        // if (strcmp(buf, "no") == 0)
        // {
        //     send(sockfd, buf, strlen(buf), 0);
        //     continue;
        // }
        send(sockfd, msg, strlen(msg), 0);
    }
    close(sockfd);
}

void* recv_thread(void *p) {
    while (1) {
        char buf[1024] = {};
        if (recv(sockfd, buf, sizeof(buf), 0) <= 0) {
            return nullptr;
        }
        // if (buf[0] == 'y') {
        //     int data_len, flag = 0;
        //     char buffer[BUFFER_SIZE];
        //     FILE *stream;
        //     if ((stream = fopen("data1", "w")) == NULL)
        //     {
        //         printf("file open error\n");
        //         exit(1);
        //     }
        //     if (data_len = recv(sockfd, buffer, BUFFER_SIZE, 0))
        //     {
        //         flag++;
        //         if (flag == 1)
        //         {
        //             printf("正在下载文件....\n");
        //         }
        //         if (data_len < 0)
        //         {
        //             printf("下载错误\n");
        //             exit(1);
        //         }
        //         //向文件中写入数据
        //         int write_len = fwrite(buffer, sizeof(char), data_len, stream);
        //         if (write_len > data_len)
        //         {
        //             cout << "file write failed" << endl;
        //             exit(1);
        //         }
        //         bzero(buffer, BUFFER_SIZE);
        //     }
        //     if (flag > 0)
        //         cout << "文件下载完毕" << endl;
        //     if (flag == 0)
        //         cout << "文件下载失败" << endl;
        //     fclose(stream);
        //     continue;
        // }
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