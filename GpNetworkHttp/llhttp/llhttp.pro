# ----------- Config -----------
TEMPLATE        = lib
QMAKE_CXXFLAGS += -DGP_REFLECTION_STATIC_ADD_TO_MANAGER
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=45828c5b-9c50-41c7-bffa-4f4541b9fa55
PACKET_NAME     = llhttp
DEFINES        += LLHTTP_LIBRARY
_VER_MAJ        = 2
_VER_MIN        = 1
_VER_PAT        = 6
DIR_LEVEL       = ./../../../

include($$DIR_LEVEL/../QtGlobalPro.pri)

equals(var_link, "static") {
	CONFIG += staticlib
}

equals(var_build, "release") {
	DEFINES += NDEBUG
}

equals(var_build, "debug") {
	DEFINES += DEBUG
	DEFINES += _DEBUG
}

# ----------- Libraries -----------
equals(var_os, "windows") {
	LIBS += -lws2_32
}

equals(var_os, "linux") {
}

# ----------- Sources and headers -----------
SOURCES += \
	api.c \
	http.c \
	llhttp.c

HEADERS += \
	llhttp.h
