#!/bin/bash

export LD_LIBRARY_PATH=/mnt/workspace/hpc/install/lib
export PATH=/opt/riscv/bin:/mnt/workspace/hpc/install/bin:$PATH

CROSS_COMPILE=riscv64-unknown-linux-gnu- make arch=Linux_RISCV clean
rm -f bin/Linux_RISCV/xhpl
rm -f testing/ptest/Linux_RISCV/HPL_pdtest.o
CROSS_COMPILE=riscv64-unknown-linux-gnu- make arch=Linux_RISCV

if [ ! bin/Linux_RISCV/xhpl ]; then
    echo ""
    echo "HPL build failed"
    exit 1
fi

echo ""
echo "HPL build succeeded"
