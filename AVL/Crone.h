#include <iostream>
#include <vector>
#include <stdbool.h>
#include <fstream>
#include <sstream>
#define MAX_SIZE 19
#define normal 0
#define Primary_KY 1
#define Foreign_KY 2
#define NOT_NULL 3
using value_type = int; //类型别名
struct Typee
/**
 * @brief type: 类型
 *        id: 这一列的名称
 *        Restrict 限制，这个不用管。
 * 
 */
{
    std::string type="int";
    std::string id="";
    value_type Restrict=normal;
};
struct Column : Typee
{
    std::vector<std::string> Columns;
};//作为数据库的一行，存储信息。
class Database
{
public:
    void CreateDatabase();
    void DropDatabase();
    void GetTable();
    void DropTable()
    {

    };
    Database(std::string link_place);
    ~Database();
    //class calasate;数据库的级联，以后会做。
    //void MakeLink();以后在搞
private:
    std::string link_place;//这个link_space是文件的路径，用于打开文件流。
};

class DBMS
{
public:
    std::string path;
    DBMS(){

    };
    int getRestrict(std::vector<std::string>::iterator);
    bool doCreateTable();
    bool eventLoop(); //事件循环，用于对输入的事件进行处理。可以考虑使用输入输出流。
    bool doaParser(); /* -- do some control */
    bool InsertData();
    void PrintTheScreen();//接收一个Column_Info，然后将其输入到命令行上。最好能达成我发的图片的效果。
    void MakeCommands(std::string words);//通过循环将任务发送到DBMS中。
    void parsevector(std::string Left_Words);
    bool doInsertTable();
    bool departend();
    /**
     * @brief 这个是下次要搞的,并且很重要，大约是要用到树
     *
     */
    private:
      std::stringstream last_sql;
      value_type options=-1;
      std::vector<std::string>Strs;
      std::string using_database;
      
};
class Linker
{
    private:
       char* ip;
};