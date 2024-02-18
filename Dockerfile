FROM ubuntu:latest
ARG DEBIAN_FRONTEND=noninteractive

# Install dependencies
RUN apt-get update && \
    apt-get install -y git clang clang-format libboost-all-dev libeigen3-dev rapidjson-dev build-essential gcc make cmake libssl-dev libncurses5-dev libncursesw5-dev sudo wget tar
    
# Install Paho C++
WORKDIR /home
RUN git clone --recurse-submodules https://github.com/eclipse/paho.mqtt.cpp.git
WORKDIR /home/paho.mqtt.cpp
RUN git checkout v1.3.2
RUN cmake -Bbuild -H. -DPAHO_WITH_MQTT_C=ON -DPAHO_BUILD_STATIC=ON -DPAHO_BUILD_DOCUMENTATION=OFF -DPAHO_BUILD_SAMPLES=OFF
RUN sudo cmake --build build/ --target install
RUN sudo ldconfig

# Set up Cross Compile Toolchain
WORKDIR /home
RUN mkdir -p /home/opt
# TODO figure out which cc we need
RUN wget -O- https://github.com/tttapa/docker-arm-cross-toolchain/releases/latest/download/x-tools-aarch64-rpi3-linux-gnu.tar.xz | tar xJ -C /home/opt
ENV PATH="$PATH:/home/opt/x-tools/aarch64-rpi3-linux-gnu/bin" 

WORKDIR /home/hyped_entrypoint
COPY entry.sh ./

ENTRYPOINT [ "/home/hyped_entrypoint/entry.sh" ]
CMD ["bash"]
