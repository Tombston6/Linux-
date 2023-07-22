#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <assert.h>
#include <unistd.h>
#include <libgen.h>
#include <string.h>
#include <errno.h>

#define BUF_SIZE 1024

int main(int argc, char** argv)
{
    if(argc <= 2)
    {
        printf("usage: %s ip_address port_number\n", basename(argv[0]));
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip,&address.sin_addr);
    address.sin_port = htons(port);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);
    
    int ret = bind(sock, (struct sockaddr*)&address, sizeof(address));      //服务器端绑定ip地址
    assert(ret != -1);

    ret = listen(sock, 5);      //创建sock监听队列
    assert(ret != -1);

    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);
    int connfd = accept(sock, (struct sockaddr*)&client, &client_addrlength);   //被动连接，从监听队列中拿取任务
    if(connfd < 0)
        printf("errno is: %d\n", errno);
    else
    {
        char buffer[BUF_SIZE];
        
        memset(buffer, '\0', BUF_SIZE);
        ret = recv(connfd, buffer, BUF_SIZE-1, 0);
        printf("got %d bytes of normal data '%s'\n", ret, buffer);
        
        memset(buffer, '\0', BUF_SIZE);
        ret = recv(connfd, buffer, BUF_SIZE-1, MSG_OOB);            //紧急信息
        printf("got %d bytes of oob data '%s'\n", ret, buffer);

        memset(buffer, '\0', BUF_SIZE);
        ret = recv(connfd, buffer, BUF_SIZE-1, 0);
        printf("got %d bytes of normal data '%s'\n", ret, buffer);

        close(connfd);                                      //关闭连接
    }

    close(sock);                                            //关闭socket
    return 0;
}
