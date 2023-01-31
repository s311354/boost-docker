#!/bin/bash

# Release all images



for VERSION in $(cat boost-versions.txt)
do
    echo "Building $VERSION"
    ./build-docker.sh ${VERSION} 4 
    ./push-dockerhub.sh ${VERSION}
    docker rmi "louissrliu/boost:${VERSION}"
done
