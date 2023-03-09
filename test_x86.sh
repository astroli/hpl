#!/bin/bash

export LD_LIBRARY_PATH=/mnt/workspace/hpl/install/lib
export PATH=/mnt/workspace/hpl/install/bin:$PATH

pushd bin/Linux_ATHLON_CBLAS

mpirun -np 16 --use-hwthread-cpus ./xhpl

popd
