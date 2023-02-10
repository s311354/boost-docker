#!/bin/bash

if [ $# -eq 0 ]
    then
        echo "Usage ./run-docker.sh BOOST_VERSION i.e., 1.64.0 examples"
        exit
fi

ORG="louissrliu"
BOOST_VERSION=$1
MOUNT=$2

docker run -it   -v $(pwd)/${MOUNT}:/mnt ${ORG}/boost:${BOOST_VERSION} bash
