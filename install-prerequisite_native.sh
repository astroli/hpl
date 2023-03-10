#!/bin/bash

OPENMPI_INSTALL_PATH=/mnt/workspace/hpl/install
OPENBLAS_INSTALL_PATH=/mnt/workspace/hpl/install

#OPENMPI_DEBUG=""
OPENMPI_DEBUG="--enable-debug"

# openmpi
echo "Build openmpi"
if [ ! -d openmpi-4.1.5 ]; then
  wget https://download.open-mpi.org/release/open-mpi/v4.1/openmpi-4.1.5.tar.bz2
  tar xvf openmpi-4.1.5.tar.bz2
fi

pushd openmpi-4.1.5
if [ ! -d build ]; then
  mkdir build && cd build
  ../configure --prefix=$OPENMPI_INSTALL_PATH $OPENMPI_DEBUG
  make all -j $(nproc)
  make install
fi
popd

if [ ! -e $OPENMPI_INSTALL_PATH/bin/mpirun ]; then
  echo "mpirun does not exist"
  exit 1
fi

# openblas
echo "Build openblas"
if [ ! -d OpenBLAS ]; then
  git clone https://github.com/xianyi/OpenBLAS.git
fi

pushd OpenBLAS
make
make PREFIX=$OPENBLAS_INSTALL_PATH install
popd

if [ ! -e $OPENBLAS_INSTALL_PATH/lib/libopenblas.a ]; then
  echo "libopenblas.a does not exists."
  exit 1
fi


