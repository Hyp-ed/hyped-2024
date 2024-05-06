FROM ubuntu:latest
ARG DEBIAN_FRONTEND=noninteractive

# Install dependencies
RUN apt-get update && \
    apt-get install -y git clang clang-format clang-tidy libboost-all-dev libeigen3-dev rapidjson-dev build-essential gcc make cmake libssl-dev libncurses5-dev libncursesw5-dev sudo 

# Install Paho C++
WORKDIR /home
RUN git clone --recurse-submodules https://github.com/eclipse/paho.mqtt.cpp.git
WORKDIR /home/paho.mqtt.cpp
RUN git checkout v1.3.2
RUN cmake -Bbuild -H. -DPAHO_WITH_MQTT_C=ON -DPAHO_BUILD_STATIC=ON -DPAHO_BUILD_DOCUMENTATION=OFF -DPAHO_BUILD_SAMPLES=OFF
RUN sudo cmake --build build/ --target install
RUN sudo ldconfig

WORKDIR /home/hyped_entrypoint
COPY entry.sh ./

ENTRYPOINT [ "/home/hyped_entrypoint/entry.sh" ]
CMD ["bash"]
