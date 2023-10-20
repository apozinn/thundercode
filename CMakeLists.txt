cmake_minimum_required(VERSION 3.12)

project(ThunderCode)
find_package(wxWidgets COMPONENTS core base stc REQUIRED)

include(${wxWidgets_USE_FILE})

set(SRC_FILES main.cpp)

if(WIN32)
    list(APPEND SRC_FILES ./sample.rc)
elseif(APPLE)
    list(APPEND SRC_FILES ../../src/osx/carbon/wxmac.icns)
endif()

add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})

INCLUDE (FindPkgConfig)

IF (NOT PKG_CONFIG_FOUND)
    MESSAGE (FATAL_ERROR "pkg-config not found...")
ENDIF (NOT PKG_CONFIG_FOUND)

pkg_check_modules (VTE REQUIRED vte-2.91>=0.50)
IF (NOT VTE_FOUND)
    MESSAGE(FATAL_ERROR "You don't seem to have vte >= 0.50 development libraries installed...")
ENDIF (NOT VTE_FOUND)

if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        RESOURCE "../../src/osx/carbon/wxmac.icns"
        MACOSX_BUNDLE_ICON_FILE wxmac.icns
        MACOSX_BUNDLE_COPYRIGHT "Copyright wxWidgets"
        MACOSX_BUNDLE_GUI_IDENTIFIER "org.wxwidgets.minimal"
        )
endif()