FROM ubuntu:16.04
ENV OPENWRT_SDK_NAME OpenWrt-SDK-15.05.1-ar71xx-generic_gcc-4.8-linaro_uClibc-0.9.33.2.Linux-x86_64
RUN apt-get update && apt-get install -y git make cmake gcc pkg-config build-essential libssl-dev libncurses5-dev unzip gawk wget python file openssh-server libssl-dev libffi-dev
RUN useradd -s /bin/bash -m -d /home/user -G ssh -puser123 user
RUN usermod -a -G ssh user
RUN echo 'user:user123' | chpasswd
RUN mkdir /var/run/sshd
RUN mkdir -p /opt
RUN su user
RUN cd /opt && wget https://downloads.openwrt.org/chaos_calmer/15.05.1/ar71xx/generic/$OPENWRT_SDK_NAME.tar.bz2
RUN cd /opt && tar xvfj $OPENWRT_SDK_NAME.tar.bz2 $OPENWRT_SDK_NAME/staging_dir/target-mips_34kc_uClibc-0.9.33.2 --strip-components=2
RUN cd /opt && tar xvfj $OPENWRT_SDK_NAME.tar.bz2 $OPENWRT_SDK_NAME/staging_dir/toolchain-mips_34kc_gcc-4.8-linaro_uClibc-0.9.33.2 --strip-components=2
VOLUME /opt
EXPOSE 22
CMD ["/usr/sbin/sshd", "-D"]
