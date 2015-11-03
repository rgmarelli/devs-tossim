/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */
#ifndef DEVS_CPP_UI_CONTROLLER__
#define DEVS_CPP_UI_CONTROLLER__

#include "Optional.h"
#include <string>
#include <vector>

namespace DEVS {
namespace UI {

class Controller
{
public:
    struct Command_Result {
        Command_Result() : err(false)
        {}

        Command_Result( std::string v, bool e = false ) : err(e), value(v)
        {}

        bool err;
        std::string value;
    };

    virtual Command_Result exec( std::string command ) = 0;
    virtual bool step() = 0;

    virtual std::string command_exit() { return "quit"; }

    virtual std::vector<std::string>* commands( std::vector<std::string> *v ) = 0;
};

} //namespace UI
} //namespace DEVS

#endif //DEVS_CPP_UI_CONTROLLER__
