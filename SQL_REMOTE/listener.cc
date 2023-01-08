#include <iostream>
#include "listener.h"
#define PORT 9000
char local_ip4[INET_ADDRSTRLEN] = {0};
class dolinstener
{
public:
    dolinstener()
    {
        bzero(&server, sizeof(server));
        server.sin_family = AF_INET;
        server.sin_port = htons(9000);
        server.sin_addr.s_addr = inet_addr("172.20.168.108");//the local_ip you get and you should copy on it.
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
        {
            std::cout << "sockfd error\n"
                      << std::endl;
            exit(-1);
        }
        log = connect(sockfd, (struct sockaddr *)&server, sizeof(server));
        if (log < 0)
        {
            std::cout << "connect error\n"
                      << std::endl;
            close(sockfd);
            exit(-1);
        }
    }
    void closes()
    {
        close(sockfd);
    }
    void rotateplay()
    {
        fgets(sendbuf, sizeof(sendbuf), stdin);
        send(sockfd, sendbuf, strlen(sendbuf), 0);
        recv(sockfd, recvbuf, sizeof(recvbuf), 0);
        if(strcmp(recvbuf,"quit\n")==0)
        {
            close(sockfd);
            exit(1);
        }
        std::cout <<recvbuf<< std::endl;
        *sendbuf = {0};
        *recvbuf = {0};
    }
    char sendbuf[1024];
    char recvbuf[1024];

private:
    int sockfd, log;
    struct sockaddr_in server;
};
int main()
{

    if( get_local_ip_using_ifaddrs(local_ip4) == RUN_SUCCESS)
    {
        printf("get_local_ip_using_ifaddrs() get local ip : %s \n", local_ip4);
    }else{
        printf("get_local_ip_using_ifaddrs() err \n");
    }
    dolinstener di;
    while (1)
    {
        di.rotateplay();
    }
    di.closes();
    return 0;
}
