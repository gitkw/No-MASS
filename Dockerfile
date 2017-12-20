FROM ubuntu:14.04

RUN sudo apt-get update
RUN sudo apt-get -y install build-essential
RUN sudo apt-get -y install cmake

COPY ./FMU /app/FMU
COPY ./rapidxml /app/rapidxml

RUN mkdir /app/build
RUN cd /app/build; cmake -DTests=on -DCMAKE_BUILD_TYPE=Debug ../FMU/; make


WORKDIR /app/build

ENTRYPOINT ["tests/runUnitTests"]

CMD []
