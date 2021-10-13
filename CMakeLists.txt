cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(simple-sqlite-orm)

find_package(Threads REQUIRED)

## Compiler flags
set(CXX_COVERAGE_COMPILE_FLAGS "-std=c++11")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${CXX_COVERAGE_COMPILE_FLAGS}")
set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} ${C_COVERAGE_COMPILE_FLAGS}")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CXX_COVERAGE_COMPILE_FLAGS} -O0")


if (CMAKE_BUILD_TYPE STREQUAL Debug)
	message("++ MAKE IN DEBUG MODE")
        message("++ CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS_DEBUG}")
        message("++ CMAKE_C_FLAGS: ${CMAKE_C_FLAGS_DEBUG}")
elseif (CMAKE_BUILD_TYPE STREQUAL Release)
	message("++ MAKE IN RELEASE MODE")
        message("++ CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
        message("++ CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
endif (CMAKE_BUILD_TYPE STREQUAL Debug)

## Sources
set (SRC_DIR src)
file (GLOB SOURCES ${SRC_DIR}/*.c*)
file (GLOB HEADERS ${SRC_DIR}/*.h)


# ${PROJECT_NAME}Config.h will be generated and located under
# binary_dir
include_directories("${PROJECT_BINARY_DIR}")

link_directories("${PROJECT_SOURCE_DIR}/requirements/lib")
include_directories("${PROJECT_SOURCE_DIR}/requirements/include")

## Target
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} 
    pthread
    zf_log
    sqlite3
    dl
)



