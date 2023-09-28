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
	error("Unknown OS. Set CONFIG+=... one of values: os_linux, os_android, os_ios, os_windows, os_macx, os_browser, os_baremetal")
}

CONFIG += ordered
