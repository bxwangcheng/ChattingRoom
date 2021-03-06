#include "server.h"

unordered_set<int> fd_group;
unordered_map<string, string> id_passwd;

int main() {
    int sockfd = init();
    service(sockfd);
}

int init() {
    int sockfd = socket(PF_INET, SOCK_STREAM, 0); // IPv4, TCP, 默认协议
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
    return sockfd;
}

void service(int& sockfd) {
    cout << "服务器启动" << endl;
    while (1) {
        struct sockaddr_in fromaddr;
        socklen_t len = sizeof(fromaddr);
        int client_fd = accept(sockfd, reinterpret_cast<struct sockaddr*>(&fromaddr), &len); //参数sockfd仍然可以接收新的连接，返回的fd是已经关联至当前连接
        if (client_fd == -1) {
            cout << "客户端连接出错..." << endl;
            continue;
        }
        if (fd_group.find(client_fd) == fd_group.end()) {
            fd_group.insert(client_fd);
            cout << "【socket文件描述符" << client_fd << "已创建】" << endl;
            //有客户端连接之后，启动线程给此客户服务
            pthread_t tid;
            int err = pthread_create(&tid, 0, service_thread, &client_fd);
            // if (err != 0) {
            //     perror("【创建进程失败】");
            // }
            // void* tret;
            // err = pthread_join(tid, &tret);
            // if (err != 0) {
            //     perror("【加入进程失败】");
            // }
            // cout << "线程ID = " << tid << "返回码" << (long)tret << endl;
        }
        if (ROOM_SIZE < fd_group.size()) {
            char str[] = "对不起，聊天室已经满了!";
            send(client_fd, str, strlen(str), 0);
            cout << "【聊天室已满，关闭socket文件描述符" << client_fd << "】" << endl;
            close(client_fd);
        }
    }
}

void SendMsgToAll(char* msg) {
    for (auto it = fd_group.begin(); it != fd_group.end(); it++) {
        cout << "【发送至socket文件描述符" << *it << "】" << endl;
        send(*it, msg, strlen(msg), 0);
    }
}

void* service_thread(void* arg) {
    int client_fd = *reinterpret_cast<int *>(arg);
    cout << "【线程ID = " << pthread_self() << " 已创建】" << endl;
    char recv_buf[4096] = {};
    char send_buf[4096] = {};
    int recv_len = 0;
    while ((recv_len = recv(client_fd, recv_buf, sizeof(recv_buf), 0)) > 0) { // fd有可能一次性接受不完，需要循环接收
        SendMsgToAll(recv_buf);
        memset(recv_buf, 0, sizeof(recv_buf)); // 下一次接收前清空缓冲区
    }
    fd_group.erase(client_fd);
    close(client_fd);
    cout << "【socket文件描述符" << client_fd << "已关闭】" << endl;
    pthread_exit(reinterpret_cast<void*>(0));
}
