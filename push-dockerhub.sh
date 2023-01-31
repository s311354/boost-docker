#!/bin/bash

if [ $# -eq 0 ]
    then
        echo "Usage ./run-docker.sh BOOST_VERSION i.e., 1.64.0"
        exit
fi

ORG="louissrliu"
BOOST_VERSION=$1

docker login
docker push ${ORG}/boost:${BOOST_VERSION}
