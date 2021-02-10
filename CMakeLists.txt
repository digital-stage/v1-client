cmake_minimum_required(VERSION 3.18.0)

## GLOBAL CONFIGURATION
project(ds-client LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


# Excude js files inside the libs folder
set_property(SOURCE "lib/ov-client/webmixer.js" PROPERTY SKIP_AUTOMOC ON)
set_property(SOURCE "lib/ov-client/webmixer.js" PROPERTY SKIP_AUTOUIC ON)

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
    if(CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "x86_64")
        # Check whether we are running under Rosetta on arm64 hardware.
        execute_process(COMMAND sysctl -q hw.optional.arm64
            OUTPUT_VARIABLE _sysctl_stdout
            ERROR_VARIABLE _sysctl_stderr
            RESULT_VARIABLE _sysctl_result
            )
        if(_sysctl_result EQUAL 0 AND _sysctl_stdout MATCHES "hw.optional.arm64: 1")
            message("Compiling multi build for x84_64 and arm64")
            #set(CMAKE_HOST_SYSTEM_PROCESSOR "arm64")
            # Not possible right now with qt6
            #set(CMAKE_OSX_ARCHITECTURES arm64 x86_64)
            # set(CMAKE_CROSSCOMPILING false)
            #set(QMAKE_APPLE_DEVICE_ARCHS arm64)
            #set(CMAKE_OSX_ARCHITECTURES arm64)
        else()
            set(CMAKE_OSX_ARCHITECTURES x86_64)
        endif()
    endif()
endif()

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()


## DEPENDENCIES
#find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(QT NAMES Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools REQUIRED)

find_package(nlohmann_json 3.2.0 REQUIRED)
add_library(ov-client STATIC lib/ov-client)
find_package(qt5keychain REQUIRED)
find_package(cpprestsdk REQUIRED)
#find_package(OpenSSL REQUIRED)

if(APPLE)
    # Fix seems necessary
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${OPENSSL_ROOT_DIR}/include")
endif()

## SOURCE AND RESOURCE CONTROL
set(TS_FILES src/ds-client_en_150.ts)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        src/mainwindow.h
        src/loginpane.cpp
        src/loginpane.h
        src/loginpane.ui
        src/api/auth.h
        src/api/auth.cpp
        src/api/keystore.h
        src/api/keystore.cpp
        ${TS_FILES}
        resources.qrc
)

set(PROJECT_RESOURCES
    resources.qrc
    )




## APPLE SPECIFIC
if(APPLE)
    # Add app icon
    set(MACOSX_BUNDLE_ICON_FILE appicon.icns)
    set(APP_ICON ${CMAKE_CURRENT_SOURCE_DIR}/images/appicon.icns)
    set_source_files_properties(${APP_ICON} PROPERTIES
           MACOSX_PACKAGE_LOCATION "Resources")
endif()




## EXECUTABLE
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    message("Building with QT6")
    qt_add_executable(ds-client
        ${PROJECT_SOURCES} ${PROJECT_RESOURCES} ${APP_ICON}
    )
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    message("Building with QT5")
    if(ANDROID)
        add_library(ds-client SHARED
            ${PROJECT_SOURCES} ${PROJECT_RESOURCES} ${APP_ICON}
        )
    else()
        add_executable(ds-client ${PROJECT_SOURCES} ${PROJECT_RESOURCES} ${APP_ICON})
        if(APPLE)
            set_target_properties(ds-client PROPERTIES
              MACOSX_BUNDLE TRUE
              MACOSX_FRAMEWORK_IDENTIFIER org.cmake.ExecutableTarget
              RESOURCE "${PROJECT_RESOURCES}")
        endif()
    endif()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

link_directories(${Boost_LIBRARY_DIR})

target_link_libraries(ds-client PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    nlohmann_json::nlohmann_json
    qt5keychain
    cpprestsdk::cpprest
)

if(APPLE)
    set_target_properties(ds-client PROPERTIES
      MACOSX_BUNDLE TRUE
      MACOSX_FRAMEWORK_IDENTIFIER org.cmake.ExecutableTarget
      RESOURCE "${PROJECT_RESOURCES}")
endif()




## VERBOSE
message("CMAKE_OSX_ARCHITECTURES: ${CMAKE_OSX_ARCHITECTURES}")
message("CMAKE_HOST_SYSTEM_PROCESSOR: ${CMAKE_HOST_SYSTEM_PROCESSOR}")
message("CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")
message("CMAKE_CROSSCOMPILING: ${CMAKE_CROSSCOMPILING}")

