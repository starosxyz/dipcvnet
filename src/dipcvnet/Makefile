
include $(USER_DEPTH)/make/makeinclude/mk_pre.mak

USER_LIB = dipcvnet

USER_SRCS_CPP= \
mppp/CDIPCVnetMPPPManager.cpp\
mppp/CDIPCVnetMPPPSession.cpp\
mppp/CDIPCVnetMPPPTapLocal.cpp\
ppp/CDIPCVnetPPPManager.cpp\
ppp/CDIPCVnetPPPSession.cpp\
vos/CDIPCVnetVOSSession.cpp\
vos/CDIPCVnetVOSManager.cpp\
vos/CDIPCVnetVOSPortLocal.cpp\
vos/CDIPCVnetVOSPortExt.cpp\
vswitch/CDIPCVnetSwitchManager.cpp\
vswitch/CDIPCVnetSwitchPortLocal.cpp\
vswitch/CDIPCVnetSwitchSession.cpp\
CDIPCVnetManager.cpp\
CDIPCVnetSessionManager.cpp\
CDIPCVnetTap.cpp\
CDIPCVnetSession.cpp\
CVNetDataChannel.cpp\
CDIPCVOSPort.cpp

USER_INCLDIRS =\
-I$(USER_DEPTH)/src/\
-I$(USER_DEPTH)/src/main/\
-I$(USER_DEPTH)/src/dipcvnet/\
-I$(USER_DEPTH)/src/include/\
-I$(USER_DEPTH)/src/dipcvos/\
-I$(USER_DEPTH)/src/utils\
-I$(USER_DEPTH)/src/vswitch

USER_DEFFLAGS =
USER_LINK_LIBS =
USER_LINK_DLLS =
USER_LINK_FLAGS=



USER_OBJ_DIR=$(TARGET_PATH)/src/dipcvnet

include $(USER_DEPTH)/make/makeinclude/mk_post.mak



