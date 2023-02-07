#!/bin/bash

if [ $# -eq 0 ]
    then
        echo "Usage ./build-docker.sh BOOST_VERSION NUM_JOBS i.e., 1.80.0 4"
        exit
fi

ORG="louissrliu"
BOOST_VERSION=$1
NUM_JOBS=$2

docker buildx build . --build-arg BOOST_VERSION=${BOOST_VERSION} --build-arg NUM_JOBS=${NUM_JOBS} --tag=${ORG}/boost:${BOOST_VERSION} --platform=arm64,
