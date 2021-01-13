#include <stdio.h>
#include <stdlib.h>

//资源的结构表示
class Resource {
public:
    Resource(int rid, int amount);
    int getrid() {return rid;}
    int getamount() {return amount;}
    int x, y;
private:
    int rid;
    int amount;
};