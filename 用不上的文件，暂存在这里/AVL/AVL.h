#include <string>
#include <iostream>
#include "Crone.h"
typedef struct node
{
    int val=-1;
    std::vector<std::string> strs;
    struct node *left=nullptr;
    struct node *right=nullptr;
    struct node *parent=nullptr;
    int height;
} node_t;