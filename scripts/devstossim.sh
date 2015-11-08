#!/usr/bin/bash

# DEVS-TOSSIM base directory
DEVSBASE=/opt/devs/src

# Import TinyOS environment variables
# The script should export the following environment variables:
#     export TOSROOT=/usr/src/tinyos
#     export TOSDIR=$TOSROOT/tos
#     export CLASSPATH=$CLASSPATH:$TOSROOT/support/sdk/java/tinyos.jar
#     export MAKERULES=$TOSROOT/support/make/Makerules
source  /etc/profile.d/tinyos.sh 

export DEVSBASE
export TOSMAKE_PATH=$DEVSBASE/devstossim/make/

