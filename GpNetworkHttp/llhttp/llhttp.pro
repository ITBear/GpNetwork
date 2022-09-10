TEMPLATE        = lib
CONFIG         += staticlib
VER_MAJ		    = 0
VER_MIN		    = 1
VER_PAT		    = 1
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=45828c5b-9c50-41c7-bffa-4f4541b9fa55
QMAKE_CXXFLAGS += -DGP_REFLECTION_STATIC_ADD_TO_MANAGER
DEFINES		   += LLHTTP_LIBRARY
PACKET_NAME     = llhttp
DIR_LEVEL       = ./../../../

include(../../../../QtGlobalPro.pri)

debug_build {
	DEFINES += DEBUG
	DEFINES += _DEBUG
} else:profile_build {
	DEFINES += DEBUG
	DEFINES += _DEBUG
} else:release_build {
	DEFINES += NDEBUG
} else {
	error(Unknown build mode. Set CONFIG+=debug_build OR CONFIG+=release_build)
}

#------------------------------ LIBS BEGIN ---------------------------------
os_windows{
}

os_linux{
}
#------------------------------- LIBS END ----------------------------------

SOURCES += \
	api.c \
	http.c \
	llhttp.c

HEADERS += \
	llhttp.h
