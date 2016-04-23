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
#ifndef DEVS_CPP_UI_REMOTE_SIMULATION__
#define DEVS_CPP_UI_REMOTE_SIMULATION__

#include "Simulation.h"
#include "../CoupledModel.h"

namespace DEVS {
namespace UI {

class Default_Bind_Policy
{
public:
    bool bind( DEVS::CoupledSimulator *simulator, DEVS::CoupledModel *model, DEVS::Simulator *child_simulator, const Buffer &properties ) {
        return model->hasModel( child_simulator->model_name() );
    }
};

template <class BIND_POLICY=Default_Bind_Policy>
class RemoteSimulation : public Simulation
{
public:

    RemoteSimulation( DEVS::CoupledSimulator *simulator, DEVS::CoupledModel *model ) : Simulation( simulator), model_(model)
    {
        pthread_mutex_init(&mutex_, NULL);
    }

    bool bind_simulator( Simulator* handler, const Buffer& properties ) {
        bool success = true;
        pthread_mutex_lock( &mutex_ );

        success = BIND_POLICY().bind( simulator_, model_, handler, properties );
        if( success ) {
            simulator_->addSimulator( handler );
        }
        pthread_mutex_unlock( &mutex_ );
        return success;
    }

    virtual bool step() {
        bool runnext=false;
        pthread_mutex_lock( &mutex_ );
        runnext = Simulation::step();
        pthread_mutex_unlock( &mutex_ );
        return runnext;
    }

protected:
    DEVS::CoupledModel *model_;
    pthread_mutex_t mutex_;
};

} //namespace UI
} //namespace DEVS

#endif //DEVS_CPP_UI_REMOTE_SIMULATION__

