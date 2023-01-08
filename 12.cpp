#include<iostream>
#include<sstream>
#include<vector>
#include<cstring>

//#include "Table.h"
#define normal 0
#define Primary_KY 1
using value_type = int;
void insert(std::string tablename, std::string value);
void create_table(std::string tablename, std::string value);

struct Typee
/**
 * @brief type: 类型
 *        id: 这一列的名称
 *        Restrict 限制，这个不用管�?
 */
{
    std::string type="int";
    std::string id="";
    value_type Restrict=normal;
};

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
        std::cout << recvbuf;

        if(operation == "insert"){
            sring >> tablename;//分离出tablename，再将字符串传给createtable
            insert(tablename, recvbuf);//, tab_path);
        }
        else if (operation == "create")
        {
            std::string str1 = recvbuf.substr(13);
            std::stringstream str2;
            str2 << str1;
            str2 >> tablename;//分离出tablename，再将字符串传给createtable
            create_table(tablename, recvbuf);
        }
        else{
            
        }
    }
};

//输入样例create table person (id int primary,name string)
//默认逗号间无空格，空格只可能出现在一个列名每个属性（id/int/primary都叫属性）之间
void create_table(std::string tablename, std::string value){
    int cnt = strlen("create table") + tablename.size() + 3;//跳到“（”位置后面一格，即开始读�?
    std::vector<Typee> table_info;//create_table用，(id int primary, name string)

    bool flag = true; //标识字符串是否便利完
    while(flag){
        int count = 1; //指示这是第几个类型，1代表id�?代表type�?代表热键与否
        Typee typee;
        while(value[cnt] != ','){
            char* tmp = (char*)malloc(sizeof(20));//存访typee中的一�?比如id
            int i = 0; //指示tmp数组
            while(value[cnt] != ' ' && value[cnt] != ',' && value[cnt] != ')'){
                tmp[i++] = value[cnt++];
            }

            if(value[cnt] == ')'){
                if(count == 2) typee.type = tmp;
                else typee.Restrict = Primary_KY;
                table_info.push_back(typee);
                flag =  false;
                break;
            }

            if(value[cnt] == ','){
                if(count == 2) typee.type = tmp;
                else typee.Restrict = Primary_KY;
                table_info.push_back(typee);
                break;
            }

            if(count == 1){
                typee.id = tmp;
                cnt ++;
                count ++;
            }
            else if(count == 2){
                typee.type = tmp;
                cnt ++;
                count ++;
            }
        }
        if(value[cnt] == ',')cnt++;

    }
    for(int i=0;i<=table_info.size()-1;++i){
        Typee a = table_info[i];
        std::cout << a.id << ' ' << a.type << ' ' << a.Restrict << std::endl;
        } //test
    
}