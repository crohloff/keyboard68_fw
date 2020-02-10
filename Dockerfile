FROM zephyrprojectrtos/ci:v0.11.9

ARG CMAKE_VERSION=3.16.2

ARG ZSDK_VERSION=0.11.4

# zip: release upload
RUN apt-get update && apt-get install --no-install-recommends -y \
	zip

# Update west, original image ships with west 0.6.x
RUN pip3 install -U west

# Newer version of cmake than original image
RUN curl -L --remote-name-all https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-Linux-x86_64.sh && \
	chmod +x cmake-${CMAKE_VERSION}-Linux-x86_64.sh && \
	./cmake-${CMAKE_VERSION}-Linux-x86_64.sh --skip-license --prefix=/usr/local && \
	rm -f ./cmake-${CMAKE_VERSION}-Linux-x86_64.sh

RUN pip3 install -r https://raw.githubusercontent.com/zephyrproject-rtos/zephyr/v2.1.0/scripts/requirements.txt

# original image installs the sdk but does not set the env variables.
ENV ZEPHYR_TOOLCHAIN_VARIANT=zephyr
ENV ZEPHYR_SDK_INSTALL_DIR=/opt/sdk/zephyr-sdk-${ZSDK_VERSION}

ENV CCACHE_DISABLE=1
