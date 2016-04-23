/* 
 * devsCPP - a DEVS C++ library
 * Copyright (c) 2013 Ricardo Guido Marelli
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
