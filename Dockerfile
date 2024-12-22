#FROM --platform=linux/amd64 ubuntu:22.04
FROM ubuntu:22.04

WORKDIR /forestmq

COPY CMakeLists.txt .
COPY *.c .
COPY *.h .
COPY Dockerfile .

ENV APPLE=0
ENV UNIX=1

RUN apt update
RUN apt install -y libjansson-dev
RUN apt install -y libevent-dev
RUN apt install -y libsystemd-dev
RUN apt install -y pkg-config
RUN apt install -y cmake

#RUN mkdir build
RUN cmake -S . -B build
RUN make -C build

EXPOSE 8005

RUN chmod +x build/forest_mq

CMD ["./build/forest_mq"]
