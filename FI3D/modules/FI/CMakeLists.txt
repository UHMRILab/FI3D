#=================== INCLUSION OF FI MODULE ====================#
option(MODULE_FI_ENABLE "Enables the FI module" ON)
if (MODULE_FI_ENABLE)
    message("Module Enabled: FI")

    set(MODULE_FI_NAME "Framework Interface")
    set(MODULE_FI_ACRONYM "FI")

    set(MODULE_FI_FORMS_DIR "${CMAKE_SOURCE_DIR}/modules/FI/forms")
    set(MODULE_FI_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/modules/FI/include")
    set(MODULE_FI_SOURCE_DIR "${CMAKE_SOURCE_DIR}/modules/FI/src")
    set(MODULE_FI_DATA "${CMAKE_SOURCE_DIR}/modules/FI/data")
    set(MODULE_FI_ASSETS "${CMAKE_SOURCE_DIR}/modules/FI/assets")
    
    include_directories(${MODULE_FI_FORMS_DIR})
    include_directories(${MODULE_FI_INCLUDE_DIR})
    include_directories(${MODULE_FI_SOURCE_DIR})
    
    file(GLOB_RECURSE MODULE_FI_SOURCES
        "${MODULE_FI_FORMS_DIR}/*.ui"
        "${MODULE_FI_INCLUDE_DIR}/*.h"
        "${MODULE_FI_SOURCE_DIR}/*.cpp"
    )
    
    set(FI3D_SOURCES ${FI3D_SOURCES} ${MODULE_FI_SOURCES})
    set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_AUTOUIC_SEARCH_PATHS} ${MODULE_FI_FORMS_DIR})
else()
    message("Module Disabled: FI")
endif()

# Add the target includes for FI if enabled
if (MODULE_FI_ENABLE)
    set(FI3D_MODULES_DIRECTORIES ${FI3D_MODULES_DIRECTORIES} ${MODULE_FI_FORMS_DIR})
    set(FI3D_MODULES_DIRECTORIES ${FI3D_MODULES_DIRECTORIES} ${MODULE_FI_INCLUDE_DIR})
    set(FI3D_MODULES_DIRECTORIES ${FI3D_MODULES_DIRECTORIES} ${MODULE_FI_SOURCE_DIR})
endif()
