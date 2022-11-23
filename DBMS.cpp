#include "Crone.h"
#include "dbms.h"
bool DBMS::eventLoop()

{
    bool end = false;
    std::string s;
    last_sql.clear();
    getline(std::cin, s);

    std::stringstream ss;
    try
    {
        std::string a1;
        last_sql << s;
        if (options == -1)
        {
            if (s.length() == 0)
            {
                return true;
            }
            last_sql >> a1;
            options = LocalMap.at(a1);
        }
        for (std::string s1; last_sql; last_sql >> s1)
        {
            if (s1 != ";")
            {
                Strs.emplace_back(s1);
                char a = *((s1.end() - 1).base());
                if (*((s1.end() - 1).base()) == ';')
                {
                    end = true;
                    s1.pop_back();
                    last_sql << ";";
                }
            }
            if (s1 == ";" || end)
            {
                doaParser();
                options = -1;
                Strs.clear();
                s.clear();
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Oh No an error occured at" << e.what() << '\n';
        last_sql.str("");
        last_sql.clear();
        Strs.clear();

    }
    return true;
};
bool DBMS::doaParser()
{
    std::cout << Strs[0] << std::endl;
    doCreateTable();
    return true;
}
bool DBMS::doCreateTable()
{
    if (Strs.size() == 0)
    {
        return false;
    }
    std::vector<Typee> typ = {{"int", "", normal}};
    Typee typee;
    std::string Table_String = *Strs.begin().base();
    Strs.erase(Strs.begin());
    int a=Strs.end().base()->size();
    if ((Strs.end()-1).base()->size()>2&&Strs[Strs.size()-1].back()==';')
    {
        (Strs.end()-1).base()->pop_back();
        (Strs.end()-1).base()->pop_back();
        if(*(Strs.end()-1).base()=="")
        {
            Strs.erase(Strs.end());
        }
    }
    else if((Strs.end()-1).base()->size()==1){
        Strs.erase(Strs.end()-1);
    }
    else{
        (Strs.end()-1).base()->pop_back();
    }
    while (true)
    {
        if (*Strs.begin().base() == "(")
        {
            Strs.erase(Strs.begin());
            break;
        }
        else if (*Strs[0].begin().base() == '(')
        {
            Strs[0].erase(Strs[0].begin());
            break;
        }
        else if ((int)Strs[0].find_first_of("(") > 0)
        {
            Strs[0] = Strs[0].substr(Strs[0].find_first_of("("));
        }
        else
        {
            Strs.erase(Strs.begin());
        }
    }
    while (Strs[0].size() == 0)
    {
        Strs.erase(Strs.begin());
    }
    std::string Restricts;
    int stage = STAGE_ONE;
    for (auto iter = Strs.begin(); iter != Strs.end(); std::cout << *iter.base() << std::endl, iter++)
    {
        if (*iter.base() != ",")
        {
            switch (stage)
            {
            case STAGE_ONE:
                typee.id = *iter.base();
                stage++;
                break;
            case STAGE_TWO:
                typee.type = *iter.base();
                stage++;
                break;
            default:
                while (iter != Strs.end())
                {
                    if ((*iter.base()->begin().base()) == ',')
                    {
                        iter.base()->erase(iter.base()->begin());
                        --iter;
                        stage = STAGE_ONE;
                        typee.Restrict=GetNumTable.at(Restricts);
                        typ.emplace_back(typee);
                        std::cout << Restricts << typee.type << typee.id << std::endl;
                        Restricts.clear();

                        break;
                    }
                    else if (iter.base()->back() == ',')
                    {

                        Restricts += *iter.base();
                        stage = STAGE_ONE;
                        typee.Restrict=GetNumTable.at(Restricts);
                        std::cout << Restricts << typee.type << typee.id << std::endl;
                        typ.emplace_back(typee);
                        typee.Restrict=GetNumTable.at(Restricts);
                        Restricts.clear();
                        break;
                    }
                    else if ((int)iter->find_first_of(",") > 0)
                    {
                        std::string s=iter->substr(1+iter->find_first_of(","));
                        *iter.base() = iter->substr(0,iter->find_first_of(","));
                        Restricts += *iter.base();
                        *iter.base() = s;
                        iter--;
                        stage=STAGE_ONE;
                        typee.Restrict=GetNumTable.at(Restricts);
                        typ.emplace_back(typee);
                        std::cout << Restricts << typee.type << typee.id << std::endl;
                        Restricts="";
                        break;
                    }
                    else
                    {
                        Restricts += *iter.base();

                        ++iter;
                    }
                }
                if(iter==Strs.end())
                {
                    iter--;
                }
                break;
            }
        }
        else{
            if(stage==2)
            {
                Restricts="";
            }
            stage=1;
            typee.Restrict=GetNumTable.at(Restricts);
            typ.emplace_back(typee);
            std::cout << Restricts << typee.type << typee.id << std::endl;
        }
    }
    typee.Restrict=GetNumTable.at(Restricts);
    typ.emplace_back(typee);
    typ.erase(typ.begin());
    std::cout << Restricts << typee.type << typee.id << std::endl;
    return true;
}
int main()
{
    DBMS dms;
    dms.eventLoop();
    while (dms.eventLoop() == true)
    {
        std::cin.clear();
    }
}
//CREATE TABLE SPACE(SPRING spring NOT NULL,STIR int , STSD int NOT NULL ) ; 