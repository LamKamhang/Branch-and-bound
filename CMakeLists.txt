cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project( branch_bound )

set (CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE
    source_files ${PROJECT_SOURCE_DIR}/src/Common/*.cpp ${PROJECT_SOURCE_DIR}/src/BranchBound/*.cpp ${PROJECT_SOURCE_DIR}/src/SimplexSolver/*.cpp ${PROJECT_SOURCE_DIR}/src/main.cpp
    header_files ${PROJECT_SOURCE_DIR}/src/Common/*.h ${PROJECT_SOURCE_DIR}/src/BranchBound/*.h ${PROJECT_SOURCE_DIR}/src/SimplexSolver/*.h
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

add_executable( parse
    ${PROJECT_SOURCE_DIR}/src/parse.cpp
)

target_link_libraries(${PROJECT_NAME} Eigen3::Eigen)
