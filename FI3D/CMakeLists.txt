cmake_minimum_required(VERSION 3.9)

#======================= Find subdirectories in directory =======================#
# Obtained from: https://stackoverflow.com/questions/7787823/cmake-how-to-get-the-name-of-all-subdirectories-of-a-directory
MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

#======================= Setup FI3D =======================#
option(CUDA_ENABLE "Whether CUDA functions should be turned on" OFF)
if (CUDA_ENABLE)
    project(FI3D LANGUAGES CXX CUDA)
else()
    if (WIN32)
        project(FI3D LANGUAGES CXX)
    elseif(UNIX)
        project(FI3D)
    endif()

    if (APPLE)
        cmake_policy(SET CMP0025 NEW)
    endif()

    set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS
        FI3D_NO_CUDA
    )
endif()

option (BUILD_AS_DEPLOY "Whether to deploy the application" OFF)

if (BUILD_AS_DEPLOY)
    set(CMAKE_CONFIGURATION_TYPES "Release" CACHE STRING "" FORCE)
    set(FI3D_ASSETS_DIR "./assets" CACHE FILEPATH "Full path to assets directory" FORCE)
    set(FI3D_DATA_DIR "./data" CACHE FILEPATH "Full path to data directory" FORCE)
    add_definitions(-DBUILD_AS_DEPLOY=true)
else ()
    set(CMAKE_CONFIGURATION_TYPES "Release;RelWithDebInfo" CACHE STRING "" FORCE)
    set(FI3D_ASSETS_DIR "${CMAKE_SOURCE_DIR}/assets" CACHE FILEPATH "Full path to assets directory" FORCE)
    set(FI3D_DATA_DIR "${CMAKE_SOURCE_DIR}/data" CACHE FILEPATH "Full path to data directory" FORCE)
endif()

add_definitions(-DFI3D_ASSETS_PATH="${FI3D_ASSETS_DIR}")
add_definitions(-DFI3D_DATA_PATH="${FI3D_DATA_DIR}")

#======================= INCLUSION OF Qt =======================#
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH $ENV{QTDIR})
find_package(OpenGL)
find_package(Qt6Charts REQUIRED)
find_package(Qt6Core REQUIRED)
#find_package(Qt6Gamepad REQUIRED)
find_package(Qt6Gui REQUIRED)
find_package(Qt6Network REQUIRED)
find_package(Qt6OpenGL REQUIRED)
find_package(Qt6Xml REQUIRED)
find_package(Qt6Widgets REQUIRED)

#======================= INCLUSION OF VTK ======================#
set(VTK_DIR $ENV{VTK_DIR})
find_package(VTK REQUIRED)

#=================== INCLUSION OF FI3D ====================#
set(FI3D_FORMS_DIR "${CMAKE_SOURCE_DIR}/forms")
set(FI3D_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(FI3D_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src/fi3d")

include_directories(${FI3D_FORMS_DIR})
include_directories(${FI3D_INCLUDE_DIR})
include_directories(${FI3D_SOURCE_DIR})

file(GLOB_RECURSE FI3D_SOURCES
    "${FI3D_FORMS_DIR}/*.ui"
    "${FI3D_FORMS_DIR}/*.qrc"
    "${FI3D_INCLUDE_DIR}/*.h"
    "${FI3D_SOURCE_DIR}/*.cpp"
)

if (CUDA_ENABLE)
    file(GLOB_RECURSE CUDA_SOURCES
        "${FI3D_INCLUDE_DIR}/*.cuh"
        "${FI3D_SOURCE_DIR}/*.cu"
    )
    set(FI3D_SOURCES ${FI3D_SOURCES} ${CUDA_SOURCES})
endif()

#======================= Find and Include Components =======================#
SUBDIRLIST(FI3D_COMPONENTS_DIRS ${CMAKE_SOURCE_DIR}/components)
FOREACH(fi3dComponentDir ${FI3D_COMPONENTS_DIRS})
    if (EXISTS "${CMAKE_SOURCE_DIR}/components/${fi3dComponentDir}/CMakeLists.txt")
        include(${CMAKE_SOURCE_DIR}/components/${fi3dComponentDir}/CMakeLists.txt)
    else ()
        message("No CMAKE information found for ${fi3dComponentDir}")
    endif ()
ENDFOREACH()

#======================= Find and Include Modules =======================#
set(MODULE_ACRONYMS)
set(ENABLED_MODULES)
SUBDIRLIST(FI3D_MODULES_DIRS ${CMAKE_SOURCE_DIR}/modules)
FOREACH(fi3dModuleDir ${FI3D_MODULES_DIRS})
    if (EXISTS "${CMAKE_SOURCE_DIR}/modules/${fi3dModuleDir}/CMakeLists.txt")
        include(${CMAKE_SOURCE_DIR}/modules/${fi3dModuleDir}/CMakeLists.txt)

        list(APPEND MODULE_ACRONYMS "${fi3dModuleDir}")
    else ()
        message("No CMAKE information found for ${fi3dModuleDir}")
    endif ()
ENDFOREACH()

#=================== SETUP EXECTUABLE ====================#
# Enable debug logging on RELWITHDEBINFO configuration
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS
    $<$<CONFIG:RELWITHDEBINFO>:QT_MESSAGELOGCONTEXT>
)

# Add the forms directory to the AUTOUIC search paths
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_AUTOUIC_SEARCH_PATHS} ${FI3D_FORMS_DIR})
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_AUTOUIC_SEARCH_PATHS} "${FI3D_FORMS_DIR}/anchors")
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_AUTOUIC_SEARCH_PATHS} "${FI3D_FORMS_DIR}/data")
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_AUTOUIC_SEARCH_PATHS} "${FI3D_FORMS_DIR}/server")
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_AUTOUIC_SEARCH_PATHS} "${FI3D_FORMS_DIR}/rendering")

