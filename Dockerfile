#FROM --platform=linux/amd64 ubuntu:22.04
FROM ubuntu:22.04

WORKDIR /forestmq

COPY . .

ENV APPLE = 0
ENV UNIX = 1

RUN apt update
RUN apt install -y libulfius-dev uwsc
RUN apt install -y libmicrohttpd-dev
RUN apt install -y libjansson-dev
RUN apt install -y libcurl4-gnutls-dev
RUN apt install -y libgnutls28-dev
RUN apt install -y libgcrypt20-dev
RUN apt install -y libsystemd-dev
RUN apt install -y pkg-config
RUN apt install -y cmake

#RUN mkdir build
RUN cmake -S . -B build
RUN make -C build

EXPOSE 8005

RUN chmod +x build/forest_mq

CMD ["./build/forest_mq"]
