#include <string.h>
#include <map>
#include <iostream>
#define SELECT 0
#define INSERT 1
#define DELETE 2
#define CREATE 3
#define ALTER 4
#define DROP 5
#define USE 6
#define STAGE_ONE 1
#define STAGE_TWO 2
#define STAGE_THREE 3
const std::map<std::string,int> LocalMap={{"SELECT",SELECT},{"INSERT",INSERT},{"DELETE",DELETE},{"CREATE",CREATE},{"ALTER",ALTER},{"DROP",DROP},{"USE",USE}};
const std::map<std::string,int> GetNumTable={{"NOTNULL",3},{"PRIMARYKEY",1},{"FOREIGNKEY",2},{"",0},{"DEFAULT",0}};