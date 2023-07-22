#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <strings.h>
#include <unistd.h>

static bool stop = false;
//SIGTERM信号的处理函数，触发时结束主程序中的循环
static void handle_term(int sig)
{
    stop = true;
}

int main(int argc, char* argv[])
{
    signal(SIGTERM, handle_term);   //信号处理函数
    if(argc <= 3)
    {
        printf("usage: %s ip_address port_number backlog\n", basename(argv[0]));
        return 1;
    }
    const char* ip = argv[1];       //ip地址
    int port = atoi(argv[2]);       //端口号
    int backlog = atoi(argv[3]);    //队列大小

    int sock = socket(PF_INET, SOCK_STREAM, 0);     //创建一个socket，ipv4协议，不阻塞，默认协议
    assert(sock >= 0);

    //创建一个IPv4socket地址
    struct sockaddr_in address;
    bzero(&address, sizeof(address));   //将address置为0
    address.sin_family = AF_INET;       //IPv4
    inet_pton(AF_INET, ip, &address.sin_addr);  //字符串ip地址转化为网络字节序
    address.sin_port = htons(port);     //将无符号短整型转换为hostshort，从主机字节顺序到网络字节顺序

    int ret = bind(sock, (struct sockaddr*)&address, sizeof(address));      //将sockaddr绑定scoket
    assert(ret != -1);

    ret = listen(sock, backlog);        //创建socket队列, backlog表示监听队列的能最多装多少任务
    assert(ret != -1);

    //循环等待连接，直到有SIGTERM信号将它中断
    while(!stop)
        sleep(1);
    
    //关闭socket
    close(sock);
    return 0;
}
