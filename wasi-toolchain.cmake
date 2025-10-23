# WASI Toolchain File for CMake
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR wasm32)

# Force UNIX to be recognized as a Unix-like system
set(UNIX 1 CACHE BOOL "This is a UNIX-like platform" FORCE)
set(WASI 1)
set(CMAKE_CROSSCOMPILING TRUE)

# Specify the WASI SDK location
set(WASI_SDK_PREFIX "/opt/wasi-sdk")

# Set build tools
set(CMAKE_MAKE_PROGRAM "/usr/bin/make" CACHE FILEPATH "make program")

# Set compilers
set(CMAKE_C_COMPILER "${WASI_SDK_PREFIX}/bin/clang" CACHE FILEPATH "C compiler")
set(CMAKE_CXX_COMPILER "${WASI_SDK_PREFIX}/bin/clang++" CACHE FILEPATH "C++ compiler")
set(CMAKE_AR "${WASI_SDK_PREFIX}/bin/llvm-ar" CACHE FILEPATH "ar")
set(CMAKE_RANLIB "${WASI_SDK_PREFIX}/bin/llvm-ranlib" CACHE FILEPATH "ranlib")
set(CMAKE_LINKER "${WASI_SDK_PREFIX}/bin/wasm-ld" CACHE FILEPATH "linker")

# Set the sysroot
set(CMAKE_SYSROOT "${WASI_SDK_PREFIX}/share/wasi-sysroot")

# Compiler flags for WASI - use nostdinc to prevent host header inclusion
set(CMAKE_C_FLAGS_INIT "--sysroot=${CMAKE_SYSROOT} -D__wasi__ -D_WASI_EMULATED_SIGNAL -D_WASI_EMULATED_PROCESS_CLOCKS")
set(CMAKE_CXX_FLAGS_INIT "--sysroot=${CMAKE_SYSROOT} -D__wasi__ -D_WASI_EMULATED_SIGNAL -D_WASI_EMULATED_PROCESS_CLOCKS")

# Prevent CMake from finding host system libraries
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Set the find root path to WASI sysroot
set(CMAKE_FIND_ROOT_PATH "${CMAKE_SYSROOT}")

# Disable features not supported in WASI
set(CMAKE_EXECUTABLE_SUFFIX ".wasm")

# Prevent CMake from adding system include directories
set(CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES "")
set(CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES "")

# Disable libraries that don't exist in WASI
set(LIBMD_FOUND FALSE CACHE BOOL "Disable libmd" FORCE)
set(HAVE_LIBMD FALSE CACHE BOOL "Disable libmd" FORCE)

# Prevent curl from detecting Windows headers/features
set(HAVE_IO_H FALSE CACHE BOOL "No io.h in WASI" FORCE)
set(HAVE_WINDOWS_H FALSE CACHE BOOL "No windows.h in WASI" FORCE)
set(HAVE_WINSOCK2_H FALSE CACHE BOOL "No winsock2.h in WASI" FORCE)
set(HAVE_WS2TCPIP_H FALSE CACHE BOOL "No ws2tcpip.h in WASI" FORCE)

# Set curl integer sizes for wasm32
set(SIZEOF_OFF_T 8 CACHE STRING "Size of off_t" FORCE)
set(SIZEOF_CURL_OFF_T 8 CACHE STRING "Size of curl_off_t" FORCE)
set(SIZEOF_CURL_SOCKET_T 4 CACHE STRING "Size of curl_socket_t" FORCE)
set(SIZEOF_SA_FAMILY_T 2 CACHE STRING "Size of sa_family_t" FORCE)
set(SIZEOF_SIZE_T 4 CACHE STRING "Size of size_t for wasm32" FORCE)
set(SIZEOF_SSIZE_T 4 CACHE STRING "Size of ssize_t for wasm32" FORCE)
set(SIZEOF_LONG 4 CACHE STRING "Size of long for wasm32" FORCE)
set(SIZEOF_LONG_LONG 8 CACHE STRING "Size of long long" FORCE)
set(SIZEOF_INT 4 CACHE STRING "Size of int" FORCE)
set(SIZEOF_SHORT 2 CACHE STRING "Size of short" FORCE)
set(SIZEOF_TIME_T 8 CACHE STRING "Size of time_t" FORCE)
set(SIZEOF_VOIDP 4 CACHE STRING "Size of void* for wasm32" FORCE)

# Disable IPv6 and other network features not in WASI
set(ENABLE_IPV6 OFF CACHE BOOL "Disable IPv6" FORCE)
set(HAVE_NETINET_IN6_H FALSE CACHE BOOL "No netinet/in6.h in WASI" FORCE)
set(HAVE_NETINET_TCP_H FALSE CACHE BOOL "No netinet/tcp.h in WASI" FORCE)
set(HAVE_LINUX_TCP_H FALSE CACHE BOOL "No linux/tcp.h in WASI" FORCE)
set(HAVE_SYS_SOCKIO_H FALSE CACHE BOOL "No sys/sockio.h in WASI" FORCE)
set(HAVE_CLOCK_GETTIME_MONOTONIC_RAW FALSE CACHE BOOL "No CLOCK_MONOTONIC_RAW in WASI" FORCE)
set(HAVE_STRERROR_R FALSE CACHE BOOL "No strerror_r in WASI" FORCE)
set(HAVE_MEMRCHR FALSE CACHE BOOL "No memrchr in WASI" FORCE)
set(HAVE_CLOSESOCKET FALSE CACHE BOOL "No closesocket in WASI" FORCE)
set(HAVE_ARC4RANDOM FALSE CACHE BOOL "No arc4random in WASI" FORCE)
