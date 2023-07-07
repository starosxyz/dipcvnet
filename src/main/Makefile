include $(USER_DEPTH)/make/makeinclude/mk_pre.mak
USER_BIN = dipcvnet
USER_SRCS_CPP= \
CDIPCVNetConfig.cpp\
CDIPCVNetProcess.cpp\
main.cpp
USER_INCLDIRS =\
-I$(USER_DEPTH)/src/\
-I$(USER_DEPTH)/src/main/\
-I$(USER_DEPTH)/src/dipcvnet/\
-I$(USER_DEPTH)/src/include/\
-I$(USER_DEPTH)/src/dipcvos/\
-I$(USER_DEPTH)/src/utils\
-I$(USER_DEPTH)/src/vswitch


USER_DEFFLAGS =-DWEBRTC_POSIX -DWEBRTC_LINUX -DUSE_DIPC
USER_LINK_LIBS =
USER_LINK_DLLS =\
-ldipcvnet\
-lvnetpolicy\
-lvosthread\
-ldwstack\
-lvnetprobe\
-ldipcvswitch\
-ldipcpacket\
-ldipcvnetutils

USER_LINK_FLAGS= -L$(USER_DEPTH)/target\
-L/opt/staros.xyz/dipc/dlls
USER_OBJ_DIR=$(TARGET_PATH)/src/main
include $(USER_DEPTH)/make/makeinclude/mk_post.mak