# Add the executable
if (WIN32) 
    add_executable(FI3D WIN32 ${FI3D_SOURCES} "${CMAKE_SOURCE_DIR}/src/main.cpp")
elseif(UNIX)
    add_executable(FI3D ${FI3D_SOURCES} "${CMAKE_SOURCE_DIR}/src/main.cpp")
endif()

# Add the target includes for FI3D 
target_include_directories(FI3D PRIVATE ${FI3D_FORMS_DIR})
target_include_directories(FI3D PRIVATE ${FI3D_INCLUDE_DIR})
target_include_directories(FI3D PRIVATE ${FI3D_SOURCE_DIR})

# Add the component targets
target_include_directories(FI3D PRIVATE "${FI3D_COMPONENTS_DIRECTORIES}/components")

# Add the module targets
target_include_directories(FI3D PRIVATE ${FI3D_MODULES_DIRECTORIES})

#===================== LINKING LIBRARIES =======================#
target_link_libraries( FI3D Qt6::Charts)
#target_link_libraries( FI3D Qt6::Gamepad)
target_link_libraries( FI3D Qt6::Network)
target_link_libraries( FI3D Qt6::OpenGL)
target_link_libraries( FI3D Qt6::Xml)
target_link_libraries( FI3D Qt6::Widgets)
target_link_libraries( FI3D ${QT_LIBRARIES} ${OPENGL_LIBRARIES})
target_link_libraries( FI3D ${VTK_LIBRARIES})

if (BUILD_AS_DEPLOY)
    # Copy VTK required DLLs.
    FILE(GLOB vtk_files RELATIVE $ENV{VTK_DIR}/bin $ENV{VTK_DIR}/bin/*)
    FOREACH(vtk_file ${vtk_files})
        IF(NOT IS_DIRECTORY $ENV{VTK_DIR}/bin/${vtk_file})
            add_custom_command(TARGET FI3D POST_BUILD
                COMMAND "${CMAKE_COMMAND}" -E copy_if_different 
                "$ENV{VTK_DIR}/bin/${vtk_file}" 
                "${CMAKE_BINARY_DIR}/$<CONFIG>/")
        ENDIF()
    ENDFOREACH()

    # Copy OpenSSL required files.
    add_custom_command(TARGET FI3D POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different 
        "$ENV{OPENSSL_DIR}/bin/libcrypto-1_1-x64.dll" 
        "${CMAKE_BINARY_DIR}/$<CONFIG>/")
    add_custom_command(TARGET FI3D POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different 
        "$ENV{OPENSSL_DIR}/bin/libssl-1_1-x64.dll" 
        "${CMAKE_BINARY_DIR}/$<CONFIG>/")
    add_custom_command(TARGET FI3D POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different 
        "$ENV{OPENSSL_DIR}/bin/openssl.exe" 
        "${CMAKE_BINARY_DIR}/$<CONFIG>/")    

    # Copy Assets. 
    add_custom_command(TARGET FI3D POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_directory 
        ${CMAKE_SOURCE_DIR}/assets
        "${CMAKE_BINARY_DIR}/$<CONFIG>/assets")
    FOREACH(acronym ${ENABLED_MODULES})
        add_custom_command(TARGET FI3D POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_directory 
            ${CMAKE_SOURCE_DIR}/modules/${acronym}/assets
            "${CMAKE_BINARY_DIR}/$<CONFIG>/assets/modules/${acronym}")
    ENDFOREACH()

    # Generate Qt required files.
    add_custom_command(TARGET FI3D POST_BUILD
        COMMAND "$ENV{QTDIR}/bin/windeployqt.exe" "FI3D.exe"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/")

endif()

#===================== FI3D LIBRARY =======================#
option(BUILD_SHARED_LIB "Whether to compile a shared library" OFF)

if (BUILD_SHARED_LIB)
    # Add FI3D library
    add_library(FI3D_LIB SHARED ${FI3D_SOURCES})

    target_include_directories(FI3D_LIB PRIVATE ${FI3D_FORMS_DIR})
    target_include_directories(FI3D_LIB PRIVATE ${FI3D_INCLUDE_DIR})
    target_include_directories(FI3D_LIB PRIVATE ${FI3D_SOURCE_DIR})

    # Add the module targets
    target_include_directories(FI3D_LIB PRIVATE ${FI3D_COMPONENTS_DIRECTORIES})

    # Add the module targets
    target_include_directories(FI3D_LIB PRIVATE ${FI3D_MODULES_DIRECTORIES})

    # Link libraries
    target_link_libraries( FI3D_LIB Qt6::Charts)
    #target_link_libraries( FI3D_LIB Qt6::Gamepad)
    target_link_libraries( FI3D_LIB Qt6::Network)
    target_link_libraries( FI3D_LIB Qt6::OpenGL)
    target_link_libraries( FI3D_LIB Qt6::Xml)
    target_link_libraries( FI3D_LIB Qt6::Widgets)
    target_link_libraries( FI3D_LIB ${QT_LIBRARIES} ${OPENGL_LIBRARIES})
    target_link_libraries( FI3D_LIB ${VTK_LIBRARIES})
endif()
