FROM ubuntu:latest
RUN apt-get update && \
    apt-get install -y git clang clang-format cmake libboost-all-dev libeigen3-dev rapidjson-dev

COPY entry.sh ./

ENTRYPOINT [ "/entry.sh" ]

CMD ["bash"]



