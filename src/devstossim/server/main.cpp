/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 * Servidor de ejemplo que corre el modelo RadioMedium
 */

#include <model/RadioMedium.h>

#include <CoupledSimulator.h>
#include <RemoteMessageInterface.h>
#include <RemoteSimulatorAcceptor.h>
#include <RemoteBinding.h>
#include <TCPMessageLayer.h>
#include <UI/RemoteSimulation.h>
#include <UI/CommandLine.h>

class Auto_Bind_Policy
{
public:
    bool bind( DEVS::CoupledSimulator *simulator, DEVS::CoupledModel *model, DEVS::Simulator *child_simulator, const DEVS::Buffer &properties ) {
        RadioMedium *radio = dynamic_cast<RadioMedium*>( model );
        RadioParams params = properties.content<RadioParams>();
        radio->register_radio( child_simulator->model_name(), params );
        return true;
    }
};

int main(int argc, char *argv[])
{
    std::string connection_description = ":5001";
    if( argc > 1 ) {
        connection_description = ":" + std::string(argv[1]);
    }

    DEVS::RemoteSimulatorAcceptor<DEVS::TCPMessageLayer> remote_simulator_server;
    remote_simulator_server.listen( connection_description );

    RadioMedium radio;
    DEVS::CoupledSimulator radiosim(&radio);

    DEVS::UI::RemoteSimulation<Auto_Bind_Policy> simulation( &radiosim, &radio );

    DEVS::RemoteBinding< DEVS::TCPMessageLayer, 
                         DEVS::UI::RemoteSimulation<Auto_Bind_Policy> > bindings( remote_simulator_server, simulation );
    bindings.run();

    DEVS::UI::CommandLine ui( &simulation );
    ui.run();

    bindings.remove_all();
    remote_simulator_server.close();
}

