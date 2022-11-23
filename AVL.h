#include <string>
#include <iostream>
typedef struct node
{
    int val=-1;
    std::string id={""};
    struct node *left=nullptr;
    struct node *right=nullptr;
    struct node *parent=nullptr;
    int height;
} node_t;