#include <Crone.h>
int main()
{
    std::string s;
    getline(std::cin,s);
    std::stringstream streamstring(s);
    DBMS dbms;
    while(dbms.eventLoop()==true){
         
    }
}