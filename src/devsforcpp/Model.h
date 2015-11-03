/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */
#ifndef DEVS_CPP_MODEL__
#define DEVS_CPP_MODEL__

#include "PortCollection.h"
#include <string>

namespace DEVS {

class Model
{
public:
    Model(const std::string name) : name_(name)
    {}

    virtual ~Model() {
    }

    PortCollection& inputPorts() { return inputPorts_; }
    PortCollection& outputPorts() { return outputPorts_; }

    const std::string& name() const { return name_; }

private:
    PortCollection inputPorts_;
    PortCollection outputPorts_;
    std::string name_;
}; // Model

} // DEVS

#endif
