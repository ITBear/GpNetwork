CONFIG		-= qt
QT			-= core gui widgets
TEMPLATE	= lib
VER_MAJ		= 0
VER_MIN		= 1
VER_PAT		= 0
CONFIG		+= warn_on
DEFINES		+= GPNETWORK_LIBRARY \
			HAVE_NETINET_IN_H \
			_GLIBCXX_USE_NANOSLEEP

QMAKE_CXXFLAGS += -DGP_MODULE_UUID=e6eaf32a-e9e7-4c0c-aa56-b83524556f77
QMAKE_CXXFLAGS += -DGP_TYPE_SYSTEM_STATIC_ADD_TO_MANAGER

PACKET_NAME     = GpNetwork
OUT_BUILD_PATH  = ./../../../Bin_tmp/

compiler_gcc{
	QMAKE_CC		=	gcc-10
	QMAKE_CXX		=	g++-10
	QMAKE_LINK		=	g++-10
}else:compiler_clang{
	QMAKE_CXXFLAGS	+= -stdlib=libc++
	QMAKE_LFLAGS    += -stdlib=libc++

	#QMAKE_CXXFLAGS	+= -stdlib=libstdc++
	#QMAKE_LFLAGS   += -stdlib=libstdc++
}else:compiler_emscripten{
}else{
	error(Unknown compiler mode. Set CONFIG+=compiler_gcc OR CONFIG+=compiler_clang OR CONFIG+=compiler_emscripten)
}

#c++20
CONFIG			+=	c++2a
QMAKE_CXXFLAGS	+=	-std=gnu++2a

QMAKE_CXXFLAGS	+= -fvisibility=hidden -fvisibility-inlines-hidden
QMAKE_CXXFLAGS	+= -ffunction-sections -fdata-sections -fexceptions -fstrict-aliasing -fstack-clash-protection
QMAKE_CXXFLAGS	+= -Wall -Wextra -Wdouble-promotion -Wswitch-default -Wswitch-enum -Wuninitialized
QMAKE_CXXFLAGS	+= -Wstrict-aliasing -Wfloat-equal -Wshadow -Wplacement-new -Wcast-align -Wconversion -Wlogical-op
QMAKE_CXXFLAGS	+= -Wduplicated-cond -Wduplicated-branches -Wrestrict -Wnull-dereference -Wno-terminate
QMAKE_CXXFLAGS	+= -Wno-unknown-warning-option -Wno-unused-command-line-argument -Wno-comment
#QMAKE_CXXFLAGS	+= -fno-rtti
QMAKE_LFLAGS    += -Wl,--gc-sections

#------------------------ DEBUG or RELEASE ---------------------
debug_build {
	message([$$PACKET_NAME]: ***************** Build mode DEBUG *****************)
	DEFINES			   += DEBUG_BUILD
	TARGET_POSTFIX		= _d
	OUT_BUILD_MODE_PATH	= Debug
	os_linux {
		QMAKE_CXXFLAGS	+= -fsanitize=address -fsanitize=undefined -fno-sanitize=vptr
		LIBS += -lasan
		LIBS += -lubsan
		#BOOST_POSTFIX = _asan
	}
} else:release_build {
	message([$$PACKET_NAME]: ***************** Build mode RELEASE *****************)
	DEFINES			    += RELEASE_BUILD
	TARGET_POSTFIX		=
	OUT_BUILD_MODE_PATH	= Release
} else {
	error(Unknown build mode. Set CONFIG+=debug_build OR CONFIG+=release_build)
}

#------------------------ OS ---------------------
os_linux {
	OUT_BUILD_OS_PATH = Linux
} else:os_android {
	OUT_BUILD_OS_PATH = Android
} else:os_ios {
	OUT_BUILD_OS_PATH = Ios
} else:os_windows {
	OUT_BUILD_OS_PATH = Win
} else:os_macx {
	OUT_BUILD_OS_PATH = Macx
} else:os_browser {
	OUT_BUILD_OS_PATH = Browser
} else {
	error(Unknown OS. Set CONFIG+=... one of values: os_linux, os_android, os_ios, os_windows, os_macx, os_browser)
}

