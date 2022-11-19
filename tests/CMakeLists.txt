enable_testing(true)

find_package(Qt6 REQUIRED COMPONENTS Test)

add_executable(TrafficMessageCount_test
    traficmessagecount_test.cpp
    ${CMAKE_SOURCE_DIR}/APIC/apiclient.cpp
    ${CMAKE_SOURCE_DIR}/APIC/curlutil.cpp
    ${CMAKE_SOURCE_DIR}/model/trafficmessages.cpp
    ${CMAKE_SOURCE_DIR}/parsers/digitraffigparser.cpp
    ${CMAKE_SOURCE_DIR}/util.cpp
)

target_include_directories(TrafficMessageCount_test PUBLIC
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/lib
    ${CURL_INCLUDE_DIRS}
)

target_link_libraries(TrafficMessageCount_test PRIVATE
    Qt::Test
    CURL::libcurl
)

add_test(NAME TrafficMessageCount COMMAND TrafficMessageCount_test)
