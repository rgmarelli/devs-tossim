---
layout: post
title:  "Radio model"
date:   2016-04-23 08:00:00 -0300
---

![The DEVS-TOSSIM user interface]({{ site.baseurl }}/images/friis.png)

The radio model uses the [Friis transmission equation](https://en.wikipedia.org/wiki/Friis_transmission_equation) to model communication between sensor nodes; therefore the communication range is limited.

Communications interference is simulated for each transceiver depending on their specific state. The reception of a radio message will be successful if no other message is detected by the transceiver.

It's important to point out that radio communication, reception and transmission of message, is not instanteneous but depends on bandwidth and message's length.
