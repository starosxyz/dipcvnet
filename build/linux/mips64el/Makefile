#
# Copyright (C) 2016-2017, Nanjing StarOS Technology Co., Ltd
#
build_platform=1
build_unittest=1
#######################################################################################
DOXY_INPUT = $(ROOTDIR)/README.md $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/
#######################################################################################
NULL_STDERR = 2>$(/dev/null) || true
MAJOR_VERSION := $(shell awk '/PLATFORM_MAJOR_VERSION/ { print $$3}' ${ROOTDIR}/include/platformversion.h)
MINOR_VERSION := $(shell awk '/PLATFORM_MINOR_VERSION/ { print $$3}' ${ROOTDIR}/include/platformversion.h)
BETA_VERSION := $(shell awk '/PLATFORM_BETA_VERSION/ { print $$3}' ${ROOTDIR}/include/platformversion.h)
VERSION = $(MAJOR_VERSION).$(MINOR_VERSION).$(BETA_VERSION)
#######################################################################################

ifeq (,$(os))
os=linux
endif

ifeq (0,$(debug))
ifeq (0,$(static))
TARNAME=platform_release_shared_$(os)_mips64el_v$(VERSION)
INSTALLNAME=install.sh
else
TARNAME=platform_release_static_$(os)_mips64el_v$(VERSION)
INSTALLNAME=install.sh
endif
else
ifeq (0,$(static))
TARNAME=platform_debug_shared_$(os)_mips64el_v$(VERSION)
INSTALLNAME=install.sh
else
TARNAME=platform_debug_static_$(os)_mips64el_v$(VERSION)
INSTALLNAME=install.sh
endif
endif

export STARDLLBUILD=$(ROOTDIR)/build/linux/mips64el/$(TARNAME)

ifeq (1,$(build_platform))
buidlall +=platform
tocleanall +=platformclean
endif


ifeq (1,$(build_unittest))
buidlall +=unittest
tocleanall +=unittestclean
endif

