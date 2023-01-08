#ifndef CRONE_H
#include "Crone.h"
#endif
#include <iostream>
#define BPLUS 1
#define AVL 0
/**
 * @brief 注意 type_Document 和 info_Document都是用于读写的函数，这个如果想要了解的话找我私聊。
 *
 */
struct type_Document
{
   char id_convert[MAX_SIZE];
   char type_convert[MAX_SIZE];
   int Restrict;
};
struct info_Document
{
   char Info_One[MAX_SIZE];
};
class Table
{
public:
   Table(std::string Base_Path, std::string Table_Name, std::vector<Typee> typs, int tree_ype)
   /**
    * @brief 这个是初始化函数，可以通过这里创建一个Table类。
    *     Base_Path 数据库的路径
    *     Table_Name 表的名字。
    *     类型的名字。   such_as:string,int,class,可以看我输入的样例。
    *
    */
   {
      std::cout << "" << std::endl;
      File_Path = Base_Path;
      Table_is = Table_Name;
      tree_type = tree_ype;
      for (auto tyee : typs)
      {
         Column types;
         types.id = tyee.id, types.Restrict = tyee.Restrict, types.type = tyee.type, types.Columns = {""};
         types.Columns.pop_back();
         Column_Info.emplace_back(types);
      }
   }
   ~Table()
   {
   }
   bool CheckRightness(std::vector<std::string> &infos)
   {
      /**
       * @brief 判断输入逻辑正误的函数，可以不管
       *
       */
      try
      {
         if (infos.size() > Column_Info.size())
         {
            throw std::out_of_range("You have so much input");
         }
         else
         {
         }
         if (std::__count_if(Column_Info.begin(), Column_Info.end(),
                             [](decltype(Column_Info.begin()) s)
                             { return s.base()->Restrict == Primary_KY || s.base()->Restrict == Foreign_KY; }) >= 2)
         {
            throw std::logic_error("As you know,there should always only one PK or Foreign_KY in a table");
         }
      }
      catch (const std::exception &e)
      {
         std::cerr << e.what() << '\n';
         return false;
      }

      return true;
   }
   Table &InsertToTail(std::vector<std::string> infos)
   { /**
      * @brief 用于在尾部插入数据
      *        infos 是数据的信息。
      *        !!!!!!!!!!!!!!所有数据，无论类型是什么，统一通过string存储。
      */
      if (CheckRightness(infos))
      {
         int i = 0;
         for (auto info : infos)
         {
            Column_Info[i].Columns.emplace_back(info);
            i++;
         }
      }
      return *this;
   }
   Table &WriteInBinary()
   { /**
      * @brief 将类写到二进制文件TABLE.table
      *        ps:如果有需要的话，可以私聊我，加入一个options，用于数据存储的方式。具体细节请看ofstream。
      *
      */
      std::ofstream file(File_Path + "/" + Table_is, std::ios::out | std::ios::binary);
      int nums = Column_Info.size();
      int numss = Column_Info[0].Columns.size();
      int primaryplace = 0;
      int place = 0;
      file.write((char *)&nums, sizeof(int));
      file.write((char *)&numss, sizeof(int));
      for (auto Column_every : Column_Info)
      {
         type_Document types;
         const char *num = Column_every.id.data();
         sprintf(types.id_convert, "%s", Column_every.id.data());
         sprintf(types.type_convert, "%s", Column_every.type.data());
         types.Restrict = Column_every.Restrict;
         if (types.Restrict == Primary_KY)
         {
            primaryplace = place;
         }
         place++;
         file.write((char *)&types, sizeof(types));
      }
      for (auto Column_every : Column_Info)
      {
         if (tree_type == BPLUS)
         {
            int i = file.tellp();
            File_Offset.emplace_back(i);
            std::cout << i << std::endl;
            Column_Info[primaryplace];
         }
         for (auto info : Column_every.Columns)
         {
            info_Document infos;
            sprintf(infos.Info_One, "%s", info.data());
            file.write((char *)&infos, sizeof(infos));
         }
      }

      file.close();
      return *this;
   }
   void ReadInBinary()
   {
      /**
       * @brief 将二进制文件TABLE_NAME.table读出
       *        ps:如果有需要的话，可以私聊我，加入一个options，用于数据存储的方式。具体细节请看ofstream。
       *
       */
      type_Document temptype;
      info_Document info;
      std::ifstream file(File_Path + "/" + Table_is,
                         std::ios::in | std::ios::binary);
      int nums, numss;
      file.read((char *)&nums, sizeof(int));
      file.read((char *)&numss, sizeof(int));
      int c = 0;
      while (c != nums)
      {
         Column types;
         types.id = "", types.Restrict = 0, types.type = "", types.Columns = {""};
         Column_Info.emplace_back(types);
         file.read((char *)&temptype, sizeof(temptype));
         c++;
         std::string str1 = temptype.id_convert;
         Column_Info[c].id.swap(str1);
         std::string str2 = temptype.type_convert;
         Column_Info[c].type.swap(str2);
         Column_Info[c].Restrict = temptype.Restrict;
         // std::cout << Column_Info[c].id << Column_Info[c].Restrict << Column_Info[c].type << std::endl;
      }
      Column_Info.erase(Column_Info.begin());
      Column types;
      types.id = "", types.Restrict = 0, types.type = "", types.Columns = {""};
      Column_Info.emplace_back(types);
      for (int j = 0; j < nums; j++)
      {
         auto offset = file.tellg();
         std::cout<<offset<<std::endl;
         File_Offset.emplace_back(offset);
         if (tree_type == AVL)
         {
            /*insert AVL tree things*/
         }
         for (int i = 0; i < numss; i++)
         {
            if (j == 0 && i > 0 && tree_type == BPLUS)
            {
               auto offset = file.tellg();
            }
            file.read((char *)&info, sizeof(info));
            Column col;
            Column_Info[j].Columns.emplace_back(std::string{""});
            Column_Info[j].Columns[i] = info.Info_One;
            std::cout << Column_Info[j].Columns[i] << std::endl;
         }
         Column_Info[j].Columns.pop_back();
         auto iter = Column_Info[j].Columns.begin();
         // Column_Info[j].Columns.erase(iter);
      }
      Column_Info.pop_back();
   }
   void CloseTable()
   {
      this->~Table();
   }
   void parseBplus(std::vector<int> places)
   {
      info_Document info;
      std::ifstream file(File_Path + "/" + Table_is, std::ios::in | std::ios::binary);
      while (places.size())
      {
         file.seekg(*places.begin().base());
         places.erase(places.begin());
         int j=0;
         while (j!=File_Offset.size())
         {
            file.read((char*)&info,sizeof(info));
            if(j!=0)
            {
            file.seekg(File_Offset[j]-File_Offset[j-1]-sizeof(info)+file.tellg());
            }
            else if(File_Offset.size()!=1)
            {
               file.seekg(File_Offset[1]-File_Offset[0]-sizeof(info)+file.tellg());
            }
            j++;
         }
      }
   }
   void search()
   {
      std::vector<Column> cols;
      int i = 0;
      for (auto Columns : Column_Info)
      {
         cols.push_back(Columns);
      }
      for (auto Columns : Column_Info)
      {
         if (Columns.id == "books")
         {
            for (auto colss : Columns.Columns)
            {
               if (atoi(colss.c_str()) > 2392)
               {
                  int j = 0;
                  for (auto Colum : cols)
                  {
                     auto suck = Colum.Columns.begin() + i;
                     Colum.Columns.erase(Colum.Columns.begin() + i);
                     cols[j] = Colum;
                     j++;
                  }
               }
               i++;
            }
         }
      }
   }

