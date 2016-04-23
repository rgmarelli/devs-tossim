# devsCPP

Copyright (c) 2015 Ricardo Guido Marelli
All rights reserved.

---------------------------------------

## Development guide


### 1. Introduction

This guide explains how to use the DEVS framework and generate new DEVS models. The DEVS framework is generic and can be used to create simulations that have no relation with wireless sensor networks.

---------------------------------------

### 2. Creating new DEVS atomic models

To create a new DEVS atomic model you must inherit from the abstract class: *AtomicModel*. This class defines the following three abstract methods that must be implemented on derived classes:

##### virtual void internalTransition()

    DEVS internal transition.

##### virtual void externalTransition(DEVS::ExternalMessage* message)

    DEVS external transition. It receives the external message as a parameter. The memory allocated 
    for this pointer is automatically released by the framework.

##### virtual DEVS::OutputMessage* outputFunction()

    This function is called to generate an output message that depends on the state of the model. 
    To return an output message, you must allocate memory that will be released by the framework. 
    If you do not want to return an output message, this function must return *NULL*.


On the other hand, the state's duration function has a default implementation that returns the value of the state variable "*sigma*".

##### virtual TIME timeAdvanceFunction()

    The state variable "sigma" can be assigned invoking the method setSigma(). 

    The method *timeAdvanceFunction()* can be overloaded to provide a different implementation.

    The class TIME represents time in DEVS and allows passivating a model with the constant: 
    DEVS::TIME::infinity(). It has nanosecond precision.


To control the phases or states of models, the following methods are provided:

##### void registerPhase(std::string phase)

    This method registers a new phase. All phases of a model must be registered in its constructor.

##### std::string phase()

    Returns the current phase of the model.

##### setPhase(std::string phase)

    It's used to change the phase of a model.

##### bool phaseIs(std::string phase)

    It's used to check if a model is in a specific phase.


Finally, when constructing an atomic model, you must register its input and output ports. Ports are accessible with the methods: *inputPorts()* and *outputPorts()*. For example:

    this->outputPorts().add( DEVS::Port( this-> name(),
                                       "TimerFire") );
                                       
    this->inputPorts().add( DEVS::Port(this->name(),
                                       "TimerStart"));


---------------------------------------
 
### 3. Creating new DEVS coupled models

The class CoupledModel is used to generate new coupled models. This class can be used directly or derived to define the coupled model in the constructor or to overload the *output translation function*. The definition of a coupled model implies:
  
* __Registering input and output ports.__

      This is done just as for the atomic models.


* __Registering the models that compound the coupled model.__

      Child models can be atomic models or other coupled models. These models are registered using the model's name which implies that names must be unique in the context of a coupled model.

        DEVS::CoupledModel coupled ( "aCoupledModel" );
        coupled.add( "model1" );
        coupled.add( "model2" );


* __Registering couplings.__

      Couplings can be established between ports of the child models or with the ports of the coupled model. One port can be coupled to multiple ports. Examples:

        coupled.addCoupling( model1.outputPorts().getPortByName( "Out"),
                             model2.inputPorts().getPortByName( "In") );
                             
        coupled.addCoupling( DEVS::Port("model1","Out"), DEVS::Port("model2","In"));
        
        coupled.addCoupling( DEVS::Port("model1","Out), 
                             coupled.outputPorts().getPortByName("Out") );


* __Determining if a specific output translation function is required.__

      The frameworks provides a generic output translation function that can be overloaded to implement model specific behavior:

        virtual ExternalMessage* translate( OutputMessage *message, Port &dstPort )
        
           The method is called once for every dstPort port that is coupled to the port specified in the message. 
           The function can return NULL to suppress output for the port. When returning a message, memory must 
           be allocated; this memory is released automatically by the framework.

---------------------------------------

### 4. Instantiation of simulators

To run a simulation you must instantiate the corresponding simulators. Every DEVS model must be associated to a simulator that will be in charge of executing that model. For example:

    SimpleAtomic atomic_model1( "model1" );
    SimpleAtomic atomic_model2( "model2" );
    DEVS::CoupledModel coupled( "aCoupledModel" );
       
    DEVS::AtomicSimulator simulator1( &atomic_model1);
    DEVS::AtomicSimulator simulator2( &atomic_model2);
    DEVS::CoupledSimulator coupled_simulator( &coupled );

The preceding source code will instantiate two atomic models, a coupled model, and their simulators.

    coupled_simulator.addSimulator( &simulator1 );
    coupled_simulator.addSimulator( &simulator2 );

Simulators of the child models (model1, model2) must be registered with the simulator of the coupled model (aCoupledModel).

    for( int i=0; i < 20; i++ ) {
        coupled_simulator.simulate();
    }

Once all simulators are registered, you can run 20 simulation steps.


Alternatively, the classes CoupledCompositeModel and CoupledCompositeSimulator can be used to generate an hierarchy of simulators:

    DEVS::CoupledCompositeModel coupled( "aCoupledModel" );
     
    SimpleModel* model1 = new SimpleModel( "model1" );
    SimpleModel* model2 = new SimpleModel( "model2" );
     
    coupled.add( model1 );
    coupled.add( model2 );

Note that when using CoupledCompositeModel, the child model instances are directly registered with the coupled model.

    DEVS::CoupledCompositeSimulator coupled_simulator(&coupled);

On instantiation, CoupledCompositeSimulator will instantiate all the simulators for the child models.

---------------------------------------
