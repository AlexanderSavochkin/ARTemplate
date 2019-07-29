cmake_minimum_required(VERSION 2.8.7)
project(SimpleAR)

file(GLOB_RECURSE SimpleAR_Sources ${SimpleAR_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE SimpleAR_Headers ${SimpleAR_SOURCE_DIR}/*.h)

cmake_minimum_required(VERSION 2.8.7)

find_package(OpenCV REQUIRED)
find_package(OpenSceneGraph REQUIRED COMPONENTS osgDB osgViewer)
#find_package(Eigen3 3.3 REQUIRED NO_MODULE)

# Also add some useful compiler flag
set(CMAKE_CXX_FLAGS
   "${CMAKE_CXX_FLAGS} -msse4.1 -msse3 -msse2 -msse -march=native -O3 -std=c++0x"
)

include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}
  ${OPENSCENEGRAPH_INCLUDE_DIRS}
  ${Eigen3_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

add_executable(SimpleAR ${SimpleAR_Sources})
target_link_libraries(SimpleAR ${OpenCV_LIBS} ${OPENSCENEGRAPH_LIBRARIES} ${OSGGA_LIBRARIES})

