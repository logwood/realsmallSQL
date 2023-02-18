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
#include <map>
#include "delete_file.h"
#include <sys/stat.h>
#include "CreateTable.h"
#define PORT 12346
int nums=0;
class dbms_far
{
public:
    bool eventLoop(std::string& recvbuf)
    {
        //create table Persons ( Id_P int primary, LastName string , FirstName string , Address string , City string ) 
        //select Id_P from Persons where Id_P = 25
        //insert into Persons ( Id_P , LastName ) values ( 25 , jack ) , ( 28 , hek ) , ( 24 , hello )
        //select books from stud where books = 25
        //delete from Persons where Id_P = 25
        //std::string recvbuf = "drop table Person";
        std::string operation;//操作名称
        std::string tablename="stud";
        std::stringstream sring(recvbuf);//输入流
        sring >> operation;
        if (operation == "insert")
        {
            std::vector<std::vector<std::string>> places;
            sring >> operation;
            if(operation=="into")
            {
                sring >> tablename;
            }
            else
            {
                tablename=operation;
            }
                std::string s;
                int i=-1,j=0;
                while(getline(sring,s,','))
                {
                    std::stringstream sri(s);//输入流
                    while(getline(sri,s,' '))
                    {
                        if(!s.compare("("))
                        {
                            i++;
                            places.push_back({""});
                            places[i].pop_back();
                        }
                        else if(s.size()>=7&&!s.substr(0,6).compare("values"))
                        {
                            places.push_back({""}),i++,places[i].pop_back();
                            auto stt=s.substr(6);
                            sri.str("");
                            sri.clear();
                            sri.str(stt);
                        }
                        else if(s.size()>=1&&s[0]=='(')
                        {
                            i++;
                            places.push_back({""});
                            places[i].pop_back();
                            auto stt=s.substr(1);
                            sri.str("");
                            sri.clear();
                            sri.str(stt);
                        }
                        else if(!s.compare("values")){places.push_back({""}),i++,places[i].pop_back();}
                        else if(!s.compare("")||!s.compare(")")||!s.compare("values")){}
                        else 
                        {
                            if(s.back()==')')
                            {
                                s.pop_back();
                            }
                            places[i].emplace_back(s);
                        }
                    }
                }
            std::vector<Typee> typ = {{"int", "", normal}};
            Table gettable(Oral_path, tablename+".table", typ, BPLUS);
            gettable.InsertToPlace(places);
            gettable.showTable();
        }
        else if (operation == "create")
        {
            sring << recvbuf;
            sring >> operation;
            if(!operation.compare("table"))
            {
                sring >> tablename; //分离出tablename，再将字符串传给createtable
                std::vector<Typee> cirr;
                cirr=create_table(recvbuf,cirr);
                Table gettable(Oral_path,tablename+".table",cirr,BPLUS);
                gettable.showTable();
                std::vector<std::string> typee;
                for(auto cir:cirr)
                {
                    typee.push_back(cir.id);
                }
                Index *index = new Index(tablename,3,typee);
                index->showBtreeByLeftAndWrite();
                gettable.WriteInBinary();
            }
            else
            {
                sring>>tablename;
                mkdir(tablename.c_str(),0777);
            }
        }
        else if(operation == "select")
        {
            const std::map<std::string,int> LocalMap={{">",1},{"<",-1},{"=",0}};
            std::string column; //记录被选择的column名称，为*代表全选
            std::string word; //保存流读取的当前字符串
            std::vector<std::string> cond;//存储条件操作所用字符串，cond(0)为匹配列名，cond(1)为匹配符号，cond(2)匹配常数值

            int i = 1; //记录当前为第几个字符串
            while(sring >> word){
                if(i == 1)column = word;
                else if(i == 3)tablename = word;
                else if(i >= 5 && i <= 7)cond.push_back(word);
                ++ i ;
            }
            int need=LocalMap.at(cond[1]);
            std::vector<Typee> typ = {{"int", "", normal}};
            Table gettable(Oral_path, tablename+".table", typ, AVL);
            gettable.ReadInBinary();
            if(gettable.search(cond[2],need,column,cond[0]).size()!=0)
            gettable.showTable();
        }        
        else if(operation == "delete")
        {
            const std::map<std::string,int> LocalMap={{">",1},{"<",-1},{"=",0}};
            std::string column; //记录被选择的column名称，为*代表全选
            std::string word; //保存流读取的当前字符串
            std::vector<std::string> cond;//存储条件操作所用字符串，cond(0)为匹配列名，cond(1)为匹配符号，cond(2)匹配常数值

            int i = 1; //记录当前为第几个字符串
            while(sring >> word){
                if(i == 1)column = "*";
                else if(i == 2)tablename = word;
                else if(i >= 4 && i <= 6)cond.push_back(word);
                ++ i ;
            }
            int need=LocalMap.at(cond[1]);
            std::vector<Typee> typ = {{"int", "", normal}};
            Table gettable(Oral_path, tablename+".table", typ, AVL);
            gettable.ReadInBinary();
            gettable.delect(cond[2],need,column,cond[0]);
            gettable.showTable();
        }
        else if(operation == "use")
        {
            sring >>tablename;
            sring >>tablename;
            Oral_path=tablename;
        }
        else if(operation == "drop")
        {
            sring >> operation;
            sring >>tablename;
            if(operation == "table")
            {
                remove_file(Oral_path,tablename);
            }
            else
            {
                remove_files("./"+tablename);
            }
        }
        else if(operation == "quit")
        {
            exit(0);
        }
        else if(operation == "exit")
        {
            return false;
        }
        return true;
    }
private:
    std::string Oral_path="";
};
static void* client_thread(void *arg)
{
    int recvlen = 0;
    char recvbuf[1024] = "";
    long connfd = (long)arg;
    dbms_far dms;
    while((recvlen = recv(connfd,recvbuf,sizeof(recvbuf),0))>0)
    {
        std::cout<<"recv_buf:"<<recvbuf<<recvlen<<std::endl;
        std::mutex mut;
        send(connfd, recvbuf, recvlen, 0);
        mut.lock();
        std::string al(recvbuf);
        if(dms.eventLoop(al))     
        {
            memset(recvbuf,'\0',1024);
            std::cout<<">>";
        }   
        mut.unlock();
    }
    std::cout<<"client closed"<<recvbuf<<std::endl;
    close(connfd);
    nums--;
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
    printf("client ip=%s,port=%d,nums=%d\n", cli_ip,ntohs(client.sin_port),++nums);

    pthread_create(&thread_id,NULL,&client_thread,(void*)connfd);
    pthread_detach(thread_id);//线程分离，结束时自动回收线程
    }
    close(sockfd);
    return 0;
}
int main()
{
    std::cout <<"enter the type you want to get"<<std::endl;
    std::cout <<"1 for far connect and 0 for local connect"<<std::endl;
    std::cout <<"< ";
    int want_farconn=0;
    scanf("%d",&want_farconn);
    fflush(stdin);
    if(want_farconn)
        do_do();
    else
    {
        int recvlen = 0;
        char recvbuf[1024] = "";
        cin.getline(recvbuf,1024);
        dbms_far dms;
        std::string recvs(recvbuf);
        while(dms.eventLoop(recvs))     
            {
                std::cout<<"< ";
                cin.getline(recvbuf,1024);
                recvs.assign(recvbuf);
            }   
    }
}