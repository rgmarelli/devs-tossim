/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */
#ifndef DEVS_CPP_PORT__
#define DEVS_CPP_PORT__

#include <string>
#include <cstring>

namespace DEVS {

class Port
{
public:

    typedef unsigned char PortType;

    Port() : empty_(true), type_(0)
    {}

    // Type y Name son definidos por el standard DEVS
    // Agregamos model_name para asi poder identificar univocamente 
    // a este objeto en el contexto de un modelo acoplado
    Port(std::string model_name, std::string name, PortType type=1) : empty_(false), type_(type), 
                                                                      name_(name), model_name_(model_name)
    {
    }

    bool operator<(const Port& port) const {
      if(model_name_ < port.model_name()) {
          return true;
      }
      else if(model_name_ == port.model_name()) {
          if(name_ < port.name()) {
              return true;
          }
      }
      return false;
    }
 
    PortType type() const { return type_; } 
    const std::string& name() const { return name_; } 
    const std::string& model_name() const { return model_name_; }
    bool empty() const { return empty_; }
    bool nameIs (const std::string& name) const { 
        return name == name_;    
    }

    unsigned int serialize(char* buffer) const {
        unsigned int size = 1;
        buffer[0] = type_;
        std::memcpy( &buffer[ size ], name_.c_str(), name_.size() );
        size = size + name_.size();
        buffer[ size ] = '\0';
        size++;
        std::memcpy( &buffer[ size ], model_name_.c_str(), model_name_.size() );        
        size = size + model_name_.size();
        buffer[ size ] = '\0';
        size++;
        return size;
    }

    void fromBuffer( char* buffer, unsigned int size ) {
        type_ = buffer[0];
        name_ = &buffer[1];
        model_name_ = &buffer[ 2 + name_.size() ];
        empty_ = false;
    }

private:
    bool empty_;
    PortType type_;
    std::string name_;
    std::string model_name_;
}; // Port

} // DEVS

#endif
