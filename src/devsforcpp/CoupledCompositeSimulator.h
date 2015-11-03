/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
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


