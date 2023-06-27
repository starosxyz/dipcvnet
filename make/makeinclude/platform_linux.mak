#
# Copyright (C) 2016-2020 南京北星极网络科技有限公司
#
debug ?= 1
optimize ?= 1
#profile ?= 1
exceptions ?= 1
threads ?= 0
static ?=1
build_dipc_only ?=1
OBJEXT ?= o
LIBEXT ?= a
DLLEXT ?= so
CC_OUTPUT_FLAG ?= -o
DLL_OUTPUT_FLAG ?= -o
LINK_OUTPUT_FLAG ?= -o
ARCH:=$(shell uname -m)

ifeq (,$(debug))
  debug = 1
endif
ifeq (,$(optimize))
  optimize = 0
endif

DEFFLAGS += -DCAW_LINUX -DCAW_UNIX -DWEBRTC_LINUX -DWEBRTC_POSIX
ifeq (,$(_MACHINE))
	DEFFLAGS += -D_CPU_TYPE=$(_CPU_TYPE) -DOS_LINUX_PC -DCPU=$(_CPU)
	ASFLAGS += -D_CPU_TYPE=$(_CPU_TYPE) -DOS_LINUX_PC -DCPU=$(_CPU)
else
	DEFFLAGS += -$(_MACHINE) -D_CPU_TYPE=$(_CPU_TYPE) -DOS_LINUX_PC -DCPU=$(_CPU)
	ASFLAGS += -$(_MACHINE) -D_CPU_TYPE=$(_CPU_TYPE) -DOS_LINUX_PC -DCPU=$(_CPU)
endif

ifeq (1,$(debug))
	DEFFLAGS += -DDEBUG_VERSION
	DEFFLAGS += -DCAW_DEBUG
	DEFFLAGS +=-fdiagnostics-color=always -D_FILE_OFFSET_BITS=64 -Winvalid-pch -g -Wextra -Wlogical-op -Werror=missing-include-dirs -Werror=pointer-arith -Werror=init-self -Wfloat-equal -Wsuggest-attribute=noreturn -Werror=return-type -Wredundant-decls -Wimplicit-fallthrough=5 -Wendif-labels -Wstrict-aliasing=3 -Wwrite-strings -Werror=overflow -Werror=shift-count-overflow -Werror=shift-overflow=2 -Wdate-time -Wunused -Wduplicated-branches -Wmisleading-indentation -Wno-sign-compare -Wno-unused-parameter -ffast-math -fdiagnostics-show-option -fstack-protector -fstack-protector-strong --param=ssp-buffer-size=4 -fPIC -pthread
	DEFFLAGS +=-pipe -Wall -fsigned-char -g -fno-defer-pop
	ASFLAGS +=-pipe -Wall -fsigned-char -g -P -x assembler-with-cpp -fno-defer-pop
else
	DEFFLAGS += -DRELEASE_VERSION
	DEFFLAGS += -DCAW_DISABLE_TRACE
	DEFFLAGS += -DXOS_DISABLE_LOG
	DEFFLAGS += -DCAW_RELEASE
	DEFFLAGS +=-fdiagnostics-color=always -D_FILE_OFFSET_BITS=64 -Winvalid-pch -Wextra -Wlogical-op -Werror=missing-include-dirs -Werror=pointer-arith -Werror=init-self -Wfloat-equal -Wsuggest-attribute=noreturn  -Werror=return-type -Wredundant-decls -Wimplicit-fallthrough=5 -Wendif-labels -Wstrict-aliasing=3 -Wwrite-strings -Werror=overflow -Werror=shift-count-overflow -Werror=shift-overflow=2 -Wdate-time -Wunused -Wduplicated-branches -Wmisleading-indentation -Wno-sign-compare -Wno-unused-parameter -ffast-math -fdiagnostics-show-option -fstack-protector -fstack-protector-strong --param=ssp-buffer-size=4 -fPIC -pthread
	DEFFLAGS +=-pipe -Wall -fno-strict-aliasing -fno-schedule-insns2 -fsigned-char -fno-omit-frame-pointer -fno-defer-pop
	ASFLAGS +=-pipe -Wall -fno-strict-aliasing -fno-schedule-insns2 -fsigned-char -fno-omit-frame-pointer -P -x assembler-with-cpp -fno-defer-pop
endif


ifeq ($(threads),1)
  DEFFLAGS += -D_REENTRANT
