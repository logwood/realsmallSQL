#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <vector>
void remove_file (std::string oral_path,std::string tablename)
{
    remove((oral_path+"/"+tablename).c_str());
    remove(("idx/"+tablename).c_str());
    remove(("Bplustree/idx/"+tablename).c_str());
    remove(oral_path.c_str());
}
std::vector<std::string> remove_files(std::string dir_name){
    std::vector<std::string> result;
    const char *dir_name_c = dir_name.c_str();
    struct dirent *filename;
    DIR *dir;
    dir = opendir(dir_name_c);
    while ((filename = readdir(dir)) != NULL)
    {
        std::string filePath = dir_name;
        if(strcmp(filename->d_name, ".") == 0 ||
           strcmp(filename->d_name, "..") == 0)
           continue;
        filePath = filename->d_name;
        remove_file(dir_name,filePath);
    }
    remove(dir_name.c_str());
    return result;
};