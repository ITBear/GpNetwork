TEMPLATE        = lib
#CONFIG         += staticlib
VER_MAJ		    = 0
VER_MIN		    = 1
VER_PAT		    = 0
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=e6eaf32a-e9e7-4c0c-aa56-b83524556f77
QMAKE_CXXFLAGS += -DGP_TYPE_SYSTEM_STATIC_ADD_TO_MANAGER
DEFINES		   += GPNETWORK_LIBRARY
PACKET_NAME     = GpNetwork
DIR_LEVEL       = .

include(../QtGlobalPro.pri)

#------------------------------ LIBS BEGIN ---------------------------------
os_windows{
	GP_CORE_LIB_V		= 2
}

os_linux{
}

LIBS += -lGpCore2$$TARGET_POSTFIX$$GP_CORE_LIB_V
LIBS += -lpistache
LIBS += -lcurl
#------------------------------ LIBS END ---------------------------------

SOURCES += \
    GpNetwork.cpp \
    HTTP/GpHttpClient.cpp \
    HTTP/GpHttpRequest.cpp \
    HTTP/GpHttpResponse.cpp \
    HTTP/GpHttpResponseCode.cpp \
    HTTP/GpHttpServer.cpp \
    HTTP/Pistache/GpHttpRouteHandlerPistache.cpp \
    HTTP/Pistache/GpHttpServerFactoryPistache.cpp \
    HTTP/Pistache/GpHttpServerPistache.cpp \
    IO/Events/GpIOEvent.cpp \
    IO/Events/GpIOEventPoller.cpp \
    IO/Events/GpIOEventPollerEpoll.cpp \
    IO/Events/GpIOEventPollerTask.cpp \
    IO/Sockets/GpSocket.cpp \
    IO/Sockets/GpSocketAddr.cpp \
    IO/Sockets/GpSocketFlags.cpp \
    IO/Sockets/GpSocketIPv.cpp \
    IO/Sockets/GpSocketProtocol.cpp \
    IO/Sockets/GpSocketTCP.cpp

HEADERS += \
    GpNetworkConfig.hpp \
    GpNetwork_global.hpp \
    GpNetwork.hpp \
    HTTP/GpHTTP.hpp \
    HTTP/GpHttpClient.hpp \
    HTTP/GpHttpRequest.hpp \
    HTTP/GpHttpRequestHandler.hpp \
    HTTP/GpHttpRequestHandlerFactory.hpp \
    HTTP/GpHttpResponse.hpp \
    HTTP/GpHttpResponseCode.hpp \
    HTTP/GpHttpServer.hpp \
    HTTP/GpHttpServerFactory.hpp \
    HTTP/Pistache/GpHttpRouteHandlerPistache.hpp \
    HTTP/Pistache/GpHttpServerFactoryPistache.hpp \
    HTTP/Pistache/GpHttpServerPistache.hpp \
    IO/Events/GpIOEvent.hpp \
    IO/Events/GpIOEventPoller.hpp \
    IO/Events/GpIOEventPollerEpoll.hpp \
    IO/Events/GpIOEventPollerTask.hpp \
    IO/Events/GpIOEvents.hpp \
    IO/GpIO.hpp \
    IO/Sockets/GpSocket.hpp \
    IO/Sockets/GpSocketAddr.hpp \
    IO/Sockets/GpSocketFlags.hpp \
    IO/Sockets/GpSocketIPv.hpp \
    IO/Sockets/GpSocketProtocol.hpp \
    IO/Sockets/GpSocketTCP.hpp \
    IO/Sockets/GpSockets.hpp \
    IO/Sockets/GpSockets_global.hpp
