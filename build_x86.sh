#!/bin/bash

export LD_LIBRARY_PATH=/mnt/workspace/hpl/install/lib
export PATH=/mnt/workspace/hpl/install/bin:$PATH

make arch=Linux_ATHLON_CBLAS

