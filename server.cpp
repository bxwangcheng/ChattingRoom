#include "server.h"

int sockfd;         //服务器socket的文件描述符
int cur_people = 0;
vector<int> fd_group(ROOM_SIZE, 0); //客户端的socketfd, 100个元素，fds[0]~fds[99]
typedef struct sockaddr SA;

int main() {
    init();
    service();
}

void init() {
    sockfd = socket(PF_INET, SOCK_STREAM, 0); // IPv4, TCP, 默认协议
    if (sockfd == -1) {
        perror("创建socket失败");
        exit(-1);
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = PORT;
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sockfd, (SA *)&addr, sizeof(addr)) == -1) {
        perror("绑定失败");
        exit(-1);
    }
    if (listen(sockfd, 100) == -1) {
        perror("设置监听失败");
        exit(-1);
    }
}

void service() {
    cout << "服务器启动" << endl;
    while (1) {
        struct sockaddr_in fromaddr;
        socklen_t len = sizeof(fromaddr);
        int fd = accept(sockfd, (SA *)&fromaddr, &len);
        if (fd == -1) {
            cout << "客户端连接出错..." << endl;
            continue;
        }
        for (int i = 0; i < ROOM_SIZE; i++) {
            if (fd_group[i] == 0) {
                //记录客户端的socket
                fd_group[i] = fd;
                cur_people++;
                cout << "fd = " << fd << endl;
                //有客户端连接之后，启动线程给此客户服务
                pthread_t tid;
                pthread_create(&tid, 0, service_thread, &fd);
                break;
            }
        }
        if (ROOM_SIZE < cur_people) {
            char str[] = "对不起，聊天室已经满了!";
            cout << "full, reject" << endl;
            send(fd, str, strlen(str), 0);
            close(fd);
        }
    }
}

void SendMsgToAll(char* msg) {
    for (int i = 0; i < ROOM_SIZE; i++) {
        if (fd_group[i] != 0) {
            cout << "sendto " << fd_group[i] << endl;
            send(fd_group[i], msg, strlen(msg), 0);
        }
    }
}

void* service_thread(void* p) {
    int fd = *(int*)p;
    cout << "pthread = " << fd << endl;
    while (1) {
        char buf[100] = {};
        if (recv(fd, buf, sizeof(buf), 0) <= 0) {
            int i;
            for (i = 0; i < ROOM_SIZE; i++) {
                if (fd == fd_group[i]) {
                    fd_group[i] = 0;
                    cur_people--;
                    break;
                }
            }
            cout << "退出：fd = " << fd <<" quit" << endl;
            pthread_exit((void*)i);
        }
        // if (strcmp(buf, "rz") == 0)
        // {
        //     int data_len, flag = 0;
        //     char buffer[BUFFER_SIZE];
        //     FILE *stream;
        //     if ((stream = fopen("data", "w")) == NULL)
        //     {
        //         printf("file open error\n");
        //         exit(1);
        //     }
        //     if (data_len = recv(fd, buffer, BUFFER_SIZE, 0))
        //     {
        //         flag++;
        //         if (flag == 1)
        //         {
        //             printf("正在接收文件....\n");
        //         }
        //         if (data_len < 0)
        //         {
        //             printf("接收错误\n");
        //             exit(1);
        //         }
        //         //向文件中写入数据
        //         int write_len = fwrite(buffer, sizeof(char), data_len, stream);
        //         if (write_len > data_len)
        //         {
        //             printf("file write failed\n");
        //             exit(1);
        //         }
        //         bzero(buffer, BUFFER_SIZE);
        //     }
        //     if (flag > 0)
        //     {
        //         printf("文件传送完毕\n");
        //         char *str = "有一份新文件已上传，是否下载？(yes/no)";
        //         SendMsgToAll(str);
        //     }
        //     if (flag == 0)
        //         printf("文件传输失败\n");
        //     fclose(stream);
        //     continue;
        // }
        // if (strcmp(buf, "yes") == 0)
        // {
        //     FILE *stream;
        //     if ((stream = fopen("data", "r")) == NULL)
        //     {
        //         printf("The file was not opened!\n");
        //         exit(1);
        //     }
        //     int len = 0;
        //     char file[1024] = {};
        //     if ((len = fread(file, 1, 1024, stream)) > 0)
        //     {
        //         char F[3] = "yes";
        //         send(fd, F, 3, 0);
        //         sleep(1);
        //         send(fd, file, len, 0);
        //     }
        //     continue;
        // }
        // if (strcmp(buf, "no") == 0)
        // {
        //     printf("1111");
        //     continue;
        // }
        //把服务器接受到的信息发给所有的客户端
        SendMsgToAll(buf);
    }
}
