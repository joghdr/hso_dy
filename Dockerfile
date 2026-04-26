FROM ubuntu:24.04 AS build-compint
# 1. Base
RUN apt-get update && apt-get install -y --no-install-recommends \
gcc-14 \
g++-14 \
libstdc++-14-dev \
python3 \
python3-dev && \
rm -rf /var/lib/apt/lists/*

# 5. System Linkage (Alternatives)
RUN update-alternatives --remove-all gcc || true && \
update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-14 100 \
--slave /usr/bin/g++ g++ /usr/bin/g++-14 \
--slave /usr/bin/gcov gcov /usr/bin/gcov-14 && \
update-alternatives --install /usr/bin/cc cc /usr/bin/gcc-14 100 && \
update-alternatives --install /usr/bin/c++ c++ /usr/bin/g++-14 100

RUN ln -sf /usr/bin/python3 /usr/bin/python


FROM build-compint AS build

RUN apt-get update && apt-get install -y --no-install-recommends \
ca-certificates \
wget \
gnupg \
lsb-release \
cmake \
git \
ninja-build \
pkg-config \
zlib1g-dev \
libncurses-dev && \
rm -rf /var/lib/apt/lists/*

# 2. Add LLVM 18 Repository
RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | gpg --dearmor -o /etc/apt/keyrings/llvm.gpg && \
echo "deb [signed-by=/etc/apt/keyrings/llvm.gpg] http://apt.llvm.org/noble/ llvm-toolchain-noble-18 main" > /etc/apt/sources.list.d/llvm.list

# 3. Install LLVM 18 Suite (Needed to build IWYU)
RUN apt-get update && apt-get install -y --no-install-recommends \
llvm-18-dev \
libclang-18-dev \
clang-18 && \
rm -rf /var/lib/apt/lists/*

# 4. Build IWYU from Source (Clang 18 Branch)
RUN git clone --branch clang_18 --depth 1 https://github.com/include-what-you-use/include-what-you-use.git /tmp/iwyu && \
mkdir -p /tmp/iwyu/build && cd /tmp/iwyu/build && \
cmake -G Ninja -DCMAKE_PREFIX_PATH=/usr/lib/llvm-18 .. && \
ninja install && \
rm -rf /tmp/iwyu


FROM build-compint AS hso-dy-dev
RUN apt-get update && apt-get install -y --no-install-recommends \
ca-certificates \
wget \
gnupg \
lsb-release \
cmake \
make \
git \
ninja-build \
pkg-config \
zlib1g-dev \
libncurses-dev \
libclang-cpp18 \
libllvm18 && \
rm -rf /var/lib/apt/lists/*

RUN ldconfig


FROM hso-dy-dev AS hso-dy-deps

RUN apt-get update && apt-get install -y --no-install-recommends \
libgsl27 \
libgsl-dev && \
rm -rf /var/lib/apt/lists/*

WORKDIR /tmp

#LHAPDF v6.5.4
RUN wget https://lhapdf.hepforge.org/downloads/?f=LHAPDF-6.5.4.tar.gz -O LHAPDF-6.5.4.tar.gz && \
tar xf LHAPDF-6.5.4.tar.gz && \
cd LHAPDF-6.5.4 && \
./configure --prefix=/usr/local --disable-python && \
make -j$(nproc) && \
make install && \
rm -rf LHAPDF-6.5.4.tar.gz LHAPDF-6.5.4

#cuba v4.2.2 integration (static) library
# make -j1 explicit, cuba does not build correctly with many cores
RUN wget https://feynarts.de/cuba/Cuba-4.2.2.tar.gz && \
tar xf Cuba-4.2.2.tar.gz && \
cd Cuba-4.2.2 && \
./configure --prefix=/usr/local && \
make lib -j1 && \
make install && \
rm  -rf Cuba-4.2.2.tar.gz  Cuba-4.2.2

#Minuit2 standalone 6.36.10
#hash: f4102aa5aecf9d00dae429695c67177fe57a3945
RUN mkdir root_repo && cd root_repo && \
git init && \
git remote add origin https://github.com/root-project/root.git && \
git fetch --depth 1 origin f4102aa5aecf9d00dae429695c67177fe57a3945 && \
git checkout FETCH_HEAD && \
cd math/minuit2 && mkdir build &&\
cmake -S . -B build  \
-DCMAKE_INSTALL_PREFIX=/usr/local \
-Dminuit2_standalone=ON \
-Dminuit2_omp=OFF \
-DBUILD_SHARED_LIBS=ON \
-DCMAKE_BUILD_TYPE=Release && \
cmake --build build -j$(nproc) && \
cmake --install build && \
cd /tmp && \
rm -rf root_repo

RUN ldconfig

##copy MMHT2014nlo68cl grids (collinear functions and other coefficients)
COPY assets/grids/ /usr/local/share/LHAPDF/

# ----- hso build and test-----

FROM hso-dy-deps AS hso-dy-build

WORKDIR /app

COPY . .

COPY --from=build /usr/local/bin/include-what-you-use /usr/local/bin
COPY --from=build /usr/local/bin/iwyu_tool.py /usr/local/bin
COPY --from=build /usr/local/share/include-what-you-use /usr/local/share/include-what-you-use
COPY --from=build /usr/lib/llvm-18/lib/clang/18/include /usr/lib/llvm-18/lib/clang/18/include
RUN ln -s /usr/lib/x86_64-linux-gnu/libLLVM.so.18.1 /usr/lib/x86_64-linux-gnu/libLLVM-18.so.18.1


RUN cmake --workflow --preset verify-release


# ----- hso light release image -----

FROM ubuntu:24.04 AS hso-dy


WORKDIR /app

RUN apt-get update && apt-get install -y \
libgsl27 \
libstdc++6 \
&& rm -rf /var/lib/apt/lists/*

COPY --from=hso-dy-deps   /usr/local/lib/libMinuit2.so      /usr/local/lib
COPY --from=hso-dy-deps   /usr/local/lib/libMinuit2Math.so  /usr/local/lib
COPY --from=hso-dy-deps   /usr/local/lib/libLHAPDF.so       /usr/local/lib
COPY --from=hso-dy-deps   /usr/local/share/LHAPDF           /usr/local/share
COPY --from=hso-dy-build /app/build/release/bin/HSODrellYanFitter /usr/local/bin/HSODrellYanFitter
COPY --from=hso-dy-build /app/assets/data   /app/assets/data

RUN chmod -R 755 /app/assets/data && ldconfig

ENV CUBACORES=5























