TEMPLATE = subdirs

os_linux {
	SUBDIRS += \
		./llhttp \
		./GpNetworkHttpCore
} else:os_android {
	SUBDIRS += \
		./llhttp \
		./GpNetworkHttpCore
} else:os_ios {
	SUBDIRS += \
		./llhttp \
		./GpNetworkHttpCore
} else:os_windows {
	SUBDIRS += \
		./llhttp \
		./GpNetworkHttpCore
} else:os_macx {
	SUBDIRS += \
		./llhttp \
		./GpNetworkHttpCore
} else:os_browser {
	SUBDIRS +=
} else {
    error("Unknown OS")
}

CONFIG += ordered
