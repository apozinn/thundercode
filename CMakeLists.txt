cmake_minimum_required(VERSION 3.12)

project(ThunderCode)
find_package(wxWidgets COMPONENTS core base stc REQUIRED)

include(${wxWidgets_USE_FILE})
include(FetchContent)

set(SRC_FILES main.cpp)
add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE ${SRC_FILES})

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json PRIVATE ${wxWidgets_LIBRARIES})