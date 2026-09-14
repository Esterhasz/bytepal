cmake_minimum_required (VERSION 3.10)
cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("include" "demos/include")
project ("bytepal")

file (GLOB_RECURSE SRC_FILES 
	"include/*.h"
	"include/*.hpp"
	
	"demos/*.hpp"
	"demos/*.h"
	"demos/include/*.hpp"
	"demos/include/*.h"

	"demos/*.cpp"
)

add_executable(bytepal ${SRC_FILES})

add_custom_command(
    TARGET bytepal POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/content"
            "$<TARGET_FILE_DIR:bytepal>/content"
)