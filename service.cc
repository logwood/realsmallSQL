#include <iostream>
#include <stdlib.h>
#include <string.h>                     
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>                  
#include <thread>
#include <string.h>
#include <sstream>
#include <mutex>
#include "Table.h"
#define PORT 9000
class dbms_far
{
public:
    bool eventLoop(std::string recvbuf)
    {
        std::string operation;
        std::string tablename;
        std::stringstream sring;
        sring << recvbuf;
        sring >> operation;

        if (operation == "insert")
        {
            sring << recvbuf;
            sring >> tablename; //分离出tablename，再将字符串传给createtable
            insert(tablename, sring.str(), tab_path);
        }
        else if (operation == "create")
        {
            sring << recvbuf;
            sring >> tablename; //分离出tablename，再将字符串传给createtable
            create_table(tablename, sring.str());
        }
        else
        {
        }

    }; 
    //输入样例create table person (id int primary,name string)
    //默认逗号间无空格，空格只可能出现在一个列名每个属性（id/int/primary都叫属性）之间
    void create_table(std::string tablename, std::string value)
    {
        int cnt = strlen("create table") + tablename.size() + 3; //跳到“（”位置后面一格，即开始读入
        std::vector<Typee> table_info;                           // create_table用，(id int primary, name string)
        std::string tmp;                                         //存访typee中的一个,比如id

        while (value[cnt] != ')')
        {
            int count = 0; //代表第i个属性
            Typee ttype;   //临时储存一个列名的三个属性

            int i = 0;
            while (value[cnt] == '"')
                cnt++; //如果是字符串，去掉原本字符串起始的“符号
            while (value[cnt] != ',' && value[cnt] != '"' && value[cnt] != ')' && value[cnt] != ' ')
            { //判断结束位置
                tmp[i++] = value[cnt++];
            }
            if (value[cnt] == '"')
                cnt++;
            std::cout<<value[cnt-2]<<value[cnt-1]<<value[cnt];
            if (value[cnt] == ' ')
            {
                if (count == 0)
                    ttype.id = tmp;
                else
                    ttype.type = tmp; //只可能count == 1
                count++;
            }

            if (value[cnt] == ',')
            {
                if (count == 1)
                    table_info.push_back(ttype);
                if (count == 2)
                {
                    if (tmp[0] == '1')
                    {
                        ttype.Restrict = Primary_KY;
                    }
                    table_info.push_back(ttype);
                }
                cnt++;
                tmp.clear();
            }
        }
    std::cout<<table_info[0].id<<table_info[0].Restrict;

    } //输入样例 insert person values(1001,“peter”),默认逗号间无空格
    void insert(std::string tablename, std::string value, std::string tab_path)
    {
        int cnt = tablename.size() + strlen("insert") + strlen("values") + 4; //跳到“（”位置后面一格，即开始读入
        std::vector<std::string> value_info;

        while (value[cnt] != ')')
        {
            std::string tmp; //存访读入信息中的一个,比如1001
            int i = 0;
            while (value[cnt] == '"')
                cnt++; //去掉原本字符串起始的“符号
            while (value[cnt] != ',' && value[cnt] != '"' && value[cnt] != ')')
            { //判断结束位置
                tmp[i++] = value[cnt++];
            }
            value_info.push_back(tmp);
            if (value[cnt] == '"')
                cnt++;
            if (value[cnt] == ',')
                cnt++;
        }

        std::vector<Typee> type = {{"int", "", normal}};
        Table gettable(tab_path,tablename, type,0);
        gettable.InsertToTail(value_info);
    }
private:
    std::string tab_path;
};
static void* client_thread(void *arg)
{
    int recvlen = 0;
    char recvbuf[1024] = "";
    long connfd = (long)arg;
    while((recvlen = recv(connfd,recvbuf,sizeof(recvbuf),0))>0)
    {
        std::cout<<"recv_buf:"<<recvbuf<<recvlen<<std::endl;
        std::mutex mut;
        mut.lock();
        send(connfd, recvbuf, recvlen, 0);
        dbms_far dms;
        if(dms.eventLoop(recvbuf))     
        {

        }   
        mut.unlock();
    }
    std::cout<<"client closed"<<recvbuf<<std::endl;
    close(connfd);
    return NULL;
}

int do_do()
{
    int sockfd;
    int log;
    int connfd;
    pthread_t thread_id;
    struct sockaddr_in server,client;
    bzero(&server,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);//在本地的
    //创建套接字
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd < 0)
     std::cout<<"socket create error\n"<<std::endl;
    log = bind(sockfd,(struct sockaddr*)&server,sizeof(server));
    if(log < 0)
    {
        std::cout<<"bindfd error\n"<<std::endl;
        close(sockfd);
        exit(-1);
    }

    log = listen(sockfd,20);
    if(log < 0)
    {
        std::cout<<"listen error\n"<<std::endl;
        printf("listen error\n");
        close(sockfd);
        exit(-1);
    }
    std::cout<<"waiting client >>>>>"<<std::endl;

    while(1)
    {
        socklen_t client_len = sizeof(client);
        //accept
    connfd = accept(sockfd,(struct sockaddr*)&client,&client_len);
    if(connfd < 0)
    {
        std::cout<<"connfd error"<<std::endl;
        continue;
    }
    char cli_ip[INET_ADDRSTRLEN] = "";
    inet_ntop(AF_INET, &client.sin_addr, cli_ip, INET_ADDRSTRLEN);
    std::cout<<"----------------------------------------------"<<std::endl;
    printf("client ip=%s,port=%d\n", cli_ip,ntohs(client.sin_port));

    pthread_create(&thread_id,NULL,&client_thread,(void*)connfd);
    pthread_detach(thread_id);//线程分离，结束时自动回收线程
    }
    close(sockfd);
    return 0;
}
int main()
{
    do_do();
}