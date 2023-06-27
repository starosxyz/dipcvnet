#!/bin/sh
#
# Copyright (C) 2013-2015, Nanjing StarOS Technology Co., Ltd
#
CURRENT=`pwd`
export ROOTDIR=$CURRENT/../../..
export SRCDIR=$ROOTDIR/src
export DIPC_ROOT=/opt/staros.xyz/dipc
export DIPC_INCLUDE=/opt/staros.xyz/dipc/include
export DIPC_LIB_PATH=/opt/staros.xyz/dipc/dlls
export USER_DEPTH=$ROOTDIR

export XOS_ROOT=/opt/staros.xyz/xos
export XOS_INCLUDE=/opt/staros.xyz/xos/include
export XOS_LIB_PATH=/opt/staros.xyz/xos/libs
export DPDK_ROOT=/opt/staros.xyz/dpdk

#######################CJSON########################
export STAR5GCORE_ROOT=$SRCDIR/
export STAR5GCORE_LIB_PATH=$ROOTDIR/target
export STAR5GCORE_INCLUDE=$STAR5GCORE_ROOT/include
export _CPU_TYPE=_CPU_AARCH64
export _MACHINE=
export _CPU=arm64

export CC=aarch64-unknown-linux-gnu-gcc
export CXX=aarch64-unknown-linux-gnu-g++
export LD=aarch64-linux-gnu-ld
export AR=aarch64-unknown-linux-gnu-gcc-ar
export RANLIB=aarch64-unknown-linux-gnu-gcc-ranlib
export NM=aarch64-unknown-linux-gnu-gcc-nm
export STRIP=aarch64-linux-gnu-strip
export OBJDUMP=aarch64-linux-gnu-objdump
export AS=aarch64-linux-gnu-as

export GCCLIB_PATH=/usr/local/gcc-9.3.0/lib64/


###########################################################
export PKG_CONFIG_PATH=/opt/staros.xyz/dpdk/libs/pkgconfig:$ROOTDIR/dependency:$PKG_CONFIG_PATH

LD_LIBRARY_PATH=$XOS_LIB_PATH:$DIPC_LIB_PATH:$STAR5GCORE_LIB_PATH:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH
###########################################################
