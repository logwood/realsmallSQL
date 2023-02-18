#include<iostream>
#include<sstream>
#include<string.h>
#include<vector>
#include<map>
#include <sys/stat.h>
#include "CreateTable.h"

std::string oral_path="stud";
//table.h的191行继续进行，解决insert问题
//解决delete问题
//解决select问题
//把剩下的小东西整完
int main()
    {      
        //create table Persons ( Id_P int primary, LastName string , FirstName string , Address string , City string ) 
        //select books from stud where books > 7777
        //insert into stud ( buok , books ) values ( jack , 25 ) , ( back , 28 ) , ( hek , 24 )
        //select books from stud where books = 25
        //delete from stud where books > 77777
        std::string recvbuf = "drop table Person";
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
            Table gettable(oral_path, tablename+".table", typ, BPLUS);
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
                Table gettable(oral_path,tablename+".table",cirr,BPLUS);
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
            Table gettable(oral_path, tablename+".table", typ, AVL);
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
            Table gettable(oral_path, tablename+".table", typ, AVL);
            gettable.ReadInBinary();
            gettable.delect(cond[2],need,column,cond[0]);
            gettable.showTable();
        }
        else if(operation == "use")
        {
            sring >>oral_path;
        }
        else if(operation == "drop")
        {
            sring >> operation;
            sring >>tablename;
            if(operation == "table")
            {
                remove_file(oral_path,tablename);
            }
            else
            {
                remove_files("./"+tablename);
            }
        }
        return 0;
    }