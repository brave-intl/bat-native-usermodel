FROM ubuntu:18.04

RUN apt-get update && apt-get install -y \
	build-essential \
	git \
	python \
	python-pip \
	wget \
	unzip \
	ninja-build \
	pkg-config

WORKDIR /usermodel

RUN git clone https://chromium.googlesource.com/chromium/buildtools
RUN git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
RUN PATH=$PATH:depot_tools python depot_tools/download_from_google_storage.py \
    --no_resume --platform=linux* --no_auth --bucket chromium-gn \
    -s buildtools/linux64/gn.sha1

RUN git clone https://chromium.googlesource.com/chromium/src/build
RUN python build/linux/sysroot_scripts/install-sysroot.py --arch=amd64
ENV PATH=$PATH:/usermodel/depot_tools/

RUN git clone https://chromium.googlesource.com/chromium/src/build_overrides
RUN git clone https://chromium.googlesource.com/chromium/src/tools && ./tools/clang/scripts/update.py

RUN mkdir /usermodel/src

RUN ln -s /usermodel/buildtools /usermodel/src/buildtools
RUN ln -s /usermodel/build_overrides /usermodel/src/build_overrides
RUN ln -s /usermodel/build /usermodel/src/build
RUN ln -s /usermodel/tools /usermodel/src/tools
RUN ln -s /usermodel/third_party /usermodel/src/third_party

RUN git clone https://github.com/llvm-mirror/libcxx.git /usermodel/src/buildtools/third_party/libc++/trunk
RUN git clone https://github.com/llvm-mirror/libcxxabi.git /usermodel/src/buildtools/third_party/libc++abi/trunk

ADD . /usermodel/src
RUN cd src && git submodule init && git submodule update
RUN cd src && cp gclient_args.gni build/config/.
RUN echo buildconfig = \"//build/config/BUILDCONFIG.gn\" > /usermodel/src/.gn

RUN cp src/gclient_args.gni /usermodel/src/build/config
RUN cd src && gn gen out/default && ninja -C out/default && ./out/default/test

CMD bash