#------------------------ ARC ---------------------
arc_x86_64 {
	OUT_BUILD_ARCH_PATH = x86_64
	QMAKE_CXXFLAGS	+= -mtune=generic -march=x86-64
} else:arc_x86 {
	OUT_BUILD_ARCH_PATH = x86
	QMAKE_CXXFLAGS	+= -mtune=generic -march=i686
} else:arc_arm_v6 {
	OUT_BUILD_ARCH_PATH = arm_v6
} else:arc_armeabi_v7a {
	OUT_BUILD_ARCH_PATH = armeabi_v7a
} else:arc_arm64_v8a {
	OUT_BUILD_ARCH_PATH = arm64_v8a
} else:arc_mips {
	OUT_BUILD_ARCH_PATH = mips
} else:arc_wasm32 {
	OUT_BUILD_ARCH_PATH = wasm32
} else:arc_wasm64 {
	OUT_BUILD_ARCH_PATH = wasm64
} else {
	error(Unknown ARC. Set CONFIG+=... one of values: arc_x86_64, arc_x86, arc_arm_v6, arc_armeabi_v7a, arc_arm64_v8a, arc_mips, arc_wasm32, arc_wasm64)
}

TARGET = $$PACKET_NAME$$TARGET_POSTFIX

SP = _
DESTDIR = $$OUT_BUILD_PATH$$OUT_BUILD_MODE_PATH$$SP$$OUT_BUILD_OS_PATH$$SP$$OUT_BUILD_ARCH_PATH/

message([$$PACKET_NAME]: Target name: $$TARGET)
message([$$PACKET_NAME]: Target arch: $$OUT_BUILD_ARCH_PATH)
message([$$PACKET_NAME]: Target OS:   $$OUT_BUILD_OS_PATH)
message([$$PACKET_NAME]: Build directory $$DESTDIR)
message([$$PACKET_NAME]: -------------------------------------------------)

#------------------------------ LIBS BEGIN ---------------------------------
LIBS += -L$$DESTDIR

os_windows{
	GP_CORE_LIB_V		= 2
}

os_linux{
}

LIBS += -lGpCore2$$TARGET_POSTFIX$$GP_CORE_LIB_V
LIBS += -lpistache
LIBS += -lcurl

#------------------------------ LIBS END ---------------------------------

INCLUDEPATH += \
	../Extras \
	../Extras/Boost/boost_1_72_0$$BOOST_POSTFIX

SOURCES += \
    GpNetwork.cpp \
    HTTP/GpHttpClient.cpp \
    HTTP/GpHttpRequest.cpp \
    HTTP/GpHttpResponse.cpp \
    HTTP/GpHttpResponseCode.cpp \
    HTTP/GpHttpServer.cpp \
    HTTP/Pistache/GpHttpRouteHandlerPistache.cpp \
    HTTP/Pistache/GpHttpServerFactoryPistache.cpp \
    HTTP/Pistache/GpHttpServerPistache.cpp

HEADERS += \
    GpNetworkConfig.hpp \
    GpNetwork_global.hpp \
    GpNetwork.hpp \
    HTTP/GpHTTP.hpp \
    HTTP/GpHttpClient.hpp \
    HTTP/GpHttpRequest.hpp \
    HTTP/GpHttpRequestHandler.hpp \
    HTTP/GpHttpRequestHandlerFactory.hpp \
    HTTP/GpHttpResponse.hpp \
    HTTP/GpHttpResponseCode.hpp \
    HTTP/GpHttpServer.hpp \
    HTTP/GpHttpServerFactory.hpp \
    HTTP/Pistache/GpHttpRouteHandlerPistache.hpp \
    HTTP/Pistache/GpHttpServerFactoryPistache.hpp \
    HTTP/Pistache/GpHttpServerPistache.hpp
