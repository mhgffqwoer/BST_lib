cmake_minimum_required(VERSION 3.16)

project(
	Bst 
	VERSION 1.0
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(bin)
add_subdirectory(lib)

enable_testing()
add_subdirectory(tests)