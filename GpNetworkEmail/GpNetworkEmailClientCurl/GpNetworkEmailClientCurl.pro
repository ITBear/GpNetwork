TEMPLATE        = lib
#CONFIG         += staticlib
VER_MAJ		    = 0
VER_MIN		    = 1
VER_PAT		    = 0
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=da4d9cb1-bf8b-4e0b-a2b4-79c09b615fe1
QMAKE_CXXFLAGS += -DGP_REFLECTION_STATIC_ADD_TO_MANAGER
DEFINES		   += GP_NETWORK_EMAIL_CLIENT_CURL_LIBRARY
PACKET_NAME     = GpNetworkEmailClientCurl
DIR_LEVEL       = ./../../../

include(../../../../QtGlobalPro.pri)

#------------------------------ LIBS BEGIN ---------------------------------
os_windows{
}

os_linux{
}
#------------------------------- LIBS END ----------------------------------

SOURCES += \
    GpSmtpClientCurl.cpp \
    GpSmtpClientCurlFactory.cpp

HEADERS += \
    GpNetworkEmailClientCurl.hpp \
    GpNetworkEmailClientCurl_global.hpp \
    GpSmtpClientCurl.hpp \
    GpSmtpClientCurlFactory.hpp
