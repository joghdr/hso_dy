ARG HSO_IMAGE_NAME="hso-dy-default"
ARG HSO_VERSION="1.0.x"

# ----- hso dev tools -----
FROM ubuntu:24.04 AS hso-dy-dev

RUN apt-get update && apt-get install -y --no-install-recommends \
build-essential \
ca-certificates \
cmake \
git \
ninja-build \
pkg-config \
python3 \
python3-dev \
wget \
&& rm -rf /var/lib/apt/lists/*

RUN ln -s /usr/bin/python3 /usr/bin/python


# ----- hso dependencies -----
FROM ${HSO_IMAGE_NAME}-dev:${HSO_VERSION} AS hso-dy-deps

RUN apt-get update && apt-get install -y --no-install-recommends \
libgsl-dev \
&& rm -rf /var/lib/apt/lists/*

#LHAPDF v6.5.4
RUN wget https://lhapdf.hepforge.org/downloads/?f=LHAPDF-6.5.4.tar.gz -O LHAPDF-6.5.4.tar.gz && \
tar xf LHAPDF-6.5.4.tar.gz && \
cd LHAPDF-6.5.4 && \
./configure --prefix=/usr/local --disable-python && \
make -j$(nproc) && \
make install && \
rm -rf LHAPDF-6.5.4.tar.gz LHAPDF-6.5.4

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

#cuba v4.2.2 integration (static) library
# make -j1 explicit, cuba does not build correctly with many cores
RUN wget https://feynarts.de/cuba/Cuba-4.2.2.tar.gz && \
tar xf Cuba-4.2.2.tar.gz && \
cd Cuba-4.2.2 && \
./configure --prefix=/usr/local && \
make lib -j1 && \
make install && \
rm  -rf Cuba-4.2.2.tar.gz  Cuba-4.2.2

RUN ldconfig

##copy MMHT2014nlo68cl grids (collinear functions and other coefficients)
COPY assets/grids/ /usr/local/share/LHAPDF/


# ----- hso audit and build -----
FROM ${HSO_IMAGE_NAME}-deps:${HSO_VERSION} AS hso-dy-build

WORKDIR /app

COPY . .

RUN cd hso_engine 2> /dev/null || true

RUN cmake --workflow --preset verify-release

# ----- hso light release image -----
FROM ubuntu:24.04 AS hso-dy

WORKDIR /app

RUN apt-get update && apt-get install -y \
libgomp1 \
libgsl27 \
libstdc++6 \
&& rm -rf /var/lib/apt/lists/*

COPY --from=hso-dy-build /app/build/release/bin/HSODrellYanFitter /usr/local/bin/HSODrellYanFitter
COPY --from=hso-dy-build /app/build/release/bin/HSODrellYanHessian /usr/local/bin/HSODrellYanHessian
COPY --from=hso-dy-build   /usr/local/lib/libMinuit2.so      /usr/local/lib
COPY --from=hso-dy-build   /usr/local/lib/libMinuit2Math.so  /usr/local/lib
COPY --from=hso-dy-build   /usr/local/lib/libLHAPDF.so       /usr/local/lib
COPY --from=hso-dy-build   /usr/local/share/LHAPDF           /usr/local/share/LHAPDF
COPY --from=hso-dy-build /app/assets/data   /app/assets/data

RUN chmod -R 755 /app/assets/data && ldconfig

ENV CUBACORES=5

