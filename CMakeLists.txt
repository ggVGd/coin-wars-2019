cmake_minimum_required(VERSION 3.0)

project(coin-wars-2019)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MODULE_PATH
	${CMAKE_SOURCE_DIR}/cmake/
	${CMAKE_MODULE_PATH}
)

set(CMAKE_CXX_STANDARD 14)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS NO_OUTPUT_DIRS)

file(GLOB_RECURSE sources src/*.cpp)

add_executable(coin-wars-2019 ${sources})

target_link_libraries(coin-wars-2019
	CONAN_PKG::boost_algorithm
	CONAN_PKG::boost_filesystem
	CONAN_PKG::box2d
	CONAN_PKG::glew
	CONAN_PKG::glm
	CONAN_PKG::jsonformoderncpp
	CONAN_PKG::sdl2
	CONAN_PKG::sqlite3
)

install(TARGETS coin-wars-2019 DESTINATION bin)
