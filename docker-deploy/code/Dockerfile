FROM ubuntu:18.04
RUN apt-get update && apt-get install -y g++
RUN apt-get update && apt-get install -y make 
RUN mkdir -p /work
RUN mkdir -p /var/log/erss
WORKDIR /work
ADD . /work/

ENTRYPOINT ["./run.sh"]
