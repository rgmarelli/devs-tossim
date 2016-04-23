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

