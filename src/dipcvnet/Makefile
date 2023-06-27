include $(USER_DEPTH)/make/makeinclude/mk_pre.mak
USER_BIN = dipcvnet
USER_SRCS_CPP= \
CDIPCVNetConfig.cpp\
CDIPCVNetProcess.cpp\
dipcvnet.cpp
USER_INCLDIRS =\
-I$(USER_DEPTH)/src/\
-I$(USER_DEPTH)/src/vnet/\
-I$(USER_DEPTH)/src/dipcvnet

USER_DEFFLAGS =-DWEBRTC_POSIX -DWEBRTC_LINUX -DUSE_DIPC
USER_LINK_LIBS =
USER_LINK_DLLS =\
-lvnet

USER_LINK_FLAGS= -L$(USER_DEPTH)/target\
-L/opt/staros.xyz/dipc/dlls
USER_OBJ_DIR=$(TARGET_PATH)/src/dipcvnet
include $(USER_DEPTH)/make/makeinclude/mk_post.mak
