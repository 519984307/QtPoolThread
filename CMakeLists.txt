cmake_minimum_required(VERSION 3.5)

project(QtPoolThread VERSION 0.1 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)



find_package(QT NAMES Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

include( ${CMAKE_CURRENT_LIST_DIR}/pool_thread/poolthread.cmake )
include( ${CMAKE_CURRENT_LIST_DIR}/test/test.cmake )
include( ${CMAKE_CURRENT_LIST_DIR}/timer/timer.cmake )

add_executable(${PROJECT_NAME}
    ${POOL_THREAD}
    ${TIMER}
    ${TEST}
    main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

