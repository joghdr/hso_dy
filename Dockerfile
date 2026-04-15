
#########################################
###### build with audit presets
#########################################

FROM debian:12-slim AS auditor

RUN apt-get update && apt-get install -y \
git \
build-essential \
cmake \
g++ \
gfortran \
libgomp1 \
pkg-config \
wget \
python3 \
python3-dev \
libboost-math-dev \
libgsl-dev \
&& rm -rf /var/lib/apt/lists/*

RUN apt-get update && apt-get install -y \
clang \
llvm \
&& rm -rf /var/lib/apt/lists/*

# 'python' symlink so for LHAPDF build
RUN ln -s /usr/bin/python3 /usr/bin/python

#Build LHAPDF
WORKDIR /tmp
RUN wget https://lhapdf.hepforge.org/downloads/?f=LHAPDF-6.5.4.tar.gz -O LHAPDF-6.5.4.tar.gz && \
tar xf LHAPDF-6.5.4.tar.gz && \
cd LHAPDF-6.5.4 && \
./configure --prefix=/usr/local && \
make -j$(nproc) && \
make install && \
rm -rf LHAPDF-6.5.4.tar.gz LHAPDF-6.5.4

#Build CUBA
WORKDIR /tmp
RUN wget https://feynarts.de/cuba/Cuba-4.2.2.tar.gz && \
tar xf Cuba-4.2.2.tar.gz && \
cd Cuba-4.2.2 && \
./configure --prefix=/usr/local && \
make lib -j$(nproc) && \
make install && \
rm  -rf Cuba-4.2.2.tar.gz  Cuba-4.2.2

#Build Minuit2
WORKDIR /tmp
RUN git clone --depth 1 https://github.com/GooFit/Minuit2.git && \
cd Minuit2 && \
mkdir build && cd build && \
cmake -DCMAKE_INSTALL_PREFIX=/usr/local \
-Dminuit2_omp=ON \
-Dminuit2_standalone=ON .. && \
make -j$(nproc) && \
make install && \
mkdir -p /usr/local/include/Minuit2 && \
cp -r /tmp/Minuit2/inc/* /usr/local/include/Minuit2/ && \
cp -r /tmp/Minuit2/inc/Minuit2/* /usr/local/include/Minuit2/ 2>/dev/null || true && \
cd .. && rm -rf Minuit2

WORKDIR /app

COPY . .

##copy MMHT2014nlo68cl grids (collinear functions and other coefficients)
COPY grids/ /usr/local/share/LHAPDF/

#########################################
###### verify audit build
#########################################
RUN make verify

#########################################
###### build with release presets
#########################################
RUN cmake --preset release && cmake --build --preset release -j$(nproc) && cmake --install build/release


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

RUN chmod -R 755 /app/data

RUN ldconfig

ENV OMP_NUM_THREADS=1

ENV CUBACORES=5

