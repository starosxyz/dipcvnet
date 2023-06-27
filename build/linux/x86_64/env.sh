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
export RTE_ROOT=/opt/staros.xyz/rte
#######################CJSON########################
export STAR5GCORE_ROOT=$SRCDIR/
export STAR5GCORE_LIB_PATH=$ROOTDIR/target
export STAR5GCORE_INCLUDE=$STAR5GCORE_ROOT/include
export _CPU_TYPE=_CPU_X86_64
export _MACHINE=m64
export _CPU=x86_64

export CC=gcc
export CXX=g++


###########################################################
export PKG_CONFIG_PATH=$RTE_ROOT/libs/pkgconfig:$DPDK_ROOT/libs/pkgconfig:$ROOTDIR/dependency:$PKG_CONFIG_PATH

LD_LIBRARY_PATH=$RTE_ROOT/libs:$DPDK_ROOT/libs:$XOS_LIB_PATH:$DIPC_LIB_PATH:$STAR5GCORE_LIB_PATH:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH
###########################################################
