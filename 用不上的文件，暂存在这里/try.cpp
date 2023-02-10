#include "Table.h"
//--------------------------------------------------------------------
int getColMaxLength(Column column) {
    int maxLength = column.Columns[0].size();

    for (int i = 1; i < column.Columns.size(); ++i) {
        if (maxLength < column.Columns[i].size()) {
            maxLength = column.Columns[i].size();
        }
    }
    return maxLength;
}
void Table::showTable() {
    std::vector<int> colMaxLength;

    //Column_Info 就是 this.Column_Info
    for (int i = 0; i < Column_Info.size(); ++i)
        colMaxLength.push_back(getColMaxLength(Column_Info[i]));

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

//找出一列中最长的元素的长度


//打印边框
void Table::printLine(std::vector<int> colMaxLength) {
    for (int i = 0; i < colMaxLength.size(); ++i) {
        std::cout << "+-";
        for (int j = 0; j < colMaxLength[i]; ++j)
            std::cout << "-";
        std::cout << "-";
    }
    std::cout << "+" << std::endl;
}

//打印数据
void Table::printData(int row, std::vector<int> colMaxLength) {
    int length;

    for (int i = 0; i < colMaxLength.size(); ++i) {
        std::cout << "| ";

        if (row == -1) { //将表头视为第-1行
            std::cout << Column_Info[i].id;
            length = Column_Info[i].id.size();
        } else { //打印普通行数据
            std::cout << Column_Info[i].Columns[row];
            length = Column_Info[i].Columns[row].size();
        }
        
        for (int j = 0; j < colMaxLength[i] - length; ++j)
            std::cout << " ";
        std::cout << " ";
    }
    std::cout << "|" << std::endl;
};
int main()
{
    dothings();
}