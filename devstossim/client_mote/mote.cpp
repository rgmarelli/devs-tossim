/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 * Ejemplo de Mote cliente.
 */

#include <interface/MoteManager.h>
#include <TinyOSProcessor.h>
#include <model/Mote.h>

#include <CoupledCompositeSimulator.h>
#include <RemoteSimulatorConnector.h>
#include <TCPMessageLayer.h>

#include <model/Connected_Serial.h>
#include <model/RadioParams.h>

#include <model/DataSensor.h>
#include <model/OscillationSensor.h>

#include <sstream>

struct Args {
    Args() : mote_id(0), duty_cycle(1), serial_port(0), enable_serial(false), sensor_value(0), data_sensor_port(0), enable_data_sensor(false)
    {}
    
    std::string server_address;
    std::string mote_name;
    unsigned long mote_id;
    float duty_cycle;
    RadioParams radioparams;
    unsigned int serial_port;
    bool enable_serial;
    unsigned int sensor_value;
    unsigned int data_sensor_port;
    bool enable_data_sensor;

    void dump() {
        DEVS::Log::write(LOG_INFO,"DEVS", 
                         "[%s] moteid=%d, server_addres=%s, duty_cycle=%f, position=%f,%f,%f, enable_serial=%s (%d), enable_data_sensor=%s (%d)",
                         mote_name.c_str(), mote_id, server_address.c_str(), duty_cycle, radioparams.x, radioparams.y, radioparams.z,
                         enable_serial ? "true" : "false", serial_port,
                         enable_data_sensor ? "true" : "false", 
                         enable_data_sensor ? data_sensor_port : sensor_value);
    }
};

Args parse_args( int argc, char *argv[] ) {
    Args args;

    if( argc < 4 ) {
        printf("Usage: %s [server] [name] [id] [[duty_cycle=1]] [[x,y,z]] [[serial port=0]] [[sensor value=random]] [[sensor port]]\n",argv[0]);
        exit(1);
    }
    
    args.server_address = argv[1];

    args.mote_name = argv[2];
    args.mote_id = atol(argv[3]);

    args.duty_cycle = atof(argv[4]);

    if( argc > 5 ) {
        std::istringstream ss(argv[5]);
        std::string val;

        std::getline(ss, val, ',');
        args.radioparams.x=atof(val.c_str());
        std::getline(ss, val, ',');
        args.radioparams.y=atof(val.c_str());
        std::getline(ss, val, ',');
        args.radioparams.z=atof(val.c_str());
    }

    if( argc > 6 ) {
        args.serial_port = atol(argv[6]);
        if( args.serial_port > 0 ) {
            args.enable_serial = true;
        }
    }

    if( argc > 7 ) {
	    if( argc > 8 ) {
            args.data_sensor_port = atol(argv[8]);
            if( args.data_sensor_port > 0 ) {
                args.enable_data_sensor = true;
            }
	    }

        if( !args.enable_data_sensor ) {
            args.sensor_value = atol(argv[7]);
        }
    }
    return args;
}

int main(int argc, char *argv[])
{
    Args args = parse_args( argc, argv );
    args.dump();

    Connected_Serial::Collector_Type serial_data_collector;
    DEVS::DataCollector<> sensor_data_collector;

    Serial *serial = NULL;
    Sensor *sensor = NULL;

    if( args.enable_serial > 0 ) {
        if( !serial_data_collector.start( args.serial_port ) ) {
            DEVS::Log::write(LOG_ERR,"DEVS", "[%s] Unable to start serial forwarder on port %d", args.mote_name.c_str(), args.serial_port);
            exit(1);
        }
        serial = new Connected_Serial( args.mote_name + ".Serial", &serial_data_collector );
    }

    if( args.enable_data_sensor ) {
        if( !sensor_data_collector.start( args.data_sensor_port ) ) {
            DEVS::Log::write(LOG_ERR,"DEVS", "[%s] Unable to start data sensor on port %d", args.mote_name.c_str(), args.data_sensor_port);
            exit(1);
        }
        sensor = new DataSensor( args.mote_name + ".Sensor",&sensor_data_collector );
    }
    else {
        sensor = new OscillationSensor( args.mote_name + ".Sensor", false, args.sensor_value );
    }

    Mote mote( args.mote_name,
               MoteManager::instance()->add(new TinyOSProcessor(args.mote_name+".tinyproc",args.mote_id)),
               args.duty_cycle,
               sensor,
               serial);

    DEVS::CoupledCompositeSimulator motesim( &mote );

    DEVS::Buffer properties;
    properties.putContent<RadioParams>( args.radioparams);

    DEVS::RemoteSimulatorConnector<DEVS::TCPMessageLayer> remote_simulator( &motesim, properties );
    remote_simulator.connect( args.server_address );
    while(1) {
        if( !remote_simulator.doProtocol() ) break;
    }

    serial_data_collector.stop();
    sensor_data_collector.stop();
    printf("bye %s\n", args.mote_name.c_str());
}


