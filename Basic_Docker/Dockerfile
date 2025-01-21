FROM ubuntu:20.04

RUN apt-get update\
    && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends tzdata\
    && apt-get install -y gcc\
                          binutils\
                          bzip2\
                          flex\
                          git\
                          g++\
                          time\
                          util-linux\
                          gawk\
                          gzip\
                          help2man\
                          intltool\
                          libelf-dev\
                          zlib1g-dev\
                          make\
                          libncurses-dev\
                          libssl-dev\
                          patch\
                          perl-modules\
                          libthread-queue-any-perl\
                          python2-dev\
                          python3-dev\
                          swig\
                          unzip\
                          wget\
                          gettext\
                          xsltproc\
                          zlib1g-dev\
                          libboost-dev\
                          openjdk-11-jdk-headless\
                          libxml-parser-perl\
                          zip\
                          libusb-dev\
                          bin86\
                          bcc\
                          sharutils\
                          gcc-multilib\
                          b43-fwcutter\
    && apt-get clean 

WORKDIR /app