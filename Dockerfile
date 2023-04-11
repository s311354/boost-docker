FROM ubuntu:20.04

MAINTAINER Louis Liu <s041978@hotmail.com> 

WORKDIR /mnt/

ARG DEBIAN_FRONTEND=noninteractive

ARG CMAKE_VERSION=3.25.1

# Install package dependencies
RUN apt-get update &&\
    apt-get install -y --no-install-recommends software-properties-common build-essential \
            autoconf libtool pkg-config ca-certificates libssl-dev pkg-config \
            git wget  vim man cppman \
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

# Update repositories and install software:
RUN apt-get update && apt-get -y install curl fzf ripgrep tree xclip python3 python3-pip nodejs npm libtool libtool-bin unzip gettext

# Define which Neovim COC extensions should be installed.
ARG COC='coc-css coc-eslint coc-html coc-json coc-sh coc-sql coc-tsserver coc-yaml'
# Lazygit variables
ARG LG='lazygit'
ARG LG_GITHUB='https://aus01.safelinks.protection.outlook.com/?url=https%3A%2F%2Fgithub.com%2Fjesseduffield%2Flazygit%2Freleases%2Fdownload%2Fv0.31.4%2Flazygit_0.31.4_Linux_x86_64.tar.gz&data=05%7C01%7C%7C77e27d2dae3e47496e3008db372e8607%7C84df9e7fe9f640afb435aaaaaaaaaaaa%7C1%7C0%7C638164445207743849%7CUnknown%7CTWFpbGZsb3d8eyJWIjoiMC4wLjAwMDAiLCJQIjoiV2luMzIiLCJBTiI6Ik1haWwiLCJXVCI6Mn0%3D%7C3000%7C%7C%7C&sdata=eioD9hTJEo5YlcGN0HPTM7EKVGDvZqXf3NWSGEzucJA%3D&reserved=0'
ARG LG_ARCHIVE='lazygit.tar.gz'

# Cooperate Neovim with Python 3.
RUN pip3 install pynvim

# Cooperate NodeJS with Neovim.
RUN npm i -g neovim

# Install Neovim from source.
RUN mkdir -p /root/TMP

RUN cd /root/TMP && git clone https://github.com/neovim/neovim.git neovim

RUN cd /root/TMP/neovim && git checkout stable && make -j4 && make CMAKE_BUILD_TYPE=RelWithDebInfo &&  make install

# Install Lazygit from binary
RUN cd /root/TMP && curl -L -o $LG_ARCHIVE $LG_GITHUB
RUN cd /root/TMP && tar xzvf $LG_ARCHIVE && mv $LG /usr/bin/

# Delete TMP directory
RUN rm -rf /root/TMP

# Create directory for Vim Plug.
RUN mkdir -p /root/.config/nvim/autoload

# Install Vim Plug.
RUN curl -fLo /root/.config/nvim/autoload/plug.vim --create-dirs https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim

# Create directory for Neovim configuration files.
RUN mkdir -p /root/.config/nvim

# Create directory for Neovim COC extensions.
RUN mkdir -p /root/.config/coc/extensions

# Install Neovim COC extensions.
RUN cd /root/.config/coc/extensions && npm install $COC --global --only=prod

# Create directory for Neovim plugged files.
RUN mkdir -p /root/.config/nvim/plugged

# Copy  `init.vim' for Neovim
COPY ./init.vim /root/.config/nvim/init.vim

# Install Neovim extensions.
RUN nvim --headless +PlugInstall +qall

RUN echo ${BOOST_ROOT}
ENTRYPOINT /bin/bash
