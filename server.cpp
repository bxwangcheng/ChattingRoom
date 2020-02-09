#include "server.h"

int sockfd;         //服务器socket的文件描述符
int cur_people = 0;
vector<int> fd_group(ROOM_SIZE, 0); //客户端的socketfd, 100个元素，fds[0]~fds[99]

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
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("绑定失败");
        exit(-1);
    }
    if (listen(sockfd, ROOM_SIZE) == -1) {
        perror("设置监听失败");
        exit(-1);
    }
}

void service() {
    cout << "服务器启动" << endl;
    while (1) {
        struct sockaddr_in fromaddr;
        socklen_t len = sizeof(fromaddr);
        int fd = accept(sockfd, reinterpret_cast<struct sockaddr*>(&fromaddr), &len);
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
    char buf[RECV_BUF_SIZE] = {};
    int fd = *reinterpret_cast<int *>(p);
    cout << "pthread = " << fd << " created." << endl;
    int len;
    while ((len = recv(fd, buf, sizeof(buf), 0)) > 0) {
        SendMsgToAll(buf);
    }
    if (len < 0) {
        int i;
        for (i = 0; i < ROOM_SIZE; i++) {
            if (fd == fd_group[i]) {
                fd_group[i] = 0;
                cur_people--;
                break;
            }
        }
        cout << "退出：fd = " << fd <<" quit" << endl;
        pthread_exit(reinterpret_cast<void*>(i));
    }
}
