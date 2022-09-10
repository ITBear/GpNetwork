TEMPLATE        = lib
#CONFIG         += staticlib
VER_MAJ		    = 0
VER_MIN		    = 1
VER_PAT		    = 0
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=060ef17f-83bd-4a30-b03c-12d3f042a620
QMAKE_CXXFLAGS += -DGP_REFLECTION_STATIC_ADD_TO_MANAGER
DEFINES		   += GP_NETWORK_HTTP_CLIENT_CURL_LIBRARY
PACKET_NAME     = GpNetworkHttpClientCurl
DIR_LEVEL       = ./../../../

include(../../../../QtGlobalPro.pri)

#------------------------------ LIBS BEGIN ---------------------------------
os_windows{
}

os_linux{
}
#------------------------------- LIBS END ----------------------------------

SOURCES += \
    GpHttpClientCurl.cpp \
    GpHttpClientCurlAsync.cpp \
    GpHttpClientCurlFactory.cpp

HEADERS += \
    GpHttpClientCurl.hpp \
    GpHttpClientCurlAsync.hpp \
    GpHttpClientCurlFactory.hpp \
    GpNetworkHttpClientCurl.hpp \
    GpNetworkHttpClientCurl_global.hpp
