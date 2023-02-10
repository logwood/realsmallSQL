#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include "Table.h"
int find_start(std::string string) {
    int start = 0;

    while(true) {
        if (string[start] == ' ' || string[start] == ',' || string[start] == '(')
            ++start;
        else
            break;
    }

    return start;
}

int find_length(std::string string) {
    int length = 0;

    while(true) {
        if (string[length] != ' ' && string[length] != ',' && string[length] != ')')
            ++length;
        else
            break;
    }

    return length;
}

void analyze(std::string creat_table, std::vector<std::string>& table_info) {
    int start = strlen("create table ");
    int length;
    std::string substring = creat_table.erase(0, start);

    // 璇诲彇鎵€鏈変俊�??
    while(true) {
        start = find_start(substring);

        substring = substring.erase(0, start);
        length = find_length(substring);

        if (length != 0) {
            std::string value = substring.substr(0, length);
            table_info.push_back(value);
            start = value.length();
        } else {
            break;
        }

        substring = substring.erase(0, start);
    }
}

std::vector<Typee> execute(std::vector<std::string> table_info) {
    std::vector<Typee> header;

    int num = 0;
    int size = table_info.size();

    std::string Table_Name = table_info[num];
    ++num;

    while(num < size) {
        for(int i = 0; i < 3; ++i) {
            Typee col;
            col.id = table_info[num];
            col.type = table_info[num+1];
            if (num+2<size&&table_info[num+2] == "primary") {
                col.Restrict = 1;
                num += 3;
            } else {
                num += 2;
            }

            header.push_back(col);
            if(num>=size)
            {
                break;
            }
        }
    }
    return header;
}
std::vector<Typee> create_table(std::string creat_table,std::vector<Typee>& tel) {
    std::vector<std::string> table_info;
    analyze(creat_table, table_info);
    return execute(table_info);
}