   //--------------------------------------------------------------------
   void showTable();
   void printLine(std::vector<int> colMaxLength);
   void printData(int row, std::vector<int> colMaxLength);
   int tree_type = 0;
   //--------------------------------------------------------------------
private:
   std::vector<Column> Column_Info; //表的信息，包括了类型。
   std::string File_Path;           // File_Path是Database的文件路径。
   std::string Table_is;            //数据库的Table名。
   std::vector<int> File_Offset;
};
void dothings()
{
   std::vector<Typee> types = {{"string", "ok", Primary_KY}, {"string", "buok", normal}, {"int", "books", normal}};
   std::vector<Typee> typ = {{"int", "", normal}};
   Table table("stud", "stud.table", types, BPLUS); //这是读的实例
   table.InsertToTail({(std::string) "wng", (std::string) "table", (std::string) "7777"});
   table.InsertToTail({(std::string) "gsang", (std::string) "hekoo", (std::string) "1329"});
   table.InsertToTail({(std::string) "wsang", (std::string) "ettkd", (std::string) "2392"});
   table.InsertToTail({(std::string) "wang", (std::string) "le", (std::string) "7722277"});
   table.InsertToTail({(std::string) "wag", (std::string) "h", (std::string) "1322229"});
   table.InsertToTail({(std::string) "w", (std::string) "kd", (std::string) "2323232392"});
   // table.showTable();
   table.WriteInBinary(); //这是写的实例。
   Table gettable("stud", "stud.table", typ, BPLUS);
   gettable.ReadInBinary();
   // gettable.showTable();
   gettable.search();
   gettable.parseBplus({140,159,178});
}