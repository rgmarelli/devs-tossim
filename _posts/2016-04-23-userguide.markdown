---
layout: post
title:  "User guide"
date:   2016-04-23 04:00:00 -0300
---

* [Installation and directory structure](#installation-and-directory-structure)
* [Compiling TinyOS Applications With DEVS-TOSSIM](#compiling-tinyos-applications-with-devs-tossim)
* [Compiling the simulation server](#compiling-the-simulation-server)
* [The user interface of DEVS-TOSSIM](#the-user-interface-of-devs-tossim)
* [Running a simulation with DEVS-TOSSIM](#running-a-simulation-with-devs-tossim)

## Installation and directory structure

The simulation framework DEVS-TOSSIM was developed and tested with TinyOS 2.1.2 and Linux. The DEVS user interface requires the _readline_ library and its development files (headers).

The installation of the framework consists in the copy of the source code to a user's directory. No additional steps are required.

The framework is composed of two main directories: devscpp y devstossim.

The implementation of DEVS can be found on the directory: _devscpp_. This implementation is generic and can be used independently of the models of WSN motes (see: [devscpp](https://github.com/rgmarelli/devscpp)).

The directory _devstossim_ contains all the models for simulating wireless sensor networks and basic client and server applications. This directory has the following subdirectories:

* __client_mote__: it's a basic client application that is compiled with TinyOS applications.

* __interface__: it contains the interface of the simulator with TinyOS and the implementation of the model TinyOSProcessor.

* __make__: it contains the Makefiles that allow the integration of the simulation framework with the TinyOS build system. This makes possible to compile TinyOS application for the simulator.
    
* __model__: it contains the implementation of the DEVS models. These classes model a WSN mote and can be extended to incorporate additional behavior.
 
* __platforms__: this directory contains the TinyOS counterpart of the simulation framework.

* __server__: is a basic server program that executes the RadioMedium coupled model in the user interface of DEVS-TOSSIM. Client motes (TinyOS applications) connect to this server.

## Compiling TinyOS applications with DEVS-TOSSIM

To compile TinyOS applications with DEVS-TOSSIM is necessary to export the environment variables: *DEVSBASE* and *TOSMAKE_PATH*. Assuming that DEVS-TOSSIM source code is in the directory */opt/devs*:

    export DEVSBASE=/opt/devs/src
    export TOSMAKE_PATH=$DEVSBASE/devstossim/make/

DEVS-TOSSIM implements a TinyOS platform called "*devs*". To compile an application with the simulator, navigate to the TinyOS application directory and execute: __make devs__. For example:

    cd /opt/tinyos-2.1.2/apps/RadioCountToLeds/
    make devs

If compilation was successful a directory "*simbuild*" with a program "*client_mote*" has 
been generated and the following message is shown:

    *** Successfully built DEVS-TOSSIM (DEVS TinyOS Simulator).

## Compiling the simulation server

The server program executes the *RadioMedium* coupled model and the DEVS user interface.
To compile the program, you must execute:

    cd /opt/devs/devstossim/server
    make

This commands will generate a binary called "*server*".

## The user interface of DEVS-TOSSIM

The DEVS framework includes a basic user interface that is compiled with the root DEVS 
simulator (the higher level simulator in the hierarchy).

![The DEVS-TOSSIM user interface]({{ site.baseurl }}/images/devs-ui.png)

The interface provides the following commands:

* __dump__: dump the event queue to the log.

* __help__: this help.

* __info__: show some basic simulation information.

* __loop__: start an infinite simulation loop.
    
* __step [N]__: run N simulation steps.

* __stop__: stop the simulation.

* __time [T]__: expand or contract time by factor T. 0 = run as fast as possible.

* __quit__: exit the simulation.


The simulation uses the syslog's facility "USER" to log information. All logs are preceded
with the tag "DEVSTOSSIM" and the PID of the originating process.

## Running a simulation with DEVS-TOSSIM

To run a simulation with DEVS-TOSSIM, the server program must be executed as follows
(the argument specifies the TCP port in which the server listens):

    cd /opt/devs/devstossim/server
    ./server 5001

Then, the TinyOS application (that was compiled with the devs platform) must be executed.
Every instance of this program represents an independent sensor node. The program receives
the following arguments:

* __server__: the name of the host and port in which the server is running. For example: localhost:5001.

* __name__: name of the sensor node.

* __id__: id of the sensor node in TinyOS.

* __duty-cycle (optional)__: duty-cycle that the transceiver will use as a number between 0 and 1. Default, is 1 (the transceiver won't be suspended).

* __x,y,z (optional)__: position of the node in the space.

* __serial port (optional)__: activate a serial forwarder in this port.

* __sensor value (optional)__: if a value is specified, the sensor's value will oscillate around this value; if not, the sensor value is random.

* __data sensor port (optional)__: activate a DataSensor that will expect information in the port specified.

For example, to add the following two nodes to the simulation:

> * A root node, with:
>   * id = 0
>   * duty-cycle = 50% (0.5)
>   * position = [0,0,0]
>   * serial forwarder enabled in port 9002
>   * sensor oscillating around 5000

> * A node called node1 with:
>   * id = 1
>   * duty-cycle = 50% (0.5)
>   * position = [0,0,50]
>   * serial forwarder disabled
>   * sensor oscillating around 7000

You must execute:

    cd /opt/tinyos-2.1.2/apps/RadioCountToLeds/simbuild
    ./mote localhost:5001 root 0 0.5 0,0,0 9002 5000 &
    ./mote localhost:5001 node1 1 0.5 0,0,50 0 7000 &

Additionally, a bash script called "nodes.sh" (src/devstossim/client_mote) can be used to generate a grid of nodes.

