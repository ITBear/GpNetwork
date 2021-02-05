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
LIBS += -lcurl
LIBS += -lllhttp

#------------------------------ LIBS END ---------------------------------

SOURCES += \
	GpNetwork.cpp \
	L7/Common/Enums/GpCharset.cpp \
	L7/Common/Enums/GpContentType.cpp \
	L7/Common/Headers/GpProtoHeaderValue.cpp \
	L7/Common/Headers/GpProtoHeaders.cpp \
	L7/EMail/GpEmail.cpp \
	L7/EMail/GpEmailAddr.cpp \
	L7/EMail/GpEmailBuilder.cpp \
	L7/EMail/GpEmailEncoder.cpp \
	L7/EMail/GpEmailHeaderType.cpp \
	L7/EMail/GpEmailHeaders.cpp \
	L7/EMail/GpEmailPart.cpp \
	L7/HTTP/GpHttpCacheControl.cpp \
	L7/HTTP/GpHttpClientCurl.cpp \
	L7/HTTP/GpHttpClientCurlFactory.cpp \
	L7/HTTP/GpHttpClientGuard.cpp \
	L7/HTTP/GpHttpClientPool.cpp \
	L7/HTTP/GpHttpClientPoolCatalog.cpp \
	L7/HTTP/GpHttpConnectionFlag.cpp \
	L7/HTTP/GpHttpException.cpp \
	L7/HTTP/GpHttpHeaderType.cpp \
	L7/HTTP/GpHttpHeaders.cpp \
	L7/HTTP/GpHttpRequest.cpp \
	L7/HTTP/GpHttpRequestDoneEvent.cpp \
	L7/HTTP/GpHttpRequestHandler.cpp \
	L7/HTTP/GpHttpRequestRouteTable.cpp \
	L7/HTTP/GpHttpRequestRouterStatic.cpp \
	L7/HTTP/GpHttpRequestSerializer.cpp \
	L7/HTTP/GpHttpRequestTask.cpp \
	L7/HTTP/GpHttpRequestType.cpp \
	L7/HTTP/GpHttpResponse.cpp \
	L7/HTTP/GpHttpResponseCode.cpp \
	L7/HTTP/GpHttpResponseSerializer.cpp \
	L7/HTTP/GpHttpServer.cpp \
	L7/HTTP/GpHttpServerFactory.cpp \
	L7/HTTP/GpHttpServerSocketTaskFactory.cpp \
	L7/HTTP/GpHttpVersion.cpp \
	L7/HTTP/Node/GpHttpServerNode.cpp \
	L7/HTTP/Node/GpHttpServerNodeFactory.cpp \
	L7/HTTP/Node/GpHttpServerNodeSocketTask.cpp \
	L7/HTTP/Node/GpHttpServerNodeSocketTaskFactory.cpp \
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
	IO/Sockets/GpTcpServerTask.cpp \
	L7/EMail/SMTP/GpSmtpClient.cpp \
	L7/EMail/SMTP/GpSmtpClientCurl.cpp \
	L7/EMail/SMTP/GpSmtpClientCurlFactory.cpp \
	L7/EMail/SMTP/GpSmtpClientGuard.cpp \
	L7/EMail/SMTP/GpSmtpClientPool.cpp \
	L7/EMail/SMTP/GpSmtpClientPoolCatalog.cpp

HEADERS += \
	GpNetwork.hpp \
	GpNetworkConfig.hpp \
	GpNetwork_global.hpp \
	L7/Common/Enums/GpCharset.hpp \
	L7/Common/Enums/GpContentType.hpp \
	L7/Common/Enums/GpEnums.hpp \
	L7/Common/GpCommon.hpp \
	L7/Common/Headers/GpHeaders.hpp \
	L7/Common/Headers/GpProtoHeaderValue.hpp \
	L7/Common/Headers/GpProtoHeaders.hpp \
	L7/EMail/GpEmail.hpp \
	L7/EMail/GpEmailAddr.hpp \
	L7/EMail/GpEmailBuilder.hpp \
	L7/EMail/GpEmailEncoder.hpp \
	L7/EMail/GpEmailHeaderType.hpp \
	L7/EMail/GpEmailHeaders.hpp \
	L7/EMail/GpEmailPart.hpp \
	L7/EMail/GpEmails.hpp \
	L7/EMail/IMAP/GpIMAP.hpp \
	L7/GpNetworkL7.hpp \
	L7/HTTP/GpHTTP.hpp \
	L7/HTTP/GpHttpCacheControl.hpp \
	L7/HTTP/GpHttpClient.hpp \
	L7/HTTP/GpHttpClientCurl.hpp \
	L7/HTTP/GpHttpClientCurlFactory.hpp \
	L7/HTTP/GpHttpClientFactory.hpp \
	L7/HTTP/GpHttpClientGuard.hpp \
	L7/HTTP/GpHttpClientPool.hpp \
	L7/HTTP/GpHttpClientPoolCatalog.hpp \
	L7/HTTP/GpHttpConnectionFlag.hpp \
	L7/HTTP/GpHttpException.hpp \
	L7/HTTP/GpHttpHeaderType.hpp \
	L7/HTTP/GpHttpHeaders.hpp \
	L7/HTTP/GpHttpRequest.hpp \
	L7/HTTP/GpHttpRequestDoneEvent.hpp \
	L7/HTTP/GpHttpRequestHandler.hpp \
	L7/HTTP/GpHttpRequestHandlerFactory.hpp \
	L7/HTTP/GpHttpRequestRouteTable.hpp \
	L7/HTTP/GpHttpRequestRouter.hpp \
	L7/HTTP/GpHttpRequestRouterStatic.hpp \
	L7/HTTP/GpHttpRequestSerializer.hpp \
	L7/HTTP/GpHttpRequestTask.hpp \
	L7/HTTP/GpHttpRequestType.hpp \
	L7/HTTP/GpHttpResponse.hpp \
	L7/HTTP/GpHttpResponseCode.hpp \
	L7/HTTP/GpHttpResponseSerializer.hpp \
	L7/HTTP/GpHttpServer.hpp \
	L7/HTTP/GpHttpServerFactory.hpp \
	L7/HTTP/GpHttpServerSocketTaskFactory.hpp \
	L7/HTTP/GpHttpVersion.hpp \
	L7/HTTP/Node/GpHttpServerNode.hpp \
	L7/HTTP/Node/GpHttpServerNodeFactory.hpp \
	L7/HTTP/Node/GpHttpServerNodeSocketTask.hpp \
	L7/HTTP/Node/GpHttpServerNodeSocketTaskFactory.hpp \
	IO/Events/GpIOEvent.hpp \
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
	IO/Sockets/GpTcpServerTask.hpp \
	IO/Events/GpIOEventPoller.hpp \
	L7/EMail/SMTP/GpSMTP.hpp \
	L7/EMail/SMTP/GpSmtpClient.hpp \
	L7/EMail/SMTP/GpSmtpClientCurl.hpp \
	L7/EMail/SMTP/GpSmtpClientCurlFactory.hpp \
	L7/EMail/SMTP/GpSmtpClientFactory.hpp \
	L7/EMail/SMTP/GpSmtpClientGuard.hpp \
	L7/EMail/SMTP/GpSmtpClientPool.hpp \
	L7/EMail/SMTP/GpSmtpClientPoolCatalog.hpp

