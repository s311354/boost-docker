#!/bin/bash

if [ $# -eq 0 ]
    then
        echo "Usage ./run-docker.sh BOOST_VERSION i.e., 1.64.0"
        exit
fi

ORG="louissrliu"
BOOST_VERSION=$1

docker run -it   -v $(pwd)/examples:/mnt ${ORG}/boost:${BOOST_VERSION} bash
