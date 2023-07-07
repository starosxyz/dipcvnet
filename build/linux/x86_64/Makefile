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
TARNAME=dipcvnet_release_$(os)_x86_64_v$(VERSION)
else
TARNAME=dipcvnet_debug_$(os)_x86_64_v$(VERSION)
endif
INSTALLNAME=install.sh

export STARDLLBUILD=$(ROOTDIR)/build/linux/x86_64/$(TARNAME)

ifeq (1,$(build_star5gcore))
buidlall +=star5gcore
tocleanall +=star5gcoreclean
endif


all: check_dir $(buidlall) 
	echo $(buidlall)
	cp -rf $(ROOTDIR)/starlang/* $(STARDLLBUILD)/starlang/
	cp -rf $(ROOTDIR)/conf/* $(STARDLLBUILD)/conf/
	cp -rf /opt/staros.xyz/vos/libs/* $(STARDLLBUILD)/libs/
	test -d /opt/staros.xyz/rte/libs/ && cp -rf /opt/staros.xyz/rte/libs/*.so $(STARDLLBUILD)/libs/
	mv $(TARNAME) tmp
	tar zcvf $(TARNAME).tar.gz -C $(ROOTDIR)/build/linux/x86_64/ tmp
	cat $(INSTALLNAME) $(TARNAME).tar.gz > $(TARNAME).bin 
	@echo "build" $(buidlall)
	rm -rf tmp
	chmod a+x $(TARNAME).bin
.PHONY:star5gcore
star5gcore:check_dir
	make -C $(STAR5GCORE_ROOT)
	cp -Lf $(ROOTDIR)/bin/* $(STARDLLBUILD)/bin/
	cp -rf /opt/staros.xyz/vos/bin/* $(STARDLLBUILD)/bin/fstackbin/
ifeq (0,$(static))
	cp -Lf $(ROOTDIR)/target/*.so $(STARDLLBUILD)/libs/
else
	cp -Lf $(ROOTDIR)/target/*.a $(STARDLLBUILD)/libs/
endif
check_dir: 
	@test -d $(STARDLLBUILD) || mkdir -p $(STARDLLBUILD)
	@test -d $(STARDLLBUILD)/bin || mkdir -p $(STARDLLBUILD)/bin
	@test -d $(STARDLLBUILD)/bin/fstackbin || mkdir -p $(STARDLLBUILD)/bin/fstackbin
	@test -d $(STARDLLBUILD)/conf || mkdir -p $(STARDLLBUILD)/conf
	@test -d $(STARDLLBUILD)/logs || mkdir -p $(STARDLLBUILD)/logs
	@test -d $(STARDLLBUILD)/system || mkdir -p $(STARDLLBUILD)/system
	@test -d $(STARDLLBUILD)/libs || mkdir -p $(STARDLLBUILD)/libs
	@test -d $(STARDLLBUILD)/docs || mkdir -p $(STARDLLBUILD)/docs
	@test -d $(STARDLLBUILD)/starlang || mkdir -p $(STARDLLBUILD)/starlang
	@test -d $(STARDLLBUILD)/resources || mkdir -p $(STARDLLBUILD)/resources
	@test -d $(STARDLLBUILD)/bootstrap || mkdir -p $(STARDLLBUILD)/bootstrap
#################################################################################
	
cleanall: $(tocleanall)
###########################################################################################
.PHONY:star5gcoreclean
star5gcoreclean:
	make clean -C $(ROOTDIR)
	rm -rf $(ROOTDIR)/target/*

