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
#ifndef DEVS_CPP_COUPLED_COMPOSITE_SIMULATOR__
#define DEVS_CPP_COUPLED_COMPOSITE_SIMULATOR__

#include "CoupledCompositeModel.h"
#include "CoupledSimulator.h"
#include "AtomicSimulator.h"

namespace DEVS {

class CoupledCompositeSimulator : public CoupledSimulator
{
public:
    CoupledCompositeSimulator(CoupledCompositeModel *model) : CoupledSimulator(model)
    {
        for(std::list<Model*>::const_iterator it = model->models().begin(); it != model->models().end(); it++ ) {
            if( try_atomic( *it ) ) {
                
            }
            else if( try_coupled( *it ) ) {

            }
            else {

            }
        }
    };

    virtual ~CoupledCompositeSimulator() {
        for( std::list<Simulator*>::iterator it = simulators_.begin(); it != simulators_.end(); it++ ) {
            //cleanup
            delete(*it);
        }
    }

protected:
    bool try_atomic( Model *model ) {
        AtomicModel *atomic = dynamic_cast<AtomicModel*>( model );
        if( atomic != NULL ) {
            this->registerSimulator( new AtomicSimulator( atomic ) );
            return true;
        }
        return false;
    }

    bool try_coupled( Model *model ) {
        CoupledCompositeModel *coupled = dynamic_cast<CoupledCompositeModel*>( model );
        if( coupled != NULL ) {
            this->registerSimulator( new CoupledCompositeSimulator( coupled ) );
            return true;
        }
        return false;
    }

private:
    void registerSimulator( Simulator *simulator ) {
        this->addSimulator( simulator );
        this->simulators_.push_back( simulator );
    } 

private:
    std::list< Simulator* > simulators_;
}; //CoupledCompositeSimulator

} // DEVS

#endif


