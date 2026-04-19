
#########################################
###### build with audit presets
#########################################

FROM debian:12-slim AS auditor

RUN apt-get update && apt-get install -y --no-install-recommends \
build-essential \
ca-certificates \
clang \
cmake \
gfortran \
git \
libboost-math-dev \
libgsl-dev \
llvm \
ninja-build \
pkg-config \
python3 \
python3-dev \
wget \
&& rm -rf /var/lib/apt/lists/*

# 'python' symlink so for LHAPDF build
RUN ln -s /usr/bin/python3 /usr/bin/python

#LHAPDF v6.5.4
WORKDIR /tmp
RUN wget https://lhapdf.hepforge.org/downloads/?f=LHAPDF-6.5.4.tar.gz -O LHAPDF-6.5.4.tar.gz && \
tar xf LHAPDF-6.5.4.tar.gz && \
cd LHAPDF-6.5.4 && \
./configure --prefix=/usr/local && \
make -j$(nproc) && \
make install && \
rm -rf LHAPDF-6.5.4.tar.gz LHAPDF-6.5.4

#Minuit2 standalone 6.36.10
#hash: f4102aa5aecf9d00dae429695c67177fe57a3945 (18-apr-2026)
WORKDIR /tmp
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
cmake --build build -j20 && \
cmake --install build && \
cd /tmp && \
rm -rf root_repo

#cuba v4.2.2 integration (static) library
# make -j1 explicit to emphasize that cuba does not build well with many cores
WORKDIR /tmp
RUN wget https://feynarts.de/cuba/Cuba-4.2.2.tar.gz && \
tar xf Cuba-4.2.2.tar.gz && \
cd Cuba-4.2.2 && \
./configure --prefix=/usr/local && \
make lib -j1 && \
make install && \
rm  -rf Cuba-4.2.2.tar.gz  Cuba-4.2.2

RUN ldconfig

WORKDIR /app

##copy MMHT2014nlo68cl grids (collinear functions and other coefficients)
COPY grids/ /usr/local/share/LHAPDF/

COPY . .

RUN cmake --workflow --preset verify-audit
# RUN cmake --preset release && cmake --build --preset release -j$(nproc) && cmake --install build/release
#########################################
###### build with release presets
#########################################
RUN cmake --workflow --preset verify-release

FROM debian:12-slim

WORKDIR /app

RUN apt-get update && apt-get install -y \
libgomp1 \
libgsl27 \
libstdc++6 \
&& rm -rf /var/lib/apt/lists/*

COPY --from=auditor /usr/local/lib /usr/local/lib
COPY --from=auditor /usr/local/include /usr/local/include
COPY --from=auditor /usr/local/bin /usr/local/bin
COPY --from=auditor /usr/local/share/LHAPDF /usr/local/share/LHAPDF
COPY --from=auditor /app/data   /app/data
COPY --from=auditor /app/build/release/bin/HSODrellYanFitter /app/HSODrellYanFitter

RUN chmod -R 755 /app/data && ldconfig

ENV CUBACORES=5

