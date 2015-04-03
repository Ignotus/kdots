cmake_minimum_required(VERSION 2.8.12)

project(example)

find_package(ECM REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fexceptions")

include(KDEInstallDirs)
include(KDECompilerSettings)

set(CMAKE_AUTOMOC ON)

find_package(KDotsLib REQUIRED)

find_package(KF5 REQUIRED COMPONENTS
  KDELibs4Support
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${KDOTSLIB_INCLUDE_DIRS}
)
  
add_library(kdots_example MODULE plugin.cpp)

target_link_libraries(kdots_example
  KF5::KDELibs4Support
  ${KDOTSLIB_LIBRARIES}
  )