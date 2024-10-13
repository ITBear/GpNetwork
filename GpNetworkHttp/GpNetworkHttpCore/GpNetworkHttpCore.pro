# ----------- Config -----------
TEMPLATE        = lib
QMAKE_CXXFLAGS += -DGP_REFLECTION_STATIC_ADD_TO_MANAGER
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=17cab5e2-7e0b-4d44-8b69-deb8c902ffd8
PACKET_NAME     = GpNetworkHttpCore
DEFINES        += GP_NETWORK_HTTP_CORE_LIBRARY
_VER_MAJ        = 2
_VER_MIN        = 1
_VER_PAT        = 6
DIR_LEVEL       = ./../../../

include($$DIR_LEVEL/../QtGlobalPro.pri)

equals(var_link, "static") {
	CONFIG += staticlib
}

# ----------- Libraries -----------
equals(var_os, "windows") {
	LIBS += -lGpNetworkCore$$TARGET_POSTFIX
	LIBS += -lGpLogCore$$TARGET_POSTFIX
	LIBS += -lGpTasks$$TARGET_POSTFIX
	LIBS += -lGpReflection$$TARGET_POSTFIX
	LIBS += -lGpUtils$$TARGET_POSTFIX

	LIBS += -lllhttp$$TARGET_POSTFIX
	LIBS += -lws2_32
}

equals(var_os, "linux") {
	LIBS += -lGpNetworkCore$$TARGET_POSTFIX
	LIBS += -lGpLogCore$$TARGET_POSTFIX
	LIBS += -lGpTasks$$TARGET_POSTFIX
	LIBS += -lGpReflection$$TARGET_POSTFIX
	LIBS += -lGpUtils$$TARGET_POSTFIX

	LIBS += -lllhttp$$TARGET_POSTFIX
	LIBS += -lfmt
}

# ----------- Sources and headers -----------
SOURCES += \
    Body/GpHttpBodyPayload.cpp \
    Body/GpHttpBodyPayloadFixed.cpp \
    Body/GpHttpBodyPayloadType.cpp \
    Client/GpHttpClient.cpp \
    Client/GpHttpClientGuard.cpp \
    Client/GpHttpClientPool.cpp \
    Client/GpHttpClientPoolCatalog.cpp \
    Client/GpHttpClientRequestTask.cpp \
    Enums/GpHttpCacheControl.cpp \
    Enums/GpHttpCharset.cpp \
    Enums/GpHttpConnectionFlag.cpp \
    Enums/GpHttpContentType.cpp \
    Enums/GpHttpHeaderType.cpp \
    Enums/GpHttpRequestType.cpp \
    Enums/GpHttpResponseCode.cpp \
    Enums/GpHttpVersion.cpp \
    Exceptions/GpHttpException.cpp \
    GpNetworkHttpCoreLib.cpp \
    Headers/GpHttpHeaders.cpp \
    Headers/GpProtoHeaderValue.cpp \
    Headers/GpProtoHeadersMap.cpp \
    RequestHandlers/GpHttpRequestHandler.cpp \
    Routers/RulesRouter/GpHttpRouterRule.cpp \
    Routers/TableRouter/GpHttpRouteTableStatic.cpp \
    Routers/TableRouter/GpHttpTableRouter.cpp \
    RqRs/GpHttpParser.cpp \
    RqRs/GpHttpRequest.cpp \
    RqRs/GpHttpRequestNoBodyDesc.cpp \
    RqRs/GpHttpResponse.cpp \
    RqRs/GpHttpResponseNoBodyDesc.cpp \
    Server/GpHttpServer.cpp \
    Server/GpHttpServerCfgDesc.cpp \
    Server/GpHttpServerRequestTask.cpp \
    Server/GpHttpServerRequestTaskFactory.cpp \
    Url/GpUrl.cpp \
    Url/GpUrlAuthority.cpp \
    Url/GpUrlQuery.cpp \
    Url/GpUrlReflection.cpp


HEADERS += \
    Body/GpHttpBodyPayload.hpp \
    Body/GpHttpBodyPayloadFixed.hpp \
    Body/GpHttpBodyPayloadType.hpp \
    Client/GpHttpClient.hpp \
    Client/GpHttpClientGuard.hpp \
    Client/GpHttpClientPool.hpp \
    Client/GpHttpClientPoolCatalog.hpp \
    Client/GpHttpClientRequestTask.hpp \
    Enums/GpHttpCacheControl.hpp \
    Enums/GpHttpCharset.hpp \
    Enums/GpHttpConnectionFlag.hpp \
    Enums/GpHttpContentType.hpp \
    Enums/GpHttpHeaderType.hpp \
    Enums/GpHttpRequestType.hpp \
    Enums/GpHttpResponseCode.hpp \
    Enums/GpHttpVersion.hpp \
    Exceptions/GpHttpException.hpp \
    GpNetworkHttpCoreLib.hpp \
    GpNetworkHttpCore_global.hpp \
    Headers/GpHttpHeaders.hpp \
    Headers/GpProtoHeaderValue.hpp \
    Headers/GpProtoHeadersMap.hpp \
    RequestHandlers/GpHttpRequestHandler.hpp \
    RequestHandlers/GpHttpRequestHandlerFactory.hpp \
    Routers/GpHttpRouter.hpp \
    Routers/RulesRouter/GpHttpRouterRule.hpp \
    Routers/TableRouter/GpHttpRouteTable.hpp \
    Routers/TableRouter/GpHttpRouteTableStatic.hpp \
    Routers/TableRouter/GpHttpTableRouter.hpp \
    RqRs/GpHttpParser.hpp \
    RqRs/GpHttpRequest.hpp \
    RqRs/GpHttpRequestNoBodyDesc.hpp \
    RqRs/GpHttpResponse.hpp \
    RqRs/GpHttpResponseNoBodyDesc.hpp \
    Server/GpHttpServer.hpp \
    Server/GpHttpServerCfgDesc.hpp \
    Server/GpHttpServerRequestTask.hpp \
    Server/GpHttpServerRequestTaskFactory.hpp \
    Url/GpUrl.hpp \
    Url/GpUrlAuthority.hpp \
    Url/GpUrlQuery.hpp \
    Url/GpUrlReflection.hpp