all: check_dir $(buidlall) 
	echo $(buidlall)
	cp -rf $(ROOTDIR)/docs/* $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/docs/
	cp -rf $(ROOTDIR)/conf/* $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/conf/
	cp -rf $(ROOTDIR)/license $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/
	cp $(ROOTDIR)/COPYING $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/
	cp -rf $(ROOTDIR)/include/platformversion.h $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/
	mv $(TARNAME) tmp
	tar zcvf $(TARNAME).tar.gz -C $(ROOTDIR)/build/linux/mips64el/ tmp
	cat $(INSTALLNAME) $(TARNAME).tar.gz > $(TARNAME).bin 
	@echo "build" $(buidlall)
	rm -rf tmp
	chmod a+x $(TARNAME).bin

.PHONY:platform
platform:check_dir
	make -C $(PLATFORM_ROOT)  static=$(static)
	cp -rLf $(LIBSEC_INCLUDE)/libsec/* $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/libsec/
	cp -rLf $(VOS_INCLUDE)/vos/* $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/vos/
	cp -rLf $(CJSON_INCLUDE)/cjson/* $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/cjson/
	cp -rLf $(SNAPPY_INCLUDE)/snappy/* $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/snappy/
	cp -rf $(JSON_ROOT)/include/json/* $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/json/
	cp -rf $(STARBASE_ROOT)/include/starbase/* $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/starbase/
	cp -rf $(WFACE_ROOT)/include/wface/* $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/wface/
	cp -rf $(FIPC_ROOT)/include/fipc/* $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/fipc/
	cp -rf $(SHTTP_ROOT)/include/shttp/* $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/shttp/
	cp -rf $(RTC_ROOT)/include/webrtc/* $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/webrtc/
	cp -rf $(LEVELDB_ROOT)/include/leveldb/*.h $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/leveldb/
	cp -rf $(SDB_ROOT)/include/stardb/* $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/stardb/
	cp -rf $(SQLITEDB_ROOT)/include/sqlitedb/* $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/sqlitedb/
	cp -rf $(GLOG_INCLUDE)/glog/* $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/glog/
	cp -rf $(ZLIB_ROOT)/zlib.h $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/zlib/
	cp -rf $(ZLIB_ROOT)/zconf.h $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/zlib/
	cp -rf $(ROOTDIR)/src/uv/include/* $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/
	cp -rf $(ROOTDIR)/src/expat/include/* $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/expat/
	cp -rf $(ROOTDIR)/src/nghttp2/lib/includes/nghttp2/* $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/nghttp2/
	cp -rf $(ROOTDIR)/src/ngtcp2/nghttp3/lib/includes/nghttp3/* $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/nghttp3/
	cp -rf $(ROOTDIR)/src/ngtcp2/ngtcp2/lib/includes/ngtcp2/* $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/ngtcp2/
	cp -rf $(ROOTDIR)/src/ngtcp2/ngtcp2/crypto/includes/ngtcp2/* $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/ngtcp2/
	cp -rf $(SSL_ROOT)/openssl/include/* $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/
ifeq (0,$(static))
	cp -Lf $(ROOTDIR)/target/*.so $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/libs/
else
	cp -Lf $(ROOTDIR)/target/*.a $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/libs/
endif
.PHONY:unittest
unittest:check_dir
	make -C $(UNITTEST_ROOT) static=$(static)
	cp -rf $(UNITTEST_ROOT)/bin/* $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/bin/
.PHONY:doxygen	
doxygen:
	@sudo yum install -y doxygen graphviz
	ROOT="$(ROOTDIR)" \
	     BUILD_ROOT="$(ROOTDIR)/build/linux/mips64el/$(TARNAME)/" \
	     INPUT="$(DOXY_INPUT)" \
	     HTML=YES \
	     VERSION="`git describe --tags --dirty`" \
	     doxygen $(DOXYGEN_ROOT)/doxygen.cfg
check_dir: 
	@test -d $(ROOTDIR)/build/linux/mips64el/$(TARNAME) || mkdir -p $(ROOTDIR)/build/linux/mips64el/$(TARNAME)
	@test -d $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/bin || mkdir -p $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/bin
	@test -d $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/conf || mkdir -p $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/conf
	@test -d $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/logs || mkdir -p $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/logs
	@test -d $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/libs || mkdir -p $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/libs
	@test -d $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/docs || mkdir -p $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/docs
	@test -d $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include || mkdir -p $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include
	@test -d $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/openssl || mkdir -p $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/openssl
	@test -d $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/starbase || mkdir -p $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/starbase
	@test -d $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/wface || mkdir -p $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/wface
	@test -d $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/json || mkdir -p $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/json
	@test -d $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/webrtc || mkdir -p $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/webrtc
	@test -d $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/webrtc/base || mkdir -p $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/webrtc/base
	@test -d $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/webrtc/system_wrappers || mkdir -p $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/webrtc/system_wrappers
	@test -d $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/webrtc/system_wrappers/include || mkdir -p $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/webrtc/system_wrappers/include
	@test -d $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/fipc || mkdir -p $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/fipc
	@test -d $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/shttp || mkdir -p $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/shttp
	@test -d $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/leveldb || mkdir -p $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/leveldb
	@test -d $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/zlib || mkdir -p $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/zlib
	@test -d $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/glog || mkdir -p $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/glog
	@test -d $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/snappy || mkdir -p $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/snappy
	@test -d $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/cjson || mkdir -p $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/cjson
	@test -d $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/libsec || mkdir -p $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/libsec
	@test -d $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/vos || mkdir -p $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/vos
	@test -d $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/sqlitedb || mkdir -p $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/sqlitedb
	@test -d $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/stardb || mkdir -p $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/stardb
	@test -d $(SSL_ROOT)/lib || mkdir -p $(SSL_ROOT)/lib
	@test -d $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/expat || mkdir -p $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/expat
	@test -d $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/uv || mkdir -p $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/uv
	@test -d $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/nghttp2 || mkdir -p $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/nghttp2
	@test -d $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/ngtcp2 || mkdir -p $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/ngtcp2
	@test -d $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/nghttp3 || mkdir -p $(ROOTDIR)/build/linux/mips64el/$(TARNAME)/include/nghttp3
#################################################################################
	
cleanall: $(tocleanall)
###########################################################################################
.PHONY:platformclean
platformclean:
	make clean -C $(PLATFORM_ROOT)
	rm -rf $(PLATFORM_ROOT)/target/*

.PHONY:opensslclean
opensslclean:
	make clean -C $(SSL_ROOT)/openssl
	rm -rf $(SSL_ROOT)/lib

.PHONY:unittestclean
unittestclean:
	make clean -C $(UNITTEST_ROOT)
	rm -rf $(UNITTEST_ROOT)/target/*
