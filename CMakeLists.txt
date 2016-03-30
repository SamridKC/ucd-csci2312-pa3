cmake_minimum_required(VERSION 3.3)
project(ucd_csci2312_pa3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    Cluster.h
    ClusteringTests.cpp
    ClusteringTests.h
    ErrorContext.cpp
    ErrorContext.h
    Exceptions.h
    KMeans.h
    main.cpp
    Point.h Cluster.cpp Point.cpp Exceptions.cpp KMeans.cpp)

add_executable(ucd_csci2312_pa3 ${SOURCE_FILES})