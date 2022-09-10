TEMPLATE        = lib
#CONFIG         += staticlib
VER_MAJ		    = 0
VER_MIN		    = 1
VER_PAT		    = 0
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=e6eaf32a-e9e7-4c0c-aa56-b83524556f77
QMAKE_CXXFLAGS += -DGP_REFLECTION_STATIC_ADD_TO_MANAGER
DEFINES		   += GP_NETWORK_CORE_LIBRARY
PACKET_NAME     = GpNetworkCore
DIR_LEVEL       = ./../..

include(../../../QtGlobalPro.pri)

#------------------------------ LIBS BEGIN ---------------------------------
os_windows{
}

os_linux{
}
#------------------------------- LIBS END ----------------------------------

SOURCES += \
    IO/Events/GpIOEvent.cpp \
    IO/Events/GpIOEventPoller.cpp \
    IO/Events/GpIOEventPollerCatalog.cpp \
    IO/Events/GpIOEventPollerCfgDesc.cpp \
    IO/Events/GpIOEventPollerEpoll.cpp \
    IO/Events/GpIOEventPollerEpollCfgDesc.cpp \
    IO/Events/GpIOEventPollerEpollFactory.cpp \
    IO/Events/GpIOEventType.cpp \
    IO/Sockets/GpSocket.cpp \
    IO/Sockets/GpSocketAddr.cpp \
    IO/Sockets/GpSocketFlags.cpp \
    IO/Sockets/GpSocketIPv.cpp \
    IO/Sockets/GpSocketProtocol.cpp \
    IO/Sockets/GpSocketTCP.cpp \
    IO/Sockets/GpSocketTask.cpp \
    IO/Sockets/GpTcpServerTask.cpp

HEADERS += \
    GpNetworkCore.hpp \
    GpNetworkCore_global.hpp \
    IO/Events/GpIOEvent.hpp \
    IO/Events/GpIOEventPoller.hpp \
    IO/Events/GpIOEventPollerCatalog.hpp \
    IO/Events/GpIOEventPollerCfgDesc.hpp \
    IO/Events/GpIOEventPollerEpoll.hpp \
    IO/Events/GpIOEventPollerEpollCfgDesc.hpp \
    IO/Events/GpIOEventPollerEpollFactory.hpp \
    IO/Events/GpIOEventPollerFactory.hpp \
    IO/Events/GpIOEventType.hpp \
    IO/Events/GpIOEvents.hpp \
    IO/GpIO.hpp \
    IO/GpIOObjectId.hpp \
    IO/Sockets/GpSocket.hpp \
    IO/Sockets/GpSocketAddr.hpp \
    IO/Sockets/GpSocketFlags.hpp \
    IO/Sockets/GpSocketIPv.hpp \
    IO/Sockets/GpSocketProtocol.hpp \
    IO/Sockets/GpSocketTCP.hpp \
    IO/Sockets/GpSocketTask.hpp \
    IO/Sockets/GpSocketTaskFactory.hpp \
    IO/Sockets/GpSockets.hpp \
    IO/Sockets/GpSockets_global.hpp \
    IO/Sockets/GpTcpServerTask.hpp
