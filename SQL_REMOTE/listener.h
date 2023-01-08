#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

//使用 ifconf结构体和ioctl函数时需要用到该头文件
#include <net/if.h>
#include <sys/ioctl.h>

//使用ifaddrs结构体时需要用到该头文件
#include <ifaddrs.h>


#define PJ_MAX_HOSTNAME  (128)

#define RUN_SUCCESS 0
#define RUN_FAIL -1

/*可以检测ip4也可以检测ip6，但是需要ifaddrs.h，某些Android系统上没有该头文件（可自己实现该头文件所带内容）
该方法较为强大，可以通过网卡名（ifAddrStruct->ifr_name）获取IP. 
*/
int get_local_ip_using_ifaddrs(char *str_ip)
{
    struct ifaddrs * ifAddrStruct=NULL;
    void * tmpAddrPtr=NULL;
    int status = RUN_FAIL;

    getifaddrs(&ifAddrStruct);

    while (ifAddrStruct!=NULL) 
    {
        if (ifAddrStruct->ifa_addr->sa_family==AF_INET)  // check it is IP4
        { 
            tmpAddrPtr = &((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
            if(inet_ntop(AF_INET, tmpAddrPtr, str_ip, INET_ADDRSTRLEN))
            {
                status = RUN_SUCCESS;
                if(strcmp("127.0.0.1",str_ip))
                {
                     break;
                }
            }  
        }else if(ifAddrStruct->ifa_addr->sa_family==AF_INET6){
            //可以添加IP6相应代码
        }
        ifAddrStruct=ifAddrStruct->ifa_next;
    }
    return status;
}