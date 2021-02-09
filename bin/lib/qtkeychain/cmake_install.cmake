# Install script for directory: /Users/tobias/Developer/digitalstage/ds-client/lib/qtkeychain

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/qt6keychain/translations" TYPE FILE FILES
    "/Users/tobias/Developer/digitalstage/ds-client/bin/lib/qtkeychain/qtkeychain_de.qm"
    "/Users/tobias/Developer/digitalstage/ds-client/bin/lib/qtkeychain/qtkeychain_fr.qm"
    "/Users/tobias/Developer/digitalstage/ds-client/bin/lib/qtkeychain/qtkeychain_ro.qm"
    "/Users/tobias/Developer/digitalstage/ds-client/bin/lib/qtkeychain/qtkeychain_ru.qm"
    "/Users/tobias/Developer/digitalstage/ds-client/bin/lib/qtkeychain/qtkeychain_zh.qm"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/qt6keychain" TYPE FILE FILES
    "/Users/tobias/Developer/digitalstage/ds-client/lib/qtkeychain/keychain.h"
    "/Users/tobias/Developer/digitalstage/ds-client/bin/lib/qtkeychain/qkeychain_export.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/Users/tobias/Developer/digitalstage/ds-client/bin/lib/qtkeychain/libqt6keychain.0.12.90.dylib"
    "/Users/tobias/Developer/digitalstage/ds-client/bin/lib/qtkeychain/libqt6keychain.1.dylib"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqt6keychain.0.12.90.dylib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqt6keychain.1.dylib"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      execute_process(COMMAND "/usr/bin/install_name_tool"
        -id "/usr/local/lib/libqt6keychain.1.dylib"
        "${file}")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/Users/tobias/Qt/6.0.1/clang_64/lib"
        "${file}")
      execute_process(COMMAND /usr/bin/install_name_tool
        -add_rpath "/Users/tobias/Qt/6.0.1/clang_64/lib"
        "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" -x "${file}")
      endif()
    endif()
  endforeach()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/Users/tobias/Developer/digitalstage/ds-client/bin/lib/qtkeychain/libqt6keychain.dylib")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqt6keychain.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqt6keychain.dylib")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -id "/usr/local/lib/libqt6keychain.1.dylib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqt6keychain.dylib")
    execute_process(COMMAND /usr/bin/install_name_tool
      -delete_rpath "/Users/tobias/Qt/6.0.1/clang_64/lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqt6keychain.dylib")
    execute_process(COMMAND /usr/bin/install_name_tool
      -add_rpath "/Users/tobias/Qt/6.0.1/clang_64/lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqt6keychain.dylib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" -x "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqt6keychain.dylib")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/mkspecs/modules" TYPE FILE FILES "/Users/tobias/Developer/digitalstage/ds-client/bin/lib/qtkeychain/qt_Qt6Keychain.pri")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Qt6Keychain/Qt6KeychainLibraryDepends.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Qt6Keychain/Qt6KeychainLibraryDepends.cmake"
         "/Users/tobias/Developer/digitalstage/ds-client/bin/lib/qtkeychain/CMakeFiles/Export/lib/cmake/Qt6Keychain/Qt6KeychainLibraryDepends.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Qt6Keychain/Qt6KeychainLibraryDepends-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Qt6Keychain/Qt6KeychainLibraryDepends.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Qt6Keychain" TYPE FILE FILES "/Users/tobias/Developer/digitalstage/ds-client/bin/lib/qtkeychain/CMakeFiles/Export/lib/cmake/Qt6Keychain/Qt6KeychainLibraryDepends.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Qt6Keychain" TYPE FILE FILES "/Users/tobias/Developer/digitalstage/ds-client/bin/lib/qtkeychain/CMakeFiles/Export/lib/cmake/Qt6Keychain/Qt6KeychainLibraryDepends-debug.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Qt6Keychain" TYPE FILE FILES
    "/Users/tobias/Developer/digitalstage/ds-client/bin/lib/qtkeychain/Qt6KeychainConfig.cmake"
    "/Users/tobias/Developer/digitalstage/ds-client/bin/lib/qtkeychain/Qt6KeychainConfigVersion.cmake"
    )
endif()

