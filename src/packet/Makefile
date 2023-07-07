
include $(USER_DEPTH)/make/makeinclude/mk_pre.mak

USER_LIB = dipcpacket

USER_SRCS_CPP= \
ARPPacket.cpp\
Ethernet.cpp\
IPPacket.cpp\
TCPPacket.cpp\
UDPPacket.cpp\
VlanId.cpp\
EAP.cpp\
RADIUS.cpp\
RADIUSAttribute.cpp\
ICMPPacket.cpp\
checksum.cpp\
DSPacket.cpp\
PacketBase.cpp\
IPacketParser.cpp\
UnknownEthernet.cpp\
UnknownIPPacket.cpp\
UnknownTCPPacket.cpp\
UnknownUDPPacket.cpp\
CLLDP.cpp\
LLDPTLV.cpp\
DiscoveryPacket.cpp\
LinkDataPacket.cpp\
STPPacket.cpp


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



USER_OBJ_DIR=$(TARGET_PATH)/src/packet

include $(USER_DEPTH)/make/makeinclude/mk_post.mak



