FROM ubuntu:16.04

MAINTAINER Louis Liu <s041978@hotmail.com> 

WORKDIR /mnt/

ARG DEBIAN_FRONTEND=noninteractive

ARG CMAKE_VERSION=3.25.1

# Install package dependencies
RUN apt-get update &&\
    apt-get install -y --no-install-recommends software-properties-common build-essential \
            autoconf libtool pkg-config ca-certificates libssl-dev pkg-config \
            git wget vim man cppman \
            automake g++ gcc \
            gdb valgrind && \
    apt-get clean

# We pass the boost version as argument
ARG BOOST_VERSION
ARG NUM_JOBS

ENV NUM_JOBS=${NUM_JOBS}

# Set the LD_LIBRARY_PATH environment variable
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib/

# Install CMake
RUN cd /tmp && \
    wget https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}.tar.gz && \
    tar xzf cmake-${CMAKE_VERSION}.tar.gz && \
    cd cmake-${CMAKE_VERSION} && \
    ./bootstrap && \
    make -j${NUM_JOBS} && \
    make install && \
    rm -rf /tmp/*

ENV BOOST_VERSION=${BOOST_VERSION}
ENV BOOST_ROOT=/usr/include/boost

# Install Boost
RUN cd /tmp && \
    BOOST_VERSION_MOD=$(echo $BOOST_VERSION | tr . _) && \
    wget https://boostorg.jfrog.io/artifactory/main/release/${BOOST_VERSION}/source/boost_${BOOST_VERSION_MOD}.tar.bz2 && \
    tar --bzip2 -xf boost_${BOOST_VERSION_MOD}.tar.bz2 && \
    cd boost_${BOOST_VERSION_MOD} && \
    ./bootstrap.sh --prefix=/usr/local && \
    ./b2 --prefix=/usr/local install && \
    rm -rf /tmp/*

RUN echo ${BOOST_ROOT}
ENTRYPOINT /bin/bash
