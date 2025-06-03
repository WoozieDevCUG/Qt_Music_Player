cmake_minimum_required(VERSION 3.10)  

get_filename_component(CURRENT_DIR_PATH "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE) 
get_filename_component(CURRENT_DIR_NAME "${CURRENT_DIR_PATH}" NAME) 
project(${CURRENT_DIR_NAME})

set(CMAKE_CXX_STANDARD 11) 

if(MSVC)
    add_compile_options(/Zc:__cplusplus) 
endif()

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)
SET(CMAKE_AUTOUIC ON)

# 添加 Network 模块
find_package(Qt5 COMPONENTS Core Widgets Gui Charts Network Multimedia REQUIRED)

file(GLOB UI_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.ui")
file(GLOB RCC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*qrc")
file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_BINARY_DIR}/*.h")
file(GLOB CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

qt5_wrap_ui(UI_HEADERS ${UI_FILES})

source_group("Form Files" FILES ${UI_FILES})
source_group("Resource Files" FILES ${RCC_FILES})

add_executable(${PROJECT_NAME} WIN32 ${HEADER_FILES} ${CPP_FILES} ${UI_FILES} ${RCC_FILES})

# 添加 Qt5::Network 链接
target_link_libraries(${PROJECT_NAME} 
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
    Qt5::Charts
    Qt5::Network  # 关键：添加这一行
    Qt5::Multimedia  # 关键：添加这一行
)