cmake_minimum_required(VERSION 3.12)

project(ThunderCode)
find_package(wxWidgets COMPONENTS core base stc REQUIRED)

include(${wxWidgets_USE_FILE})

set(SRC_FILES main.cpp)

add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})