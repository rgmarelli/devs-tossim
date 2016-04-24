---
layout: post
title:  "Debugging"
date:   2016-04-23 10:00:00 -0300
---

The simulation uses the syslog’s facility “USER” to log information. All logs are preceded with the tag “DEVSTOSSIM” and the PID of the originating process.

Each node runs on its own independent process. So, debugging can be performed attaching *gdb* to each node. 

Nodes can be killed and can be added while the simulation is running; this enables the simulation of faults on specific nodes, reloading a node to change its properties, etc.


