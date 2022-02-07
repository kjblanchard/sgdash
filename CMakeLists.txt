#Requirements and definition
cmake_minimum_required(VERSION 3.21.1)
project(geometrydash VERSION 0.0.2)

#Include directories for intellisense/compiling
include_directories(
    "/usr/local/Cellar/sdl2/2.0.20/include"
    "/usr/local/Cellar/sdl2_image/2.0.5/include"
    "/usr/local/Cellar/sdl2_ttf/2.0.18/include"
    "/usr/local/Cellar/lua/5.4.4/include/lua"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/"
)

#Get all files and headers from the source folder.
file(GLOB FILES "src/**/*.?pp" "src/*.?pp")

#Use those files to make the main executable.
add_executable(main ${FILES})

#Define library names / references to their link files that we depend on, shared is dynamic linking.  Imported needs a specific location.
#This defines
set(libnames libfmod libfmodstudio libSDL2 libSDL2_image liblua.5.4 libSDL2_ttf)
foreach(library ${libnames})
        add_library(${library} SHARED IMPORTED)
endforeach()

foreach(library ${libnames})
    set_target_properties( ${library}
                            PROPERTIES IMPORTED_LOCATION
                            /usr/local/lib/${library}.dylib
                            )
endforeach()

#cpp17
add_compile_options(-Wall -Wfatal-errors)
set_property(TARGET main PROPERTY CXX_STANDARD 17)

#Link all the dynamic libraries to the executable, based on the libnames var.
target_link_libraries(main PUBLIC ${libnames})

#Copy the assets folder to the build folder.
add_custom_command(
        TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/assets
                ${CMAKE_CURRENT_BINARY_DIR}/assets)