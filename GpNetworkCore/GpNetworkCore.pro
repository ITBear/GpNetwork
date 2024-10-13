# ----------- Config -----------
TEMPLATE        = lib
QMAKE_CXXFLAGS += -DGP_REFLECTION_STATIC_ADD_TO_MANAGER
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=e6eaf32a-e9e7-4c0c-aa56-b83524556f77
PACKET_NAME     = GpNetworkCore
DEFINES        += GP_NETWORK_CORE_LIBRARY
_VER_MAJ        = 2
_VER_MIN        = 1
_VER_PAT        = 6
DIR_LEVEL       = ./../..

include($$DIR_LEVEL/../QtGlobalPro.pri)

equals(var_link, "static") {
	CONFIG += staticlib
}

# ----------- Libraries -----------
equals(var_os, "windows") {
	LIBS += -lGpLogCore$$TARGET_POSTFIX
	LIBS += -lGpTasks$$TARGET_POSTFIX
	LIBS += -lGpReflection$$TARGET_POSTFIX
	LIBS += -lGpUtils$$TARGET_POSTFIX

	LIBS += -lws2_32
}

equals(var_os, "linux") {
	LIBS += -lGpLogCore$$TARGET_POSTFIX
	LIBS += -lGpTasks$$TARGET_POSTFIX
	LIBS += -lGpReflection$$TARGET_POSTFIX
	LIBS += -lGpUtils$$TARGET_POSTFIX

	LIBS += -lfmt
}

# ----------- Sources and headers -----------
SOURCES += \
	GpNetworkCoreLib.cpp \
	GpNetworkErrors.cpp \
	GpNetworkUtils.cpp \
	GpNetworkUtilsDns.cpp \
	Pollers/Epoll/GpIOEventPollerEpoll.cpp \
	Pollers/Epoll/GpIOEventPollerEpollCfgDesc.cpp \
	Pollers/Epoll/GpIOEventPollerEpollFactory.cpp \
	Pollers/GpIOEventPoller.cpp \
	Pollers/GpIOEventPollerCatalog.cpp \
	Pollers/GpIOEventPollerCfgDesc.cpp \
	Pollers/GpIOEventType.cpp \
	Pollers/Select/GpIOEventPollerSelect.cpp \
	Pollers/Select/GpIOEventPollerSelectCfgDesc.cpp \
	Pollers/Select/GpIOEventPollerSelectFactory.cpp \
	Sockets/GpSocket.cpp \
	Sockets/GpSocketAddr.cpp \
	Sockets/GpSocketFlags.cpp \
	Sockets/GpSocketIP.cpp \
	Sockets/GpSocketIPv.cpp \
	Sockets/GpSocketProtocol.cpp \
	Sockets/GpSocketTCP.cpp \
	Sockets/GpSocketUDP.cpp \
	Sockets/GpSocketUtils.cpp \
	Tasks/GpSocketsTask.cpp \
	Tasks/GpTcpAcceptServerTask.cpp \
	Tasks/GpTcpClientTask.cpp \
	Tasks/GpTcpServerTask.cpp

HEADERS += \
	GpNetworkCoreLib.hpp \
	GpNetworkCore_global.hpp \
	GpNetworkErrors.hpp \
	GpNetworkUtils.hpp \
	GpNetworkUtilsDns.hpp \
	Pollers/Epoll/GpIOEventPollerEpoll.hpp \
	Pollers/Epoll/GpIOEventPollerEpollCfgDesc.hpp \
	Pollers/Epoll/GpIOEventPollerEpollFactory.hpp \
	Pollers/GpIOEventPoller.hpp \
	Pollers/GpIOEventPollerCatalog.hpp \
	Pollers/GpIOEventPollerCfgDesc.hpp \
	Pollers/GpIOEventPollerFactory.hpp \
	Pollers/GpIOEventType.hpp \
	Pollers/Select/GpIOEventPollerSelect.hpp \
	Pollers/Select/GpIOEventPollerSelectCfgDesc.hpp \
	Pollers/Select/GpIOEventPollerSelectFactory.hpp \
	Sockets/GpSocket.hpp \
	Sockets/GpSocketAddr.hpp \
	Sockets/GpSocketFactory.hpp \
	Sockets/GpSocketFlags.hpp \
	Sockets/GpSocketIP.hpp \
	Sockets/GpSocketIPv.hpp \
	Sockets/GpSocketMessageUDP.hpp \
	Sockets/GpSocketProtocol.hpp \
	Sockets/GpSocketTCP.hpp \
	Sockets/GpSocketUDP.hpp \
	Sockets/GpSocketUtils.hpp \
	Tasks/GpSocketsTask.hpp \
	Tasks/GpTcpAcceptServerTask.hpp \
	Tasks/GpTcpClientTask.hpp \
	Tasks/GpTcpServerTask.hpp \
	Tasks/GpTcpServerTaskFactory.hpp

