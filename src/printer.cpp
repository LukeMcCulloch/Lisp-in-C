#include "../include/printer.h"
#include "../include/basic.h"


std::string pr_str(Value *value) {
    debugprint("pr_string");
    if (value) { //todo: fix pointer stuff, this is a hack to make null input work now that we use value (and derived value types) and not strings directly 
        return value->inspect();
    } else {
        return "";
    }
}