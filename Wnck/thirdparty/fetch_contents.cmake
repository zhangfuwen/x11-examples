include(FetchContent)
FetchContent_Declare(
    libsocket
    GIT_REPOSITORY https://github.com/dermesser/libsocket
    GIT_TAG        v2.5.0 # release-1.10.0
    )

FetchContent_MakeAvailable(libsocket)
include_directories(${libsocket_SOURCE_DIR}/headers)
message("libsocket_SOURCE_DIR ${libsocket_SOURCE_DIR}")

# polym/msgqueue
FetchContent_Declare(
    polym
    GIT_REPOSITORY https://github.com/khuttun/PolyM 
    GIT_TAG        66d8edb # release-1.10.0
    )

FetchContent_MakeAvailable(polym)
include_directories(${polym_SOURCE_DIR}/include)
message("polym_SOURCE_DIR ${polym_SOURCE_DIR}")

