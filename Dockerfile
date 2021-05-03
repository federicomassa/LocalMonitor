FROM ubuntu:18.04

COPY ./ /home

WORKDIR /home

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y cmake g++ \ 
    qt5-default libmagick++* libqt5multimedia5-plugins qml-module-qtmultimedia \
    qtmultimedia5-dev libopenshot-audio-dev libzmqpp-dev swig \ 
    libavcodec-dev libavdevice-dev libavfilter-dev libavformat-dev libavresample-dev libavutil-dev libswscale-dev \
    libopencv-dev libboost-dev libboost-test-dev doxygen libunittest++-dev && \
    rm -rf /var/lib/apt/lists/*

RUN mkdir build && cd build && cmake .. && make -j && cd .. && mkdir Output