#=================== INCLUSION OF DEMO MODULE ====================#
option(MODULE_DEMO_ENABLE "Enables the DEMO module" ON)
if (MODULE_DEMO_ENABLE)
    message("Module Enabled: DEMO")

    set(MODULE_DEMO_NAME "Demonstration")
    set(MODULE_DEMO_ACRONYM "DEMO")
    
    set(MODULE_DEMO_FORMS_DIR "${CMAKE_SOURCE_DIR}/modules/DEMO/forms")
    set(MODULE_DEMO_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/modules/DEMO/include")
    set(MODULE_DEMO_SOURCE_DIR "${CMAKE_SOURCE_DIR}/modules/DEMO/src")
    set(MODULE_DEMO_DATA "${CMAKE_SOURCE_DIR}/modules/DEMO/data")
    set(MODULE_DEMO_ASSETS "${CMAKE_SOURCE_DIR}/modules/DEMO/assets")

    include_directories(${MODULE_DEMO_FORMS_DIR})
    include_directories(${MODULE_DEMO_INCLUDE_DIR})
    include_directories(${MODULE_DEMO_SOURCE_DIR})

    file(GLOB_RECURSE MODULE_DEMO_SOURCES
        "${MODULE_DEMO_FORMS_DIR}/*.ui"
        "${MODULE_DEMO_INCLUDE_DIR}/*.h"
        "${MODULE_DEMO_SOURCE_DIR}/*.cpp"
    )

    set(FI3D_SOURCES ${FI3D_SOURCES} ${MODULE_DEMO_SOURCES})
    set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_AUTOUIC_SEARCH_PATHS} ${MODULE_DEMO_FORMS_DIR})
else()
    message("Module Disabled: DEMO")
endif()

# Add the target includes for DEMO if enabled
if (MODULE_DEMO_ENABLE)
    set(FI3D_MODULES_DIRECTORIES ${FI3D_MODULES_DIRECTORIES} ${MODULE_DEMO_FORMS_DIR})
    set(FI3D_MODULES_DIRECTORIES ${FI3D_MODULES_DIRECTORIES} ${MODULE_DEMO_INCLUDE_DIR})
    set(FI3D_MODULES_DIRECTORIES ${FI3D_MODULES_DIRECTORIES} ${MODULE_DEMO_SOURCE_DIR})
endif()
