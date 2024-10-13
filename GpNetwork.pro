TEMPLATE = subdirs

os_linux {
	SUBDIRS += \
		./GpNetworkCore \
		./GpNetworkHttp
} else:os_android {
	SUBDIRS += \
		./GpNetworkCore \
		./GpNetworkHttp
} else:os_ios {
	SUBDIRS += \
		./GpNetworkCore \
		./GpNetworkHttp
} else:os_windows {
	SUBDIRS += \
		./GpNetworkCore \
		./GpNetworkHttp
} else:os_macx {
	SUBDIRS += \
		./GpNetworkCore \
		./GpNetworkHttp
} else:os_browser {
	SUBDIRS +=
} else {
    error("Unknown OS")
}

CONFIG += ordered
