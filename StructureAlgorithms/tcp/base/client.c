/*客户端*/
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
    int sockfd;
    int ret = 0;
    if (0 > (sockfd = socket(AF_INET, SOCK_STREAM, 0))) {
        perror("socket");
        return -1;
    }
    printf("socket...........\n");

    struct sockaddr_in srv_addr;
    memset(&srv_addr, 0, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(8888);
    srv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    ret = connect(sockfd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
    if (ret < 0) {
        perror("connect");
        return -1;//exit //pthread_exit
    }
    printf("connect..............\n");
    
    char buf[100];
    char recvbuf[100];
    while (1) {
        printf("send: ");
        fgets(buf, sizeof(buf), stdin);
        ret = write(sockfd, buf, sizeof(buf));
        if (ret < 0) {
            perror("write");
            break;
        }
        if (strncmp(buf, "quit", 4) == 0)
            break;

        ret = read (sockfd, recvbuf, sizeof(recvbuf));
        if (ret < 0) {
            perror("read");
            break;
        }
        printf("receive: ");
        fputs(recvbuf, stdout);//打印接收到的数据
    }
    close(sockfd);
    return 0;
}