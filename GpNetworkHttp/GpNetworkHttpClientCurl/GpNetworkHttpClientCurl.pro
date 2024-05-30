# ----------- Config -----------
TEMPLATE        = lib
#CONFIG        += staticlib
QMAKE_CXXFLAGS += -DGP_REFLECTION_STATIC_ADD_TO_MANAGER
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=060ef17f-83bd-4a30-b03c-12d3f042a620
PACKET_NAME     = GpNetworkHttpClientCurl
DEFINES        += GP_NETWORK_HTTP_CLIENT_CURL_LIBRARY
_VER_MAJ        = 2
_VER_MIN        = 1
_VER_PAT        = 5
DIR_LEVEL       = ./../../../

include($$DIR_LEVEL/../QtGlobalPro.pri)

# ----------- Libraries -----------
os_windows{
}

os_linux{
}

# ----------- Sources and headers -----------
SOURCES += \
    GpHttpClientCurlAsync.cpp \
    GpHttpClientCurlAsyncFactory.cpp \
    GpHttpClientCurlLib.cpp \
    GpHttpClientCurlSync.cpp \
    GpHttpClientCurlSyncFactory.cpp

HEADERS += \
    GpHttpClientCurlAsync.hpp \
    GpHttpClientCurlAsyncFactory.hpp \
    GpHttpClientCurlLib.hpp \
    GpHttpClientCurlSync.hpp \
    GpHttpClientCurlSyncFactory.hpp \
    GpNetworkHttpClientCurl_global.hpp
