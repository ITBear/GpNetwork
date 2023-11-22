TEMPLATE        = lib
#CONFIG         += staticlib
VER_MAJ		    = 2
VER_MIN		    = 1
VER_PAT		    = 4
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=e6eaf32a-e9e7-4c0c-aa56-b83524556f77
QMAKE_CXXFLAGS += -DGP_REFLECTION_STATIC_ADD_TO_MANAGER
PACKET_NAME     = GpNetworkCore
DIR_LEVEL       = ./../..

DEFINES		   += GP_NETWORK_CORE_LIBRARY
DEFINES        += "GP_CURRENT_LIB_VER_MAJ=\\\"$$VER_MAJ\\\""
DEFINES        += "GP_CURRENT_LIB_VER_MIN=\\\"$$VER_MIN\\\""
DEFINES        += "GP_CURRENT_LIB_VER_PAT=\\\"$$VER_PAT\\\""
DEFINES        += "GP_CURRENT_LIB_PACKET_NAME=\\\"$$PACKET_NAME\\\""

include(../../../QtGlobalPro.pri)

#------------------------------ LIBS BEGIN ---------------------------------
os_windows{
}

os_linux{
}
#------------------------------- LIBS END ----------------------------------

SOURCES += \
	GpNetworkCore.cpp \
	Pollers/Epoll/GpIOEventPollerEpoll.cpp \
	Pollers/Epoll/GpIOEventPollerEpollCfgDesc.cpp \
	Pollers/Epoll/GpIOEventPollerEpollFactory.cpp \
	Pollers/GpIOEventPoller.cpp \
	Pollers/GpIOEventPollerCatalog.cpp \
	Pollers/GpIOEventPollerCfgDesc.cpp \
	Pollers/GpIOEventType.cpp \
	Sockets/GpSocket.cpp \
	Sockets/GpSocketFactoryTCP.cpp \
	Sockets/GpSocketFlags.cpp \
	Sockets/GpSocketIPv.cpp \
	Sockets/GpSocketProtocol.cpp \
	Sockets/GpSocketTCP.cpp \
	Sockets/GpSocketUDP.cpp \
	Tasks/GpSocketTask.cpp \
	Tasks/GpTcpAcceptServerTask.cpp

HEADERS += \
	GpIOObjectId.hpp \
	GpNetworkCore.hpp \
	GpNetworkCore_global.hpp \
	Pollers/Epoll/GpIOEventPollerEpoll.hpp \
	Pollers/Epoll/GpIOEventPollerEpollCfgDesc.hpp \
	Pollers/Epoll/GpIOEventPollerEpollFactory.hpp \
	Pollers/GpIOEventPoller.hpp \
	Pollers/GpIOEventPollerCatalog.hpp \
	Pollers/GpIOEventPollerCfgDesc.hpp \
	Pollers/GpIOEventPollerFactory.hpp \
	Pollers/GpIOEventType.hpp \
	Sockets/GpSocket.hpp \
	Sockets/GpSocketAddr.hpp \
	Sockets/GpSocketFactory.hpp \
	Sockets/GpSocketFactoryTCP.hpp \
	Sockets/GpSocketFlags.hpp \
	Sockets/GpSocketIPv.hpp \
	Sockets/GpSocketProtocol.hpp \
	Sockets/GpSocketTCP.hpp \
	Sockets/GpSocketUDP.hpp \
	Sockets/GpSockets_global.hpp \
	Tasks/GpSocketTask.hpp \
	Tasks/GpSocketTaskFactory.hpp \
	Tasks/GpTcpAcceptServerTask.hpp

