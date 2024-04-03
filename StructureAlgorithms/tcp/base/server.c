/*服务器*/
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);//创建套接字
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }                                //创建失败的错误处理
    printf("socket..............\n");//成功则打印“socket。。。。”

    struct sockaddr_in myaddr;                      //创建“我的地址”结构体
    memset(&myaddr, 0, sizeof(myaddr));             //对内存清零（保险起见）
    myaddr.sin_family = AF_INET;                    //选择IPV4地址类型
    myaddr.sin_port = htons(8888);                  //选择端口号
    myaddr.sin_addr.s_addr = inet_addr("127.0.0.1");//选择IP地址

    if (0 > bind(sockfd, (struct sockaddr *)&myaddr, sizeof(myaddr)))//绑定套接字
    {
        perror("bind");
        return -1;
    }
    printf("bind..........\n");

    if (0 > listen(sockfd, 8))//调用listen对指定端口进行监听
    {
        perror("listen");
        return -1;
    }
    printf("listen............\n");

    int connfd = accept(sockfd, NULL, NULL);//使用accept从消息队列中获取请求
    if (connfd < 0) {
        perror("accept");
        return -1;
    }
    printf("accept..............\n");

    char buf[100];//定义一个数组用来存储接收到的数据
    char sendbuf[100] = "test\n";
    int ret;
    while (1) {
        memset(buf, 0, sizeof(buf));
        ret = read(connfd, buf, sizeof(buf));
        if (0 > ret) {//读取失败
            perror("read");
            break;
        }
        else if (0 == ret) {//读取结束
            printf("write close!\n");
            break;
        }
        printf("recv: ");
        fputs(buf, stdout);//打印接收到的数据
        write(connfd, buf, sizeof(buf));
    }
    close(sockfd);//关闭套接字
    close(connfd);//断开连接
    return 0;
}