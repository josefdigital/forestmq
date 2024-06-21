#FROM --platform=linux/amd64 ubuntu:22.04
FROM ubuntu:22.04

WORKDIR /forestmq

COPY . .

RUN apt-get update
RUN apt-get -y install libjansson-dev
RUN apt-get -y install libjansson-dev
RUN apt-get -y install libmicrohttpd-dev
RUN apt-get -y install liborcania-dev
RUN apt-get -y install libulfius-dev
RUN apt-get -y install cmake


RUN mkdir build
RUN cd build
RUN cmake . .
RUN make

