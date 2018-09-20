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

RUN git clone https://github.com/llvm-mirror/libcxx.git /usermodel/buildtools/third_party/libc++/trunk
RUN git clone https://github.com/llvm-mirror/libcxxabi.git /usermodel/buildtools/third_party/libc++abi/trunk
RUN git clone https://github.com/catapult-project/catapult.git /usermodel/third_party/catapult

ADD . /usermodel
RUN git clone https://github.com/brave-intl/bat-native-rapidjson.git
RUN echo buildconfig = \"//build/config/BUILDCONFIG.gn\" > .gn

RUN printf "build_with_chromium = true\n" >> build/config/gclient_args.gni
RUN printf "is_docker = true\n" >> build/config/BUILDCONFIG.gn # little hack to allow testing

RUN gn gen out/default && ninja -C out/default

ENV DEPOT_TOOLS=/usermodel/depot_tools/

CMD out/default/test