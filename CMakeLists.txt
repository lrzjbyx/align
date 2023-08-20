cmake_minimum_required(VERSION 3.8)
project(sealnet_align)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)


# Find OpenCV
find_package(OpenCV REQUIRED)

# Find nlohmann json
find_package(nlohmann_json 3.6.0 REQUIRED)

# find pybind11
find_package(pybind11)
# 寻找Boost库
find_package(Boost REQUIRED COMPONENTS python)

# 设定Python版本，根据你的情况设定
set(Python_ADDITIONAL_VERSIONS 3.8)

# 包含头文件路径
include_directories(${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIRS})


# add_executable(SEALNET_ALIGN main.cpp align.cpp sealnet_align.cpp)
add_library(sealnet_align MODULE align.cpp sealnet_align.cpp)

# Link OpenCV libraries
# 链接 Pybind11
target_link_libraries(sealnet_align PRIVATE pybind11::module ${PYTHON_LIBRARIES} ${OpenCV_LIBS} nlohmann_json::nlohmann_json)
set_target_properties(sealnet_align PROPERTIES PREFIX "")

