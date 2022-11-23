#include "Crone.h"
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
   Table(std::string Base_Path, std::string Table_Name, std::vector<Typee> typs)
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
      for (auto tyee : typs)
      {
         Column types;
         types.id = tyee.id, types.Restrict = tyee.Restrict, types.type = tyee.type, types.Columns = {""};
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
      else
      {
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
      file.write((char *)&nums, sizeof(int));
      file.write((char *)&numss, sizeof(int));
      for (auto Column_every : Column_Info)
      {
         type_Document types;
         const char *num = Column_every.id.data();
         sprintf(types.id_convert, "%s", Column_every.id.data());
         sprintf(types.type_convert, "%s", Column_every.type.data());
         types.Restrict = Column_every.Restrict;
         file.write((char *)&types, sizeof(types));
      }
      for (auto Column_every : Column_Info)
         for (auto info : Column_every.Columns)
         {
            info_Document infos;
            sprintf(infos.Info_One, "%s", info.data());
            file.write((char *)&infos, sizeof(infos));
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
         std::cout << Column_Info[c].id << Column_Info[c].Restrict << Column_Info[c].type << std::endl;
      }
      for (int j = 0; j < numss; j++)
      {
         for (int i = 0; i < nums; i++)
         {
            file.read((char *)&info, sizeof(info));
            Column col;
            Column_Info[i].Columns.emplace_back(std::string{""});
            Column_Info[i].Columns[j] = info.Info_One;
            std::cout << Column_Info[i].Columns[j] << std::endl;
         }
      }
   }
   void CloseTable()
   {
      this->~Table();
   }
   void dothings()
   {
      std::vector<Typee> types = {{"string", "ok", Primary_KY}, {"string", "buok", normal}, {"int", "books", normal}};
      std::vector<Typee> typ = {{"int", "", normal}};
      Table table("stud", "stud.table", types); //这是读的实例
      table.InsertToTail({(std::string) "wng", (std::string) "table", (std::string) "7777"});
      table.InsertToTail({(std::string) "gsang", (std::string) "hekoo", (std::string) "1329"});
      table.InsertToTail({(std::string) "wsang", (std::string) "ettkd", (std::string) "2392"});
      table.InsertToTail({(std::string) "wang", (std::string) "le", (std::string) "7722277"});
      table.InsertToTail({(std::string) "wag", (std::string) "h", (std::string) "1322229"});
      table.InsertToTail({(std::string) "w", (std::string) "kd", (std::string) "2323232392"});
      table.WriteInBinary(); //这是写的实例。
      Table gettable("stud", "stud.table", typ);
      gettable.ReadInBinary();
   }

private:
   std::vector<Column> Column_Info; //表的信息，包括了类型。
   std::string File_Path;           // File_Path 是Database的文件路径。
   std::string Table_is;            //数据库的Table名。
};