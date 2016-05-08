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
 *
 * -----------------
 * This program is an example of use of the Simulation class
 */

#include "../include/CoupledModel.h"
#include "../include/AtomicModel.h"
#include "../include/AtomicSimulator.h"
#include "../include/CoupledSimulator.h"
#include "../include/UI/Simulation.h"
#include "../include/UI/CommandLine.h"

class MyAtomicModel : public DEVS::AtomicModel
{
public:
    MyAtomicModel() : DEVS::AtomicModel("MyAtomicModel")
    {
        setSigma(2000*DEVS::Time::mili_to_nano_sec);
    }

    virtual void internalTransition(){
        DEVS::Log::write(LOG_NOTICE,"MyAtomicModel","MyAtomicModel internal transition: %s",this->tn().to_string().c_str());        
        DEVS::Time::sleep_interval( 1000*DEVS::Time::mili_to_nano_sec );
    }

    virtual void externalTransition(DEVS::ExternalMessage* message){}

    virtual DEVS::OutputMessage* outputFunction() { 
        return NULL;
    }

};

class MyCoupledModel : public DEVS::CoupledModel
{
public:
    MyCoupledModel() : DEVS::CoupledModel("MyCoupledModel")
    {}
};

int main(int argc, char *argv[])
{
    MyCoupledModel coupled_model;
    MyAtomicModel atomic_model;

    coupled_model.add( atomic_model.name() );

    DEVS::AtomicSimulator atomic_sim( &atomic_model );
    DEVS::CoupledSimulator simulator( &coupled_model );
    simulator.addSimulator( &atomic_sim );

    DEVS::UI::Simulation simulation( &simulator );

    DEVS::UI::CommandLine ui( &simulation );
    ui.run();
    return 0;
}

