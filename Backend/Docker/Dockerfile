FROM nginx:latest



RUN apt-get -y update \
    && apt-get -y upgrade

RUN apt-get install -y \
    openssh-server \
    g++ \
    cmake \
    git     \
    python3 \
    zip \
    curl \
    unzip \
    tar     \
    autoconf \
    automake \
    libtool \
    autoconf-archive \
    python3-dev \
    python3-pip \
    python3-setuptools

#installing the mongoc dependencies and driver
RUN apt-get install -y \
    pkg-config \
    libssl-dev \
    libsasl2-dev

RUN cd ~ \
&& git clone https://github.com/Microsoft/vcpkg.git \
&& cd vcpkg \
&& ./bootstrap-vcpkg.sh -disableMetrics \
&& ./vcpkg update

RUN cd ~/vcpkg \
&& ./vcpkg install mongo-cxx-driver     \
&& ./vcpkg integrate install 
 
RUN cd ~/vcpkg \
&& ./vcpkg install python3    \
&& ./vcpkg integrate install 

RUN cd ~/vcpkg \
&& ./vcpkg install boost    \
&& ./vcpkg integrate install 


#RUN ls \
#&& cd /shared/ \
#&& mkdir build \
#&& cd build \
#&& cmake .. -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake \
#&& make 