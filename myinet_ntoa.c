#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
//测试机器的大小端字节序
void byteorder()
{
    union
    {
        short value;
        char union_bytes[sizeof(short)];
    }test;
    test.value = 0x0102;
    if((test.union_bytes[0] == 1) && (test.union_bytes[1] == 2))
        printf("big endian\n") ;
    else if((test.union_bytes[0] == 2) && (test.union_bytes[1] == 1))
        printf ("little endian\n") ;
    else
        printf ("unknow\n") ;
}

void testarpa(void)
{
    //inet_ntoa需要struct in_addr类型的变量，所以需要将const char*转换为in_addr_t类型
    struct in_addr ad1;
    struct in_addr ad2;
    in_addr_t l1 = inet_addr("1.2.3.4");
    in_addr_t l2 = inet_addr("10.23.4.31");
    memcpy(&ad1, &l1, 4);
    memcpy(&ad2, &l2, 4);
    char* szValue1 = inet_ntoa(ad1);
    char* szValue2 = inet_ntoa(ad2);
    printf ("%s\n",szValue1);
    printf ("%s\n",szValue2) ;
}

int main(void)
{
    byteorder();
    testarpa();

    return 0;
}
