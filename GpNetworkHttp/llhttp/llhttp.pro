# ----------- Config -----------
TEMPLATE        = lib
CONFIG         += staticlib
QMAKE_CXXFLAGS += -DGP_REFLECTION_STATIC_ADD_TO_MANAGER
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=45828c5b-9c50-41c7-bffa-4f4541b9fa55
PACKET_NAME     = llhttp
DEFINES        += LLHTTP_LIBRARY
_VER_MAJ        = 2
_VER_MIN        = 1
_VER_PAT        = 5
DIR_LEVEL       = ./../../../

include($$DIR_LEVEL/../QtGlobalPro.pri)

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

# ----------- Libraries -----------
os_windows{
}

os_linux{
}

# ----------- Sources and headers -----------
SOURCES += \
	api.c \
	http.c \
	llhttp.c

HEADERS += \
	llhttp.h
