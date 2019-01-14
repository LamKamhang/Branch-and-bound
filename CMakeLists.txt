cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project( branch_bound )

file(GLOB_RECURSE
    source_files ${PROJECT_SOURCE_DIR}/src/*/*.cpp
    header_files ${PROJECT_SOURCE_DIR}/src/*/*.h
)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

find_package(Eigen3 REQUIRED)

include_directories(
    ${EIGEN3_INCLUDE_DIR} 
    ${PROJECT_SOURCE_DIR}
)

add_executable(${PROJECT_NAME}
    ${source_files}
    ${header_files}
)

target_link_libraries(${PROJECT_NAME} Eigen3::Eigen)
