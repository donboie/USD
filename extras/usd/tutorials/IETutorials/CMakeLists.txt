cmake_minimum_required (VERSION 2.8.11)
set (CMAKE_CXX_STANDARD 11)

project (USD-Tests)

set (CMAKE_CXX_FLAGS "-Wno-deprecated ${CMAKE_CXX_FLAGS}")

add_definitions(-DBUILD_COMPONENT_SRC_PREFIX="" -DBUILD_OPTLEVEL_DEV)

set(USD_ROOT_DIR /software/apps/usdmaya/2016.sp6-0.8.0/cent7.x86_64/)
set(USD_LIB_DIR ${USD_ROOT_DIR}/lib/)
set(USD_INC_DIR ${USD_ROOT_DIR}/include/)

set(TBB_ROOT /software/apps/tbb/4.4/cent7.x86_64/gcc/4.8.3/)
set(OPENSUBDIV_ROOT_DIR /software/apps/OpenSubdiv/3.1.1/cent7.x86_64/gcc/4.8.3)

find_package(PythonLibs)
find_package(OpenGL)
find_package(Boost COMPONENTS python)

add_subdirectory(hydra)
