## boost-docker ## 
[![Docker Image CI](https://github.com/s311354/boost-docker/actions/workflows/docker-image.yml/badge.svg)](https://github.com/s311354/boost-docker/actions/workflows/docker-image.yml)

The Dockerfile that setups Ubuntu 16.04 xenial with boost libraries (header only). The C++ Boost libraries are installed under `/usr/local/include/boost/`.

### To build the image with Boost version 1.80.0  ###
```bash
./build-docker.sh 1.80 4
```

### To run and interact with abobe built image  ###
```bash
./run-docker.sh 1.80.0
```

### To build and run boost examples  ###

Inside the Boost Docker container, follow the [README](/examples/README.md) in the [examples](/examples/).

### Boost version  ###

see the `boost-versions.txt` file.

