/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */
#ifndef DEVS_CPP_COUPLED_COMPOSITE_MODEL__
#define DEVS_CPP_COUPLED_COMPOSITE_MODEL__

#include "CoupledModel.h"
#include "AtomicModel.h"

namespace DEVS {

class CoupledCompositeModel : public CoupledModel
{
public:
    CoupledCompositeModel( const std::string &name ) : CoupledModel(name)
    {}

    virtual void add( Model *model ) {
        CoupledModel::add( model->name() );
        models_.push_back( model );
    }

    void autoCoupleModels( Model* model1, Model* model2 ) {
        this->autoCouplePorts( model1->outputPorts(), model2->inputPorts() );
        this->autoCouplePorts( model2->outputPorts(), model1->inputPorts() );
    }

    void autoCouplePorts( const PortCollection &outPorts, const PortCollection &inPorts ) {
        for( PortCollection::const_iterator it = outPorts.begin(); it != outPorts.begin(); it++ ) {
            if( inPorts.hasPort( *it ) ) {
                this->addCoupling( *it, inPorts.getPort( *it ) );
            }
        }
    }

    const std::list< Model* > &models() { return this->models_; }

    virtual ~CoupledCompositeModel() {
        for( std::list<Model*>::iterator it = models_.begin(); it != models_.end(); it++ ) {
            //cleanup
            delete(*it);
        }
    }

private:
    std::list< Model* > models_;

}; //CoupledCompositeModel

} // DEVS

#endif

