#
# Copyright (C) 2016-2017, Nanjing StarOS Technology Co., Ltd
#
build_star5gcore=1
VERSION = 1.0.5
#######################################################################################

ifeq (,$(os))
os=linux
endif

ifeq (0,$(debug))
TARNAME=star5gcore_release_$(os)_d2000_v$(VERSION)
INSTALLNAME=install.sh
else
TARNAME=star5gcore_debug_$(os)_d2000_v$(VERSION)
INSTALLNAME=install.sh
endif

export STARDLLBUILD=$(ROOTDIR)/build/linux/d2000/$(TARNAME)

ifeq (1,$(build_star5gcore))
buidlall +=star5gcore
tocleanall +=star5gcoreclean
endif


all: check_dir $(buidlall) 
	echo $(buidlall)
	cp -rf $(ROOTDIR)/starlang/* $(ROOTDIR)/build/linux/d2000/$(TARNAME)/starlang/
	cp -rf $(ROOTDIR)/resources/* $(ROOTDIR)/build/linux/d2000/$(TARNAME)/resources/
	cp -rf $(ROOTDIR)/conf/* $(ROOTDIR)/build/linux/d2000/$(TARNAME)/conf/
	cp -rf $(ROOTDIR)/bootstrap/* $(ROOTDIR)/build/linux/d2000/$(TARNAME)/bootstrap/
	cp -rf /opt/staros.xyz/vos/libs/* $(ROOTDIR)/build/linux/d2000/$(TARNAME)/libs/
	cp -rf /opt/staros.xyz/dpdk/libs/* $(ROOTDIR)/build/linux/d2000/$(TARNAME)/libs/
	cp -rf /opt/staros.xyz/contrib/libs/* $(ROOTDIR)/build/linux/d2000/$(TARNAME)/libs/
	cp -rf /opt/staros.xyz/fstack/libs/* $(ROOTDIR)/build/linux/d2000/$(TARNAME)/libs/
	mv $(TARNAME) tmp
	tar zcvf $(TARNAME).tar.gz -C $(ROOTDIR)/build/linux/d2000/ tmp
	cat $(INSTALLNAME) $(TARNAME).tar.gz > $(TARNAME).bin 
	@echo "build" $(buidlall)
	rm -rf tmp
	chmod a+x $(TARNAME).bin
.PHONY:star5gcore
star5gcore:check_dir
	make -C $(STAR5GCORE_ROOT)
	cp -Lf $(ROOTDIR)/bin/* $(ROOTDIR)/build/linux/d2000/$(TARNAME)/bin/
	cp -rf /opt/staros.xyz/fstack/bin/* $(ROOTDIR)/build/linux/d2000/$(TARNAME)/bin/fstackbin/
	cp -Lf /opt/staros.xyz/contrib/libs/libxosdbg_msg_dumps.so $(ROOTDIR)/build/linux/d2000/$(TARNAME)/libs/libdbg_msg_dumps.fdx
	cp -Lf /opt/staros.xyz/contrib/libs/libxosdict_rfc5777.so $(ROOTDIR)/build/linux/d2000/$(TARNAME)/libs/libdict_rfc5777.fdx
	cp -Lf /opt/staros.xyz/contrib/libs/libxosdict_mip6i.so $(ROOTDIR)/build/linux/d2000/$(TARNAME)/libs/libdict_mip6i.fdx
	cp -Lf /opt/staros.xyz/contrib/libs/libxosdict_nasreq.so $(ROOTDIR)/build/linux/d2000/$(TARNAME)/libs/libdict_nasreq.fdx
	cp -Lf /opt/staros.xyz/contrib/libs/libxosdict_nas_mipv6.so $(ROOTDIR)/build/linux/d2000/$(TARNAME)/libs/libdict_nas_mipv6.fdx
	cp -Lf /opt/staros.xyz/contrib/libs/libxosdict_dcca.so $(ROOTDIR)/build/linux/d2000/$(TARNAME)/libs/libdict_dcca.fdx
	cp -Lf /opt/staros.xyz/contrib/libs/libxosdict_dcca_3gpp.so $(ROOTDIR)/build/linux/d2000/$(TARNAME)/libs/libdict_dcca_3gpp.fdx
ifeq (0,$(static))
	cp -Lf $(ROOTDIR)/target/*.so $(ROOTDIR)/build/linux/d2000/$(TARNAME)/libs/
else
	cp -Lf $(ROOTDIR)/target/*.a $(ROOTDIR)/build/linux/d2000/$(TARNAME)/libs/
endif
check_dir: 
	@test -d $(ROOTDIR)/build/linux/d2000/$(TARNAME) || mkdir -p $(ROOTDIR)/build/linux/d2000/$(TARNAME)
	@test -d $(ROOTDIR)/build/linux/d2000/$(TARNAME)/bin || mkdir -p $(ROOTDIR)/build/linux/d2000/$(TARNAME)/bin
	@test -d $(ROOTDIR)/build/linux/d2000/$(TARNAME)/bin/fstackbin || mkdir -p $(ROOTDIR)/build/linux/d2000/$(TARNAME)/bin/fstackbin
	@test -d $(ROOTDIR)/build/linux/d2000/$(TARNAME)/conf || mkdir -p $(ROOTDIR)/build/linux/d2000/$(TARNAME)/conf
	@test -d $(ROOTDIR)/build/linux/d2000/$(TARNAME)/logs || mkdir -p $(ROOTDIR)/build/linux/d2000/$(TARNAME)/logs
	@test -d $(ROOTDIR)/build/linux/d2000/$(TARNAME)/system || mkdir -p $(ROOTDIR)/build/linux/d2000/$(TARNAME)/system
	@test -d $(ROOTDIR)/build/linux/d2000/$(TARNAME)/libs || mkdir -p $(ROOTDIR)/build/linux/d2000/$(TARNAME)/libs
	@test -d $(ROOTDIR)/build/linux/d2000/$(TARNAME)/docs || mkdir -p $(ROOTDIR)/build/linux/d2000/$(TARNAME)/docs
	@test -d $(ROOTDIR)/build/linux/d2000/$(TARNAME)/starlang || mkdir -p $(ROOTDIR)/build/linux/d2000/$(TARNAME)/starlang
	@test -d $(ROOTDIR)/build/linux/d2000/$(TARNAME)/resources || mkdir -p $(ROOTDIR)/build/linux/d2000/$(TARNAME)/resources
	@test -d $(ROOTDIR)/build/linux/d2000/$(TARNAME)/bootstrap || mkdir -p $(ROOTDIR)/build/linux/d2000/$(TARNAME)/bootstrap
#################################################################################
	
cleanall: $(tocleanall)
###########################################################################################
.PHONY:star5gcoreclean
star5gcoreclean:
	make clean -C $(ROOTDIR)
	rm -rf $(ROOTDIR)/target/*

