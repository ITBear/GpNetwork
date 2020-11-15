TEMPLATE        = lib
#CONFIG         += staticlib
VER_MAJ		    = 0
VER_MIN		    = 1
VER_PAT		    = 0
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=e6eaf32a-e9e7-4c0c-aa56-b83524556f77
QMAKE_CXXFLAGS += -DGP_TYPE_SYSTEM_STATIC_ADD_TO_MANAGER
DEFINES		   += GPNETWORK_LIBRARY
PACKET_NAME     = GpNetwork
DIR_LEVEL       = ./..

include(../../QtGlobalPro.pri)

#------------------------------ LIBS BEGIN ---------------------------------
os_windows{
	GP_CORE_LIB_V		= 2
}

os_linux{
}

LIBS += -lGpCore2$$TARGET_POSTFIX$$GP_CORE_LIB_V
LIBS += -lpistache
LIBS += -lcurl
LIBS += -lllhttp

#------------------------------ LIBS END ---------------------------------

SOURCES += \
    GpNetwork.cpp \
    HTTP/GpHttpCacheControl.cpp \
    HTTP/GpHttpCharset.cpp \
    HTTP/GpHttpClientCurl.cpp \
    HTTP/GpHttpConnectionFlag.cpp \
    HTTP/GpHttpContentType.cpp \
    HTTP/GpHttpException.cpp \
    HTTP/GpHttpHeaderType.cpp \
    HTTP/GpHttpHeaderValue.cpp \
    HTTP/GpHttpHeaders.cpp \
    HTTP/GpHttpRequest.cpp \
    HTTP/GpHttpRequestDoneEvent.cpp \
    HTTP/GpHttpRequestHandler.cpp \
    HTTP/GpHttpRequestRouteTable.cpp \
    HTTP/GpHttpRequestRouterStatic.cpp \
    HTTP/GpHttpRequestSerializer.cpp \
    HTTP/GpHttpRequestTask.cpp \
    HTTP/GpHttpRequestType.cpp \
    HTTP/GpHttpResponse.cpp \
    HTTP/GpHttpResponseCode.cpp \
    HTTP/GpHttpResponseSerializer.cpp \
    HTTP/GpHttpServer.cpp \
    HTTP/GpHttpServerFactory.cpp \
    HTTP/GpHttpServerSocketTaskFactory.cpp \
    HTTP/GpHttpVersion.cpp \
    HTTP/Node/GpHttpServerNode.cpp \
    HTTP/Node/GpHttpServerNodeFactory.cpp \
    HTTP/Node/GpHttpServerNodeSocketTask.cpp \
    HTTP/Node/GpHttpServerNodeSocketTaskFactory.cpp \
    HTTP/Pistache/GpHttpRouteHandlerPistache.cpp \
    HTTP/Pistache/GpHttpServerPistache.cpp \
    HTTP/Pistache/GpHttpServerPistacheFactory.cpp \
    IO/Events/GpIOEvent.cpp \
    IO/Events/GpIOEventPoller.cpp \
    IO/Events/GpIOEventPollerEpoll.cpp \
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
    GpNetworkConfig.hpp \
    GpNetwork_global.hpp \
    GpNetwork.hpp \
    HTTP/GpHTTP.hpp \
    HTTP/GpHttpCacheControl.hpp \
    HTTP/GpHttpCharset.hpp \
    HTTP/GpHttpClientCurl.hpp \
    HTTP/GpHttpConnectionFlag.hpp \
    HTTP/GpHttpContentType.hpp \
    HTTP/GpHttpException.hpp \
    HTTP/GpHttpHeaderType.hpp \
    HTTP/GpHttpHeaderValue.hpp \
    HTTP/GpHttpHeaders.hpp \
    HTTP/GpHttpRequest.hpp \
    HTTP/GpHttpRequestDoneEvent.hpp \
    HTTP/GpHttpRequestHandler.hpp \
    HTTP/GpHttpRequestHandlerFactory.hpp \
    HTTP/GpHttpRequestRouteTable.hpp \
    HTTP/GpHttpRequestRouter.hpp \
    HTTP/GpHttpRequestRouterStatic.hpp \
    HTTP/GpHttpRequestSerializer.hpp \
    HTTP/GpHttpRequestTask.hpp \
    HTTP/GpHttpRequestType.hpp \
    HTTP/GpHttpResponse.hpp \
    HTTP/GpHttpResponseCode.hpp \
    HTTP/GpHttpResponseSerializer.hpp \
    HTTP/GpHttpServer.hpp \
    HTTP/GpHttpServerFactory.hpp \
    HTTP/GpHttpServerSocketTaskFactory.hpp \
    HTTP/GpHttpVersion.hpp \
    HTTP/Node/GpHttpServerNode.hpp \
    HTTP/Node/GpHttpServerNodeFactory.hpp \
    HTTP/Node/GpHttpServerNodeSocketTask.hpp \
    HTTP/Node/GpHttpServerNodeSocketTaskFactory.hpp \
    HTTP/Pistache/GpHttpRouteHandlerPistache.hpp \
    HTTP/Pistache/GpHttpServerPistache.hpp \
    HTTP/Pistache/GpHttpServerPistacheFactory.hpp \
    IO/Events/GpIOEvent.hpp \
    IO/Events/GpIOEventPoller.hpp \
    IO/Events/GpIOEventPollerEpoll.hpp \
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
