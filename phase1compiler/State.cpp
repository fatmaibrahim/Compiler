#include "State.h"
#include <iostream>
#include <string>

using namespace std;

class State {
    int  id;
    State *next;

public:
    node (void *v) {info = v; next = 0; }
    void put_next (node *n) {next = n;}
    node *get_next ( ) {return next;}
    void *get_info ( ) {return info;}
};
