#!/bin/bash

export LD_LIBRARY_PATH=/mnt/workspace/hpc/install/lib
export PATH=/opt/riscv/bin:/mnt/workspace/hpc/install/bin:$PATH

CROSS_COMPILE=riscv64-unknown-linux-gnu- make arch=Linux_RISCV
