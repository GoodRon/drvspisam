cmake_minimum_required(VERSION 3.3)
project(drvspisam)

add_definitions(-Wall -Wextra -Wno-missing-field-initializers -Wno-psabi)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_library(spi spi.cxx SpiDeviceData.cxx SpiPioMsg.cxx SpiAdcMsg.cxx SpiNavigatorMsg.cxx)
add_library(powerControl powerControl.cxx)

add_executable(drvspisam main.cxx Drvspisam.cxx)
target_link_libraries(drvspisam spi powerControl)

install(TARGETS drvspisam RUNTIME DESTINATION .)