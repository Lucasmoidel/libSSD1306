cmake_minimum_required(VERSION 2.6)
project(SSD1306)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++14")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

#--------------------------------------------------------------------------

add_library(SSD1306 STATIC lib/FileDescriptor.cxx
						   lib/OledHardware.cxx
						   lib/OledPixel.cxx
						   lib/OledFont8x8.cxx
						   lib/OledFont8x12.cxx
						   lib/OledFont8x16.cxx
						   lib/OledGraphics.cxx
						   lib/OledI2C.cxx)

include_directories(${PROJECT_SOURCE_DIR}/lib)

set(EXTRA_LIBS ${EXTRA_LIBS} HD44780Lcd)

#--------------------------------------------------------------------------

add_executable(info main.cpp time.cpp ipaddress.cpp users.cpp)
target_link_libraries(info SSD1306)