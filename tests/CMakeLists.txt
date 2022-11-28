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


add_executable(Weather_test
    weather_test.cpp
    ${CMAKE_SOURCE_DIR}/lib/tinyxml/tinyxml2.cpp
    ${CMAKE_SOURCE_DIR}/APIC/apiclient.cpp
    ${CMAKE_SOURCE_DIR}/APIC/curlutil.cpp
    ${CMAKE_SOURCE_DIR}/model/weather.cpp
    ${CMAKE_SOURCE_DIR}/parsers/fmiparser.cpp
    ${CMAKE_SOURCE_DIR}/util.cpp
)

target_include_directories(Weather_test PUBLIC
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/lib
    ${CURL_INCLUDE_DIRS}
)

target_link_libraries(Weather_test PRIVATE
    Qt::Test
    CURL::libcurl
)

add_test(NAME Weather COMMAND Weather_test)


add_executable(timeStamp_test timestamp_test.cpp)
target_include_directories(timeStamp_test PUBLIC ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(timeStamp_test PRIVATE Qt::Test)
add_test(Name timeStamp COMMAND timeStamp_test)
