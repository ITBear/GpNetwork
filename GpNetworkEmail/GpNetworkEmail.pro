TEMPLATE = subdirs

os_linux {
	SUBDIRS += \
		./GpNetworkEmailCore \
		./GpNetworkEmailClientCurl
} else:os_android {
	SUBDIRS += \
		./GpNetworkEmailCore \
		./GpNetworkEmailClientCurl
} else:os_ios {
	SUBDIRS += \
		./GpNetworkEmailCore \
		./GpNetworkEmailClientCurl
} else:os_windows {
	SUBDIRS += \
		./GpNetworkEmailCore \
		./GpNetworkEmailClientCurl
} else:os_macx {
	SUBDIRS += \
		./GpNetworkEmailCore \
		./GpNetworkEmailClientCurl
} else:os_browser {
	SUBDIRS +=
} else {
	error("Unknown OS. Set CONFIG+=... one of values: os_linux, os_android, os_ios, os_windows, os_macx, os_browser, os_baremetal")
}

CONFIG += ordered
