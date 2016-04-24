# DEVS-TOSSIM: A DEVS framework for simulation of TinyOS wireless sensor networks

__DEVS-TOSSIM__ is a simulation framework, which implements the *DEVS formalism*, to test and simulate wireless sensor networks that use the *TinyOS* operating system. 

The simulator removes some limitations of the simulator "*TOSSIM*" of TinyOS, such as the inability to run nodes with different software and complexity in modeling diversity of node hardware and radio. However, the main difference is the application of the DEVS formalism to formally model all hardware components of a node and the radio medium, which allows testing the developed applications for the different types of nodes and its scaling. This framework also allows the replacement of the sensor component of a node, the addition of mobility, the implementation of a new radio model or modeling a complex sensing environment.

Additionally, the simulator provides a simple user interface to control the simulation. This interface allows the user to control time during the simulation, for example: execute the simulation in real-time or as fast as possible.

Finally, it should be noted, that the DEVS framework is a generic DEVS implementation in C++ that can be used to simulate systems not related to wireless sensor networks.

More information: [http://rgmarelli.github.io/devs-tossim/](http://rgmarelli.github.io/devs-tossim/)
