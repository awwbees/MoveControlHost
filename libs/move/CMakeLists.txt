project(move)

add_library(${PROJECT_NAME}
    JuceToMoveDisplayBridge.cpp
        Move-Display.cpp
        Move-Display.h
        Move-UsbCommunicator.h
    Result.cpp
    include/move/JuceToMoveDisplayBridge.h
    include/move/Macros.h
    include/move/Result.h
    )

# FIXME: The code currently assumes that WIN32 == MSVC and will fail to build
# on MinGW, so we use the system libusb on there. This is the best option for
# native builds anyway, but for cross we should build libusb for convenience.
if(APPLE OR MSVC)
    target_sources(${PROJECT_NAME} PRIVATE
        modules/libusb/core.c
        modules/libusb/descriptor.c
        modules/libusb/hotplug.c
        modules/libusb/io.c
        modules/libusb/strerror.c
        modules/libusb/sync.c
        )
    target_include_directories(${PROJECT_NAME} PRIVATE
        libusb
        modules/libusb
        )
endif()

if(APPLE)
    target_sources(${PROJECT_NAME} PRIVATE
        modules/libusb/os/darwin_usb.c
        modules/libusb/os/poll_posix.c
        modules/libusb/os/threads_posix.c
        )
    target_compile_definitions(${PROJECT_NAME} PRIVATE OBJC_SILENCE_GC_DEPRECATIONS=1)
elseif(MSVC)
    target_sources(${PROJECT_NAME} PRIVATE
        modules/libusb/os/poll_windows.c
        modules/libusb/os/threads_windows.c
        modules/libusb/os/windows_winusb.c
        modules/libusb/os/windows_nt_common.c
        )
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        HAVE_STRUCT_TIMESPEC
        THREADS_WINDOWS
        )
else()
    find_package(PkgConfig)
    pkg_check_modules(LIBUSB libusb-1.0)
    if(NOT LIBUSB_FOUND)
        message(WARNING "libusb not found, disabling Ableton Push 2 support")
    endif()
    target_include_directories(${PROJECT_NAME} PRIVATE ${LIBUSB_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBUSB_LIBRARIES})
endif()

target_sources(${PROJECT_NAME} PRIVATE Move-Usb-Communicator.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC include)
add_library(movecontrolhost::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
