FROM ubuntu:18.04

RUN apt-get update && apt-get -y install \
    make \
    clang-8 \
    lld-8 \
    libc6-dev-i386 \
    python3
