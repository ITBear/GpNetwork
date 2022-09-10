TEMPLATE        = lib
#CONFIG         += staticlib
VER_MAJ		    = 0
VER_MIN		    = 1
VER_PAT		    = 0
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=e1fb9424-3651-49dd-97f9-70f50cc0dcb1
QMAKE_CXXFLAGS += -DGP_REFLECTION_STATIC_ADD_TO_MANAGER
DEFINES		   += GP_NETWORK_HTTP_SERVER_BASIC_LIBRARY
PACKET_NAME     = GpNetworkHttpServerBasic
DIR_LEVEL       = ./../../../

include(../../../../QtGlobalPro.pri)

#------------------------------ LIBS BEGIN ---------------------------------
os_windows{
}

os_linux{
}
#------------------------------- LIBS END ----------------------------------

SOURCES += \
    GpHttpServerBasic.cpp \
    GpHttpServerBasicFactory.cpp \
    GpHttpServerBasicSocketTask.cpp \
    GpHttpServerBasicSocketTaskFactory.cpp

HEADERS += \
    GpHttpServerBasic.hpp \
    GpHttpServerBasicFactory.hpp \
    GpHttpServerBasicSocketTask.hpp \
    GpHttpServerBasicSocketTaskFactory.hpp \
    GpNetworkHttpServerBasic.hpp \
    GpNetworkHttpServerBasic_global.hpp
