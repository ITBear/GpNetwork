#pragma once

#include "../GpNetworkCore_global.hpp"

#if defined(GP_POSIX)
#   include <sys/socket.h>
#   include <sys/types.h>
#   include <netinet/in.h>
#   include <netinet/tcp.h>
#   include <arpa/inet.h>
#   include <unistd.h>
#   include <fcntl.h>
#endif//

#if defined(GP_OS_WINDOWS)
#   include <Winsock2.h>
#endif
