#ifndef CRONE_H
#include "Crone.h"
#endif
#include "index.h"
#include <iostream>
#include <algorithm>
#include "Bplustree/BplusFactory.h"
#include <set>
#define BPLUS 1
#define AVL 0
/**
 * @brief 注意 type_Document 和 info_Document都是用于读写的函数。
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
    *     类型的名字。   such_as:string,int,class,可以看输入的样例。
    *
    */
   {
      //std::cout << "" << std::endl;
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
       * @brief 判断输入逻辑正误的函数
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
   Table &InsertToPlace(std::vector<std::vector<std::string>> infos)
   { /**
      * @brief 用于在尾部插入数据
      *        infos 是数据的信息。
      *        !!!!!!!!!!!!!!所有数据，无论类型是什么，统一通过string存储。
      */
      auto place=infos.front();
      if(place.capacity()!=0)
      {
         infos.erase(infos.begin());
         infos.erase(infos.begin());
         auto irf=infos;
         irf.resize(infos[0].size());
         for(auto& it:irf)
         it.resize(infos.size());
         int i=0;
         for(auto info:infos)
         {
            int j=0;
            for(auto inf:info)
            {
               irf[j][i]=infos[i][j];
               j++;
            }
            i++;
         }
         i=0;
         ReadInBinary();
         while(Column_Info.size()>irf.size())
            {
               irf.emplace_back(irf.back());
               for(auto &kk:irf.back())
               {
                  kk="";
               }
            }
      i=0;
      int j=0;
      decltype(irf) irr;
      irr.resize(irf.size());
      for(auto pla:place){
         for(auto Col:Column_Info)
            {
               if(!Col.id.compare(pla))
               {
                  irr[i]=irf[j];
               }
            i++;
         }
         i=0;
         j++;
      }
      for(auto &arr:irr)
      {
         if(arr.size()==0)
         {
            arr.resize(Column_Info.size(),"NULL");            
         }
      }
      auto irk=irr;
         irk.resize(irr[0].size());
         for(auto& it:irk)
         it.resize(irr.size());
         i=0;
         for(auto info:irr)
         {
            int j=0;
            for(auto ink:info)
            {
               irk[j][i]=irr[i][j];
               j++;
            }
            i++;
         }
      for (auto info : irk)
         {
            InsertToTail(info);
         }
      //  Index *index = new Index(File_Path,Column_Info.size(),dec);       
      //  j=0;
      //  for(auto ase:Column_Info)
      //  {
      //  if(ase.Restrict==Primary_KY)
      //  {
      //     break;
      //  }
      //     j++;
      //  }   
      //  if(Column_Info[j].type=="int")
      //    for(auto info:irk)
      //    {
      //       index->avl_insert(atoi(info[j].c_str()),info);
      //    }
      //  index->showBtreeByLeftAndWrite();
      }
      else
      {
         infos.erase(infos.begin());
         for (auto info : infos)
         {
            InsertToTail(info);
         }    
      }
      WriteInBinary();          
      return *this;
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
   { 
      BAddTree<int,Entry> tree(orders);
      int nots=1;
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
            if(types.type_convert=="string")
            {
               nots=0;
            }
         }
         place++;
         file.write((char *)&types, sizeof(types));
      }
      int need=0;      
      for(auto Col:Column_Info)
      {
         if(Col.Restrict==Primary_KY)
         {
            break;
         }
         need++;
      }
      int m=0;
      File_Offset.clear();
      
      for (auto Column_every : Column_Info)
      {
         int i = file.tellp();
         File_Offset.emplace_back(i);
         int j=0;
         for (auto info : Column_every.Columns)
         {
            info_Document infos;
            sprintf(infos.Info_One, "%s", info.data());
            file.write((char *)&infos, sizeof(infos));
            if(m==0&&nots==1)
            {
            long k = file.tellp();
            //std::cout << i << std::endl;
            Entry ant(atoll(Column_Info[need].Columns[j].c_str()),k);
            tree.insert(ant);
            }
            j++;
         }
         m++;
      }
      if(nots==1)
      {
      tree.searchall("Bplustree/idx/"+ Table_is);      
      std::vector<std::string> dec;
      for(auto cols:Column_Info)
      {
         dec.emplace_back(cols.id);
      }       
      Index *index = new Index(Table_is,Column_Info.size(),dec);    
      std::vector<std::string> decc;
      for(int i=0;i<Column_Info[0].Columns.size();i++)
      {
         for(auto cols:Column_Info)
         {
         decc.emplace_back(cols.Columns[i]);
         }
         index->avl_insert(atoi(Column_Info[need].Columns[i].c_str()),decc);
      }
      index->showBtreeByLeftAndWrite();
      int i=1+1;
      }
      file.close();
      return *this;
   }   
   void ReadInBinary()
   {
      /**
       * @brief 将二进制文件TABLE_NAME.table读出
       *        
       *
       */
      type_Document temptype;
      info_Document info;
      std::ifstream file(File_Path + "/" + Table_is,
                         std::ios::in | std::ios::binary);
      BAddTree<int,Entry> b(orders);
      if(!file.is_open())
      {
         file.close();
         return;
      }
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
      }
      Column_Info.erase(Column_Info.begin());
      Column types;
      types.id = "", types.Restrict = 0, types.type = "", types.Columns = {""};
      Column_Info.emplace_back(types);
      std::ifstream in("Bplustree/idx/"+Table_is,std::ios::in|std::ios::binary);
      // if(!in.eof())
 	   // b.ReadBtree(0,in);
      // std::cout<<"read things"<<std::endl;
      // b.tree_traversal(itf);
      File_Offset.clear();
      for (int j = 0; j < nums; j++)
      {
            auto offset = file.tellg();
            File_Offset.emplace_back(offset);
         if (tree_type == AVL)
         {
            /*insert AVL tree things*/
         }
         for (int i = 0; i < numss; i++)
         {
            if (j == 0 && i >= 0 )
            {
               auto offset = file.tellg();
               //std::cout <<offset<<std::endl;
            }
            file.read((char *)&info, sizeof(info));
            Column col;
            Column_Info[j].Columns.emplace_back(std::string{""});
            Column_Info[j].Columns[i] = info.Info_One;
         }
         Column_Info[j].Columns.pop_back();
         auto iter = Column_Info[j].Columns.begin();
      }
      Column_Info.pop_back();
   }
   void CloseTable()
   {
      this->~Table();
   }
   void parseBplus(std::deque<long> places,int options)
   {
      int i=0;
      info_Document info;
      std::ifstream file(File_Path + "/" + Table_is, std::ios::in | std::ios::binary);
      auto ref=*this;
      Column_Info.clear();
      while (places.size())
      {
         file.seekg(*places.begin()-sizeof(info));
         places.erase(places.begin());
         int j=0;
         std::vector<std::string> acc;
         if(i==0)
         for(auto COL:ref.Column_Info)
         {
            Column_Info.emplace_back();
            Column_Info[i].id=COL.id;
            Column_Info[i].Restrict=COL.Restrict;
            Column_Info[i].type=COL.type;
            i++;
         }
         while (j!=File_Offset.size())
         {
            file.read((char*)&info,sizeof(info));
            acc.emplace_back(info.Info_One);
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
         InsertToTail(acc);
      }
      showTable();
      Column_Info=ref.Column_Info;
   }
   void delect(std::string vaild_key,int operators,std::string column_needs,std::string column_opers)
   {
      std::vector<int> vec=search(vaild_key,operators,column_needs,column_opers,114514);
      std::set<int>s(vec.begin(), vec.end());
      vec.assign(s.begin(), s.end());
      
      for(auto& col:Column_Info)
      {
         col.Columns.clear();
      }
      Column_Info.resize(1);
      ReadInBinary();
      for(auto ve:vec)
      {
         Column_Info[0].Columns[ve]="C##DBA_CONSERED";
      }
      //WriteInBinary();
      decltype(Column_Info) coll;
      coll.resize(Column_Info.size());
      for(int i=0;i<Column_Info[0].Columns.size();i++)
      {
         
         if(Column_Info[0].Columns[i].compare("C##DBA_CONSERED"))
         {
            for(int j=0;j<Column_Info.size();j++)
            {
               coll[j].id=Column_Info[j].id;
               coll[j].Columns.emplace_back(Column_Info[j].Columns[i]);
            }
         }
      }
      int i=0;
      for(auto Cols:Column_Info)
      {
         coll[i].Restrict=Cols.Restrict;
         coll[i].id=Cols.id;
         coll[i].type=Cols.type;
         i++;
      }
      Column_Info=coll;
      WriteInBinary();
      int j=1+1;
   }
   std::vector<int> search(std::string vaild_key,int operators,std::string column_needs,std::string column_opers,int options)
   {
      tree_type=options;
      return search(vaild_key,operators,column_needs,column_opers);
   }
   std::vector<int> search(std::string vaild_key,int operators,std::string column_needs,std::string column_opers)
   {
      std::vector<Column> cols;
      std::vector<int> times;
      int i = 0;
      int k = 0;
      int keyis=0;
      for (auto Columns : Column_Info)
      {
         cols.push_back(Columns);
         if(Columns.Restrict==Primary_KY&&Columns.type=="int"&&column_needs==Columns.id)
         {
            keyis=1;
         }
      }
       if(keyis=1)
       {
          BAddTree<int,Entry> b(orders);
          std::ifstream in("Bplustree/idx/"+Table_is,std::ios::in|std::ios::binary);
          if(in.is_open())
          {
          if(tree_type!=114514)
          {
 	       b.ReadBtree(0,in);
          deque<long> ant=b.list_traversal(itfs,operators,atoll(vaild_key.c_str()));
          std::ifstream input(File_Path+"/"+Table_is,std::ios::in|std::ios::binary);
          parseBplus(ant,tree_type);
          return {};
          }
          }
       }
      for (auto Columns : Column_Info)
      {  
         if(tree_type!=AVL)
         if (!Columns.id.compare(column_opers))
         {
            for (auto colss : Columns.Columns)
            {
               if (!cmp_fuc(colss,vaild_key,operators,Columns.id))
               {
                  int j = 0;
                  for (auto Colum : cols)
                  {
                     auto suck = Colum.Columns.begin() + i;
                     Colum.Columns.erase(Colum.Columns.begin() + i);
                     cols[j] = Colum;
                     j++;
                     times.push_back(k);
                  }
                  i--;
               }
               i++;
               k++;
            }
            break;
          }
          else if(tree_type==AVL)
          {
             Index *ind =new Index(Table_is);
             std::vector<std::vector<std::string>> inde;
             if(inde.empty())
             {
               std::cout<<"OOPS,NO This file"<<std::endl;
               return {};
             }
             ind->readInHex();
             inde=ind->search(atoi(vaild_key.c_str()),2,operators);
             if(operators==0)
             {
               auto indes=ind->search(114514,2,BIGGER);
               inde.emplace_back(indes[0]);
             }
             std::vector<Typee> types;
             std::vector<std::string> inds=inde.back();
             for(auto indee:inds)
            {
                Typee typ;
                typ.id=indee;
                types.push_back(typ);
                Table table(File_Path, Table_is, types, AVL); //这是读的实例
                Column_Info=table.Column_Info;
             }
             inde.pop_back();
             for(auto indee:inde)
             {
                //std::cout<<indee.size()<<std::endl;;
                InsertToTail(indee);
             }
             if(column_needs.compare("*"))
             for (auto& Columns : Column_Info)
             {
             if (!Columns.id.compare(column_needs))
             {
               Columns.type="Selected";
             }
             }
             std::vector<Column> arr;
             for(auto Columns:Column_Info)
             {
               if(Columns.type=="Selected")
               {
                  arr.emplace_back(Columns);
               }
             i++;
             }
             Column_Info=arr;
             return times;
          }
      }
      Column_Info=cols;
      i=0;
      if(column_needs.compare("*"))
      for (auto Columns : Column_Info)
      {
         if (Columns.id.compare(column_needs))
         {
            cols.erase(cols.begin()+i);
            i--;
         }
         i++;
      }
      Column_Info=cols;
      return times;
   }

   //--------------------------------------------------------------------
   void showTable() {
    std::vector<int> colMaxLength;

    //Column_Info 就是 this.Column_Info
    for (int i = 0; i < Column_Info.size(); ++i)
      {
        colMaxLength.push_back(std::max(getColMaxLength(Column_Info[i]),(int)Column_Info[i].id.size()));
      }
    //打印表头
    printLine(colMaxLength);
    printData(-1, colMaxLength);
    printLine(colMaxLength);

    //打印数据
    for (int i = 0; i < Column_Info[0].Columns.size(); ++i)
        printData(i, colMaxLength);

    //打印表尾
    printLine(colMaxLength);
   }
   int getColMaxLength(Column column) {
   if(column.Columns.size()!=0)
   {
    int maxLength = column.Columns[0].size();
    for (int i = 1; i < column.Columns.size(); ++i) {
        if (maxLength < column.Columns[i].size()) {
            maxLength = column.Columns[i].size();
        }
    }
    return maxLength;
   }
}//打印边框
void printLine(std::vector<int> colMaxLength) {
    for (int i = 0; i < colMaxLength.size(); ++i) {
        std::cout << "+-";
        for (int j = 0; j < colMaxLength[i]; ++j)
            std::cout << "-";
        std::cout << "-";
    }
    std::cout << "+" << std::endl;
}

//打印数据 | 打印表头
void printData(int row, std::vector<int> colMaxLength) {
    int length;

    for (int i = 0; i < colMaxLength.size(); ++i) {
        std::cout << "| ";

        if (row == -1) {
            std::cout << Column_Info[i].id;
            length = Column_Info[i].id.size();
        } else {
            std::cout << Column_Info[i].Columns[row];
            length = Column_Info[i].Columns[row].size();
        }
        
        for (int j = 0; j < colMaxLength[i] - length; ++j)
            std::cout << " ";
        std::cout << " ";
    }
    std::cout << "|" << std::endl;
}
void mian()//测试样例
{
    std::vector<Typee> types = {{"string", "ok", normal}, {"string", "buok", normal}, {"int", "books", Primary_KY}};
    std::vector<Typee> typ = {{"int", "", normal}};
    Table table("stud", "stud.table", types, BPLUS); // 这是读的实例
    table.InsertToTail({(std::string) "wng", (std::string) "table", (std::string) "7777"});
    table.InsertToTail({(std::string) "gsang", (std::string) "hekoo", (std::string) "1329"});
    table.InsertToTail({(std::string) "wsang", (std::string) "ettkd", (std::string) "2392"});
    table.InsertToTail({(std::string) "wang", (std::string) "le", (std::string) "7722277"});
    table.InsertToTail({(std::string) "wag", (std::string) "h", (std::string) "1322229"});
    table.InsertToTail({(std::string) "w", (std::string) "kd", (std::string) "3232392"});
    table.InsertToTail({(std::string) "sas", (std::string) "tasdsaable", (std::string) "323277"});
    table.InsertToTail({(std::string) "asasg", (std::string) "hasdasekoo", (std::string) "12323329"});
    table.InsertToTail({(std::string) "sadg", (std::string) "easdsattkd", (std::string) "23212192"});
    table.InsertToTail({(std::string) "adsg", (std::string) "sadsale", (std::string) "2122277"});
    table.InsertToTail({(std::string) "sadg", (std::string) "asdsah", (std::string) "23232229"});
    table.InsertToTail({(std::string) "wasd", (std::string) "sadd", (std::string) "2232392"});
    table.WriteInBinary(); // 这是写的实例。
    Table gettable("stud", "stud.table", typ, BPLUS);
    gettable.ReadInBinary();
    gettable.parseBplus({140, 159, 178},114514);
}
   //--------------------------------------------------------------------
private:
   bool cmp_fuc(std::string left_value,std::string right_value,int operators,std::string type)
   {
      if(type.compare("int"))
      {
         return operators!=0?((atoll(left_value.c_str())-atoll(right_value.c_str()))*operators)>0:!(left_value==right_value);
      }
      else
      {
         return !operators?(left_value.compare(right_value))*operators:!(left_value==right_value);
      }
   }
   int tree_type = AVL;
   std::vector<Column> Column_Info; //表的信息，包括了类型。
   std::string File_Path;           // File_Path是Database的文件路径。
   std::string Table_is;            //数据库的Table名。
   std::vector<int> File_Offset;
   int orders=7;
};
