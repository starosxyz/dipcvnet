#
# Copyright (C) 2016-2017, Nanjing StarOS Technology Co., Ltd
#
build_platform=1
build_glog=1
build_ace=1
build_zlib=1
build_openssl=1
build_unittest=0
#######################################################################################
DOXY_INPUT = $(ROOTDIR)/README.md $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/
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
TARNAME=platform_release_d_$(os)_armv7_v$(VERSION)
INSTALLNAME=install.sh
else
TARNAME=platform_release_s_$(os)_armv7_v$(VERSION)
INSTALLNAME=install.sh
endif
else
ifeq (0,$(static))
TARNAME=platform_debug_d_$(os)_armv7_v$(VERSION)
INSTALLNAME=install.sh
else
TARNAME=platform_debug_s_$(os)_armv7_v$(VERSION)
INSTALLNAME=install.sh
endif
endif

export STARDLLBUILD=$(ROOTDIR)/build/linux/armv7/$(TARNAME)

ifeq (1,$(build_glog))
buidlall +=glog
tocleanall +=glogclean
endif

ifeq (1,$(build_zlib))
buidlall +=zlib
tocleanall +=zlibclean
endif

ifeq (1,$(build_openssl))
buidlall +=openssl
tocleanall +=opensslclean
endif


ifeq (1,$(build_ace))
buidlall +=ace
tocleanall +=aceclean
endif

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
	cp -rf $(ROOTDIR)/docs/* $(ROOTDIR)/build/linux/armv7/$(TARNAME)/docs/
	cp -rf $(ROOTDIR)/conf/* $(ROOTDIR)/build/linux/armv7/$(TARNAME)/conf/
	cp -rf $(ROOTDIR)/license $(ROOTDIR)/build/linux/armv7/$(TARNAME)/
	cp $(ROOTDIR)/COPYING $(ROOTDIR)/build/linux/armv7/$(TARNAME)/
	cp -rf $(ROOTDIR)/include/platformversion.h $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/
	mv $(TARNAME) tmp
	tar zcvf $(TARNAME).tar.gz -C $(ROOTDIR)/build/linux/armv7/ tmp
	cat $(INSTALLNAME) $(TARNAME).tar.gz > $(TARNAME).bin 
	@echo "build" $(buidlall)
	rm -rf tmp
	chmod a+x $(TARNAME).bin

.PHONY:glog
glog:check_dir
	make -C $(GLOG_ROOT)  static=$(static)
ifeq (0,$(static))
	cp $(GLOG_LIB_PATH)/libglog.so $(ROOTDIR)/build/linux/armv7/$(TARNAME)/libs
else
	cp $(GLOG_LIB_PATH)/libglog.a $(ROOTDIR)/build/linux/armv7/$(TARNAME)/libs
endif
	cp -rf $(GLOG_INCLUDE)/glog/* $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/glog/
.PHONY:ace
ace: check_dir
ifeq (0,$(static))
	make -C $(SRCDIR)/ACE-6.5.9 ssl=1 static=0 static_libs_only=0
	rm -rf $(ACE_LIB_PATH)/*.a
	cp -rLf $(INSTALL_PREFIX)/lib/* $(ROOTDIR)/build/linux/armv7/$(TARNAME)/libs/
else
	make -C $(SRCDIR)/ACE-6.5.9 ssl=1 static=1 static_libs_only=1
	rm -rf $(ACE_LIB_PATH)/*.so
	cp $(INSTALL_PREFIX)/lib/libACE_ETCL_Parser.a $(ROOTDIR)/build/linux/armv7/$(TARNAME)/libs/
	cp $(INSTALL_PREFIX)/lib/libACE_ETCL.a $(ROOTDIR)/build/linux/armv7/$(TARNAME)/libs/
	cp $(INSTALL_PREFIX)/lib/libACE_Monitor_Control.a $(ROOTDIR)/build/linux/armv7/$(TARNAME)/libs/
	cp $(INSTALL_PREFIX)/lib/libACE_RLECompression.a $(ROOTDIR)/build/linux/armv7/$(TARNAME)/libs/
	cp $(INSTALL_PREFIX)/lib/libACE.a $(ROOTDIR)/build/linux/armv7/$(TARNAME)/libs/
	cp $(INSTALL_PREFIX)/lib/libACE_SSL.a $(ROOTDIR)/build/linux/armv7/$(TARNAME)/libs/
	cp $(INSTALL_PREFIX)/lib/libACE_Compression.a $(ROOTDIR)/build/linux/armv7/$(TARNAME)/libs/
endif
	cp -rf $(INSTALL_PREFIX)/include/ace/* $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/ace/
.PHONY:zlib
zlib:check_dir
ifeq (0,$(static))
	cd $(ZLIB_ROOT) && chmod a+x configure && ./configure && make
	cp -Lf $(ZLIB_ROOT)/libz.so $(ROOTDIR)/build/linux/armv7/$(TARNAME)/libs/
	cp -Lf $(ZLIB_ROOT)/libz.so $(ROOTDIR)/build/linux/armv7/$(TARNAME)/libs/libz.so.1
else
	cd $(ZLIB_ROOT) && chmod a+x configure && ./configure --static && make
	cp -Lf $(ZLIB_ROOT)/libz.a $(ROOTDIR)/build/linux/armv7/$(TARNAME)/libs/
endif
	cp -rf $(ZLIB_ROOT)/zlib.h $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/zlib/
	cp -rf $(ZLIB_ROOT)/zconf.h $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/zlib/
.PHONY:openssl
openssl:check_dir
	@cd $(SSL_ROOT)/openssl-1.1.1i && chmod a+x Configure &&./Configure linux-arm shared && make
ifeq (0,$(static))
	cp -Lf $(SSL_ROOT)/openssl-1.1.1i/libssl.so $(ROOTDIR)/build/linux/armv7/$(TARNAME)/libs/
	cp -Lf $(SSL_ROOT)/openssl-1.1.1i/libcrypto.so $(ROOTDIR)/build/linux/armv7/$(TARNAME)/libs/
	cp -Lf $(SSL_ROOT)/openssl-1.1.1i/libssl.so $(ROOTDIR)/build/linux/armv7/$(TARNAME)/libs/libssl.so.1.1
	cp -Lf $(SSL_ROOT)/openssl-1.1.1i/libcrypto.so $(ROOTDIR)/build/linux/armv7/$(TARNAME)/libs/libcrypto.so.1.1
	cp -Lf $(SSL_ROOT)/openssl-1.1.1i/libssl.so $(SSL_ROOT)/lib/
	cp -Lf $(SSL_ROOT)/openssl-1.1.1i/libcrypto.so $(SSL_ROOT)/lib/
	cp -Lf $(SSL_ROOT)/openssl-1.1.1i/libssl.so $(SSL_ROOT)/lib/libssl.so.1.1
	cp -Lf $(SSL_ROOT)/openssl-1.1.1i/libcrypto.so $(SSL_ROOT)/lib/libcrypto.so.1.1
else
	cp -Lf $(SSL_ROOT)/openssl-1.1.1i/libssl.a $(ROOTDIR)/build/linux/armv7/$(TARNAME)/libs/
	cp -Lf $(SSL_ROOT)/openssl-1.1.1i/libcrypto.a $(ROOTDIR)/build/linux/armv7/$(TARNAME)/libs/
	cp -Lf $(SSL_ROOT)/openssl-1.1.1i/libssl.a $(SSL_ROOT)/lib/
	cp -Lf $(SSL_ROOT)/openssl-1.1.1i/libcrypto.a $(SSL_ROOT)/lib/
endif
	cp -Lf $(SSL_ROOT)/openssl-1.1.1i/include/openssl/*.h $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/openssl/
	cp -Lf $(SSL_ROOT)/openssl-1.1.1i/include/openssl/*.h $(SSL_ROOT)/include/
.PHONY:platform
platform:check_dir
	make -C $(PLATFORM_ROOT)  static=$(static)
	cp -rLf $(LIBSEC_INCLUDE)/libsec/* $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/libsec/
	cp -rLf $(VOS_INCLUDE)/vos/* $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/vos/
	cp -rLf $(CJSON_INCLUDE)/cjson/* $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/cjson/
	cp -rLf $(SNAPPY_INCLUDE)/snappy/* $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/snappy/
	cp -rf $(JSON_ROOT)/include/json/* $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/json/
	cp -rf $(STARBASE_ROOT)/include/starbase/* $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/starbase/
	cp -rf $(WFACE_ROOT)/include/wface/* $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/wface/
	cp -rf $(FIPC_ROOT)/include/fipc/* $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/fipc/
	cp -rf $(SHTTP_ROOT)/include/shttp/* $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/shttp/
	cp -rf $(RTC_ROOT)/include/webrtc/* $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/webrtc/
	cp -rf $(LEVELDB_ROOT)/include/leveldb/*.h $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/leveldb/
	cp -rf $(SDB_ROOT)/include/stardb/* $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/stardb/
	cp -rf $(SQLITEDB_ROOT)/include/sqlitedb/* $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/sqlitedb/
ifeq (0,$(static))
	cp -Lf $(ROOTDIR)/target/*.so $(ROOTDIR)/build/linux/armv7/$(TARNAME)/libs/
else
	cp -Lf $(ROOTDIR)/target/*.a $(ROOTDIR)/build/linux/armv7/$(TARNAME)/libs/
endif
.PHONY:unittest
unittest:check_dir
	make -C $(UNITTEST_ROOT) static=$(static)
	cp -rf $(UNITTEST_ROOT)/bin/* $(ROOTDIR)/build/linux/armv7/$(TARNAME)/bin/
.PHONY:doxygen	
doxygen:
	@sudo yum install -y doxygen graphviz
	ROOT="$(ROOTDIR)" \
	     BUILD_ROOT="$(ROOTDIR)/build/linux/armv7/$(TARNAME)/" \
	     INPUT="$(DOXY_INPUT)" \
	     HTML=YES \
	     VERSION="`git describe --tags --dirty`" \
	     doxygen $(DOXYGEN_ROOT)/doxygen.cfg
check_dir: 
	@test -d $(ROOTDIR)/build/linux/armv7/$(TARNAME) || mkdir -p $(ROOTDIR)/build/linux/armv7/$(TARNAME)
	@test -d $(ROOTDIR)/build/linux/armv7/$(TARNAME)/bin || mkdir -p $(ROOTDIR)/build/linux/armv7/$(TARNAME)/bin
	@test -d $(ROOTDIR)/build/linux/armv7/$(TARNAME)/conf || mkdir -p $(ROOTDIR)/build/linux/armv7/$(TARNAME)/conf
	@test -d $(ROOTDIR)/build/linux/armv7/$(TARNAME)/logs || mkdir -p $(ROOTDIR)/build/linux/armv7/$(TARNAME)/logs
	@test -d $(ROOTDIR)/build/linux/armv7/$(TARNAME)/libs || mkdir -p $(ROOTDIR)/build/linux/armv7/$(TARNAME)/libs
	@test -d $(ROOTDIR)/build/linux/armv7/$(TARNAME)/docs || mkdir -p $(ROOTDIR)/build/linux/armv7/$(TARNAME)/docs
	@test -d $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include || mkdir -p $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include
	@test -d $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/openssl || mkdir -p $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/openssl
	@test -d $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/starbase || mkdir -p $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/starbase
	@test -d $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/wface || mkdir -p $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/wface
	@test -d $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/json || mkdir -p $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/json
	@test -d $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/webrtc || mkdir -p $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/webrtc
	@test -d $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/webrtc/base || mkdir -p $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/webrtc/base
	@test -d $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/webrtc/system_wrappers || mkdir -p $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/webrtc/system_wrappers
	@test -d $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/webrtc/system_wrappers/include || mkdir -p $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/webrtc/system_wrappers/include
	@test -d $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/fipc || mkdir -p $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/fipc
	@test -d $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/shttp || mkdir -p $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/shttp
	@test -d $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/leveldb || mkdir -p $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/leveldb
	@test -d $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/zlib || mkdir -p $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/zlib
	@test -d $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/ace || mkdir -p $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/ace
	@test -d $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/glog || mkdir -p $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/glog
	@test -d $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/snappy || mkdir -p $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/snappy
	@test -d $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/cjson || mkdir -p $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/cjson
	@test -d $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/libsec || mkdir -p $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/libsec
	@test -d $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/vos || mkdir -p $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/vos
	@test -d $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/sqlitedb || mkdir -p $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/sqlitedb
	@test -d $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/stardb || mkdir -p $(ROOTDIR)/build/linux/armv7/$(TARNAME)/include/stardb
	@test -d $(SSL_ROOT)/lib || mkdir -p $(SSL_ROOT)/lib
#################################################################################
	
cleanall: $(tocleanall)
###########################################################################################
.PHONY:platformclean
platformclean:
	make clean -C $(PLATFORM_ROOT)
	rm -rf $(PLATFORM_ROOT)/target/*
.PHONY: aceclean
aceclean:
	make clean -C $(ROOTDIR)/src/ACE-6.5.9

.PHONY:opensslclean
opensslclean:
	make clean -C $(SSL_ROOT)/openssl-1.1.1i
	rm -rf $(SSL_ROOT)/lib

.PHONY:unittestclean
unittestclean:
	make clean -C $(UNITTEST_ROOT)
	rm -rf $(UNITTEST_ROOT)/target/*

.PHONY: zlibclean
zlibclean:
	make clean -C $(ZLIB_ROOT)
.PHONY:glogclean
glogclean:
	make clean -C $(GLOG_ROOT)
	rm -rf $(GLOG_ROOT)/target/*
