#!/bin/bash

GNU_TOOLCHAIN_PATH=/opt/riscv
OPENMPI_INSTALL_PATH=/mnt/workspace/hpc/install
OPENBLAS_INSTALL_PATH=/mnt/workspace/hpc/install

export PATH=$GNU_TOOLCHAIN_PATH/bin:$PATH

# openmpi
echo "Build openmpi"
if [ ! -d openmpi-4.1.5 ]; then
  wget https://download.open-mpi.org/release/open-mpi/v4.1/openmpi-4.1.5.tar.bz2
  tar xvf openmpi-4.1.5.tar.bz2
fi

pushd openmpi-4.1.5
if [ ! -d build ]; then
  mkdir build
fi
  cd build
  ../configure \
    --host=riscv64-unknown-linux-gnu \
    --build=x86_64-linux-gnu \
    --disable-mpi-fortran \
    --disable--oshmem-fortran \
    --without-memory-manager \
    --disable-dlopen \
    --enable-static \
    --prefix=$OPENMPI_INSTALL_PATH

  make all -j $(nproc)
  make install
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
make HOSTCC=gcc TARGET=RISCV64_GENERIC CC=riscv64-unknown-linux-gnu-gcc FC=riscv64-unknown-linux-gnu-gfortran
make HOSTCC=gcc TARGET=RISCV64_GENERIC CC=riscv64-unknown-linux-gnu-gcc FC=riscv64-unknown-linux-gnu-gfortran PREFIX=$OPENBLAS_INSTALL_PATH install
popd

if [ ! -e $OPENBLAS_INSTALL_PATH/lib/libopenblas.a ]; then
  echo "libopenblas.a does not exists."
  exit 1
fi


