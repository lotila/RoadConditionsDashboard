cmake_minimum_required(VERSION 3.16)

project(RoadConditionsDashboard)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_AUTOMOC True)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
find_package(CURL REQUIRED)

if(NOT ${CURL_FOUND})
    message(FATAL_ERROR "libcurl not found, exiting")
    return()
endif()

qt_standard_project_setup()

set(PROJECT_SOURCES
    main.cpp
    controller.cpp
    mainwindow.cpp
    model/weather.cpp
    model/maintenance.cpp
    model/roadcondition.cpp
    model/trafficmessages.cpp
    APIC/curlutil.cpp
    APIC/apiclient.cpp
    parsers/digitraffigparser.cpp
    parsers/fmiparser.cpp
    lib/tinyxml/tinyxml2.cpp
)

add_executable(${CMAKE_PROJECT_NAME}
    ${PROJECT_SOURCES}
)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/lib
    ${CURL_INCLUDE_DIRS}
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    Qt6::Widgets
    Qt::Core
    CURL::libcurl
)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
)
