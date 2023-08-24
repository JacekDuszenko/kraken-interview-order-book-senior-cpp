FROM ubuntu:20.04
ADD . /home
RUN DEBIAN_FRONTEND="noninteractive" apt-get update && apt-get -y install tzdata

RUN apt-get update \
  && apt-get install -y build-essential \
      gcc \
      g++ \
      make \
      python3.10 \
      ninja-build \
      cmake \
      autoconf \
      automake \
      libtool \
      libpthread-stubs0-dev \
  && apt-get clean
