FROM ubuntu:16.04
ENV OPENWRT_SDK_NAME OpenWrt-SDK-15.05.1-ar71xx-generic_gcc-4.8-linaro_uClibc-0.9.33.2.Linux-x86_64

RUN apt-get update -qq && apt-get install -y --no-install-recommends git subversion make cmake gcc pkg-config build-essential libssl-dev libncurses5-dev unzip gawk wget python file openssh-server libssl-dev libffi-dev qt5-default && \
	apt-get install -y arduino arduino-core gcc-avr binutils-avr avr-libc avrdude npm && \
	apt-get clean autoclean && \
	apt-get autoremove -y && \
	rm -rf /var/lib/apt /var/lib/dpkg /var/lib/cache /var/lib/log && \
	npm install -g ipk-builder

RUN useradd -s /bin/bash -m -d /home/user -G ssh -puser123 user && usermod -a -G ssh user && echo 'user:user123' | chpasswd && \
	mkdir /var/run/sshd && mkdir -p /opt && su user

RUN cd /opt && wget -nv https://downloads.openwrt.org/chaos_calmer/15.05.1/ar71xx/generic/$OPENWRT_SDK_NAME.tar.bz2 && \
	tar xfj $OPENWRT_SDK_NAME.tar.bz2 $OPENWRT_SDK_NAME/staging_dir/target-mips_34kc_uClibc-0.9.33.2 --strip-components=2 && \
	tar xfj $OPENWRT_SDK_NAME.tar.bz2 $OPENWRT_SDK_NAME/staging_dir/toolchain-mips_34kc_gcc-4.8-linaro_uClibc-0.9.33.2 --strip-components=2 && \
	rm $OPENWRT_SDK_NAME.tar.bz2

VOLUME /opt
EXPOSE 22
CMD ["/usr/sbin/sshd", "-D"]
