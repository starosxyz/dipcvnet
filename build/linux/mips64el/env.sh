#!/bin/sh
#
# Copyright (C) 2013-2015, Nanjing StarOS Technology Co., Ltd
#
CURRENT=`pwd`
export ROOTDIR=$CURRENT/../../..
export SRCDIR=$ROOTDIR/src
export PLATFORM_ROOT=$ROOTDIR
export USER_DEPTH=$ROOTDIR
#######################CJSON########################
export CJSON_ROOT=$SRCDIR/cjson
export CJSON_LIB_PATH=$ROOTDIR/target
export CJSON_INCLUDE=$CJSON_ROOT/include

#######################CJSON########################
export LIBSEC_ROOT=$SRCDIR/libsec
export LIBSEC_LIB_PATH=$ROOTDIR/target
export LIBSEC_INCLUDE=$LIBSEC_ROOT/include

#######################VOS########################
export VOS_ROOT=$SRCDIR/vos
export VOS_LIB_PATH=$ROOTDIR/target
export VOS_INCLUDE=$VOS_ROOT/include

#######################GLOG########################
export GLOG_ROOT=$SRCDIR/glog
export GLOG_LIB_PATH=$ROOTDIR/target
export GLOG_INCLUDE=$GLOG_ROOT/include/
#######################ZLIB##############################
export ZLIB_ROOT=$SRCDIR/zlib
export ZLIB_LIB_PATH=$ZLIB_ROOT/
export ZLIB_INCLUDE=$ZLIB_ROOT/

#######################WFACE########################
export WFACE_ROOT=$SRCDIR/wface
export WFACE_LIB_PATH=$ROOTDIR/target
export WFACE_INCLUDE=$WFACE_ROOT/include
#######################FIPC########################
export FIPC_ROOT=$SRCDIR/fipc
export FIPC_LIB_PATH=$ROOTDIR/target
export FIPC_INCLUDE=$FIPC_ROOT/include

#######################SSL########################
export SSL_ROOT=$SRCDIR/openssl
export SSL_INCLUDE=$SSL_ROOT/openssl/include/
export SSL_LIB_PATH=$SSL_ROOT/openssl/
#######################RTC##############################
export RTC_ROOT=$SRCDIR/webrtc
export RTC_INCLUDE=$RTC_ROOT/include
export RTC_LIB_PATH=$ROOTDIR/target
#######################JSON##############################
export JSON_ROOT=$SRCDIR/jsoncpp
export JSON_INCLUDE=$JSON_ROOT/include
export JSON_LIB_PATH=$ROOTDIR/target

#######################STARBASE##############################
export STARBASE_ROOT=$SRCDIR/starbase
export STARBASE_INCLUDE=$STARBASE_ROOT/include
export STARBASE_LIB_PATH=$ROOTDIR/target

#######################UNITTEST##############################
export UNITTEST_ROOT=$ROOTDIR/unittest
export UNITTEST_INCLUDE=$UNITTEST_ROOT/include
export UNITTEST_LIB_PATH=$ROOTDIR/target

#######################DOXYGEN########################
export DOXYGEN_ROOT=$ROOTDIR/doxygen
export SNAPPY_ROOT=$SRCDIR/snappy
export SNAPPY_LIB_PATH=$ROOTDIR/target
export SNAPPY_INCLUDE=$SNAPPY_ROOT/include

#######################SHTTP##############################
export SHTTP_ROOT=$SRCDIR/shttp
export SHTTP_LIB_PATH=$ROOTDIR/target
export SHTTP_INCLUDE=$SHTTP_ROOT/include

#######################leveldb########################
export LEVELDB_ROOT=$SRCDIR/leveldb
export LEVELDB_LIB_PATH=$ROOTDIR/target
export LEVELDB_INCLUDE=$LEVELDB_ROOT/include

#######################sdb########################
export SDB_ROOT=$SRCDIR/stardb
export SDB_LIB_PATH=$ROOTDIR/target
export SDB_INCLUDE=$SDB_ROOT/include

#######################sqlitedb########################
export SQLITEDB_ROOT=$SRCDIR/sqlitedb
export SQLITEDB_LIB_PATH=$ROOTDIR/target
export SQLITEDB_INCLUDE=$SQLITEDB_ROOT/include

export _CPU_TYPE=_CPU_MIPS
export _MACHINE=
export _CPU=mips


export CC=/opt/gcc-4.9.3-64-gnu/bin/mips64el-linux-gcc
export CXX=/opt/gcc-4.9.3-64-gnu/bin/mips64el-linux-g++
export LD=/opt/gcc-4.9.3-64-gnu/bin/mips64el-linux-ld
export AR=/opt/gcc-4.9.3-64-gnu/bin/mips64el-linux-ar
export RANLIB=/opt/gcc-4.9.3-64-gnu/bin/mips64el-linux-ranlib
export NM=/opt/gcc-4.9.3-64-gnu/bin/mips64el-linux-nm
export STRIP=/opt/gcc-4.9.3-64-gnu/bin/mips64el-linux-strip
export OBJDUMP=/opt/gcc-4.9.3-64-gnu/bin/mips64el-linux-objdump
export AS=/opt/gcc-4.9.3-64-gnu/bin/mips64el-linux-as



###########################################################

LD_LIBRARY_PATH=$SSL_ROOT/openssl/:$ZLIB_LIB_PATH:$PLATFORM_ROOT/target:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH
###########################################################
