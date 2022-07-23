FROM ubuntu:20.04

RUN apt-get update && apt-get install -y tzdata

ARG HOME="/root"
# timezone setting
ENV TZ=Asia/Tokyo 

RUN apt-get update && apt-get install -y vim wget git dpkg-dev cmake g++ gcc binutils libx11-dev libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev tesseract-ocr libtesseract-dev libavcodec-dev libavformat-dev libswscale-dev libv4l-dev libgstreamer-plugins-base1.0-dev libgstreamer1.0-dev libgtk2.0-dev libgtk-3-dev libpng-dev libjpeg-dev libopenexr-dev libtiff-dev libwebp-dev unzip

RUN wget https://github.com/opencv/opencv/archive/3.4.10.zip 
RUN wget https://github.com/opencv/opencv_contrib/archive/3.4.10.zip 

RUN unzip -q 3.4.10.zip
RUN unzip -q 3.4.10.zip.1

RUN ln -s opencv-3.4.10 opencv
RUN ln -s opencv_contrib-3.4.10 opencv_contrib

RUN cd opencv && mkdir build 
WORKDIR opencv/build

RUN cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D OPENCV_EXTRA_MODULES_PATH=/opencv_contrib/modules ..
RUN make -j9
RUN make install
