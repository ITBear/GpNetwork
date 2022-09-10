TEMPLATE        = lib
#CONFIG         += staticlib
VER_MAJ		    = 0
VER_MIN		    = 1
VER_PAT		    = 0
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=32b5607b-1f7b-4e82-a5ff-d35311a7e482
QMAKE_CXXFLAGS += -DGP_REFLECTION_STATIC_ADD_TO_MANAGER
DEFINES		   += GP_NETWORK_EMAIL_CORE_LIBRARY
PACKET_NAME     = GpNetworkEmailCore
DIR_LEVEL       = ./../../../

include(../../../../QtGlobalPro.pri)

#------------------------------ LIBS BEGIN ---------------------------------
os_windows{
}

os_linux{
}
#------------------------------- LIBS END ----------------------------------

SOURCES += \
    GpEmail.cpp \
    GpEmailAddr.cpp \
    GpEmailBuilder.cpp \
    GpEmailEncoder.cpp \
    GpEmailHeaderType.cpp \
    GpEmailHeaders.cpp \
    GpEmailPart.cpp \
    SMTP/GpSmtpClient.cpp \
    SMTP/GpSmtpClientGuard.cpp \
    SMTP/GpSmtpClientPool.cpp \
    SMTP/GpSmtpClientPoolCatalog.cpp

HEADERS += \
    GpEmail.hpp \
    GpEmailAddr.hpp \
    GpEmailBuilder.hpp \
    GpEmailEncoder.hpp \
    GpEmailHeaderType.hpp \
    GpEmailHeaders.hpp \
    GpEmailPart.hpp \
    GpNetworkEmailCore.hpp \
    GpNetworkEmailCore_global.hpp \
    IMAP/GpIMAP.hpp \
    SMTP/GpSMTP.hpp \
    SMTP/GpSmtpClient.hpp \
    SMTP/GpSmtpClientFactory.hpp \
    SMTP/GpSmtpClientGuard.hpp \
    SMTP/GpSmtpClientPool.hpp \
    SMTP/GpSmtpClientPoolCatalog.hpp
