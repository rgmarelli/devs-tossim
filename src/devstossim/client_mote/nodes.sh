#!/bin/bash
#
# Copyright (c) 2013 Ricardo Guido Marelli
# All rights reserved.
#

SERVER="localhost:5001"
DUTYCYCLE="1.0"

if [ "$#" -lt 3 ]; then
    echo "Usage: $0 [BINARY] [NODES] [DEEP] [[X STEP]] [[Y STEP]]"
    exit 1
fi


BINARY=$1
NODES=$2
DEEP=$3

STEPX=50 # 
STEPY=30 #
if [ "$#" -gt 3 ]; then
    STEPX=$4
fi

if [ "$#" -gt 4 ]; then
    STEPY=$5
fi

NODES_PER_LEVEL=$(($NODES / $DEEP))

if [ ! -f $BINARY ]; then
    echo "File $BINARY does not exists"
    exit 1 
fi

echo "BINARY: $BINARY. Nodes: $NODES, DEEP: $DEEP, NODES PER LEVEL: $NODES_PER_LEVEL, X STEP: $STEPX, Y STEP: $STEPY"


printf "Y\\X\t"
for h in $(seq 1 $NODES_PER_LEVEL)
do
    X=$(($h * $STEPX))
    printf "%3d\t" "$X"
done
printf "\n"

COUNT=0
for i in $(seq 1 $DEEP)
do
    Y=$(($i * $STEPY))
    printf "%3d\t" "$Y"
    for j in $(seq 1 $NODES_PER_LEVEL)
    do
        COUNT=$(($COUNT + 1))
        X=$(($j * $STEPX))
        SENSORVAL=$(( ( RANDOM % 70000 )  + 1000 ))
        $BINARY $SERVER node$COUNT $COUNT $DUTYCYCLE $X,$Y,0 0 $SENSORVAL &
        printf "%3d\t" "$COUNT"
    done
    printf "\n";
done