endif # threads

ifeq ($(static),0)
DEFFLAGS +=-fPIC 
ASFLAGS +=-fPIC
endif

ifeq (1,$(optimize))
	DEFFLAGS +=-O3
	ASFLAGS +=-O3
else
	DEFFLAGS +=-O0
	ASFLAGS +=-O0
endif

#-I/opt/staros.xyz/fstack/include
#

INCLDIRS +=$(USER_INCLDIRS)\
-I/opt/staros.xyz/dipc/include\
-I/opt/staros.xyz/dipc/include/libyaml\
-I/opt/staros.xyz/dipc/include/curl\
-I/opt/staros.xyz/dipc/include/sjs\
-I/opt/staros.xyz/dipc/include/python\
-I/opt/staros.xyz/bxjstack/include\
-I/opt/staros.xyz/bxjstack/include/vswitch

DLLSINCLUDE +=

##-L/opt/staros.xyz/fstack/libs\

USER_LINK_FLAGS +=\
-L/opt/staros.xyz/dipc/dlls\
-L/opt/staros.xyz/bxjstack/libs
ASFLAGS +=

##-I/opt/staros.xyz/fstack/include			
DEFFLAGS += $(USER_DEFFLAGS) 
LIBS += $(USER_LINK_LIBS)
DLLS += $(DLLSINCLUDE) $(USER_LINK_DLLS)
DEFFLAGS +=$(USER_DEFFLAGS) -DUSE_XOS
DEFFLAGS += -DDIPC_FULL -DALL_IN_ONE

PKGCONF ?= pkg-config

DEFFLAGS += $(USER_DEFFLAGS) 


DLLPLATEX +=\
-lfshmipc\
-lipstack\
-lipstacktp\
-lscrypto\
-lsslace\
-lsslptcp\
-lpseudotcp\
-lsslutils\
-lstarhttps\
-lstarssh2\
-lpkixssh\
-lutilsex

DLLSINCLUDE +=\
-lbxjstack\
-ldipcstarlang\
-ldipcclient\
-ldipc\
-ldipcservice\
-ldipcutils\
-lxmlengine\
$(DLLPLATEX)\
-lpyxmlengine\
-lstardb\
-lsnappy\
-lshttp\
-lwface\
-lfipc\
-lsoapparser\
-lstarhttp2\
-lstarsctp\
-lstarnetconf\
-lusrsctp\
-lnghttp2\
-lleveldb\
-lstarbase\
-lsqlitedb\
-lrtc\
-lglog\
-lffi\
-lvos\
-llibsec\
-lxmlpython3100\
-lsjs\
-lexpat\
-lACE\
-ljson\
-lcjson

DLLSINCLUDE +=-lzlib\
-lopenssl\
-lm\
-lrt\
-ldl\
-lutil\
-lpthread\
-latomic

CPPFLAGS += -Wno-deprecated -std=c++17 $(DEFFLAGS) $(USER_C++_DEFFLAG) $(INCLDIRS)
CFLAGS += -Werror=implicit-function-declaration -Werror=incompatible-pointer-types -Wstrict-prototypes -Wnested-externs $(DEFFLAGS) $(USER_C_DEFFLAGS) $(INCLDIRS) 
LDFLAGS += $(USER_LINK_DLLS) $(DLLSINCLUDE)
DEFLINKFLAGS +=
LDFLAGS+= $(DEFLINKFLAGS)
#ASFLAGS +=$(DEFFLAGS)
NASMFLAG += $(ASFLAGS) $(INCLDIRS)

LINK_FLAGS += $(USER_LINK_FLAGS)

COMPILE.c  = $(CC) $(CFLAGS)
COMPILE.cc  = $(CXX) $(CPPFLAGS)
COMPILE.cpp = $(CXX) $(CPPFLAGS)
COMPILE.cxx = $(CXX) $(CPPFLAGS)
COMPILE.as = $(CC) $(NASMFLAG)


LINK.c  = $(CC) $(LINK_FLAGS) $(LDFLAGS) 
LINK.cc  = $(CXX) $(LINK_FLAGS) $(LDFLAGS) 
LINK.cpp = $(CXX) $(LINK_FLAGS) $(LDFLAGS)
LINK.cxx = $(CXX) $(LINK_FLAGS) $(LDFLAGS)

NULL_STDERR = 2>$(/dev/null) || true

