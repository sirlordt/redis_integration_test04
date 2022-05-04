# version 3.11 or later of CMake or needed later for installing GoogleTest
# so let's require it now.
cmake_minimum_required( VERSION 3.11-3.18 )

project( main )

set( CMAKE_FIND_LIBRARY_SUFFIXES ".a" ) # <--- Change the preference to .a and not .so/.dll
#set( BUILD_SHARED_LIBS OFF )
set( CMAKE_EXE_LINKER_FLAGS "-static" ) # <-- full static executable
#set( CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++" ) # valgrind detect a memoty leak of 72 kb. Is ok is gnu standar library c++ memory leak
#set( CMAKE_EXE_LINKER_FLAGS "-fsanitize=address" ) # <-- active the address sanitizer in development

#SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native" ) #Required by uuid gen

file( GLOB_RECURSE SOURCES "src/main_01.cpp" )

add_executable( ${PROJECT_NAME} ${SOURCES} )

target_link_libraries( ${PROJECT_NAME} PRIVATE HIREDIS )

# -static-libgcc -static-libstdc++

# Tell C++ compiler to use C++20 features. We don't actually use any of them.
target_compile_features( ${PROJECT_NAME} PUBLIC cxx_std_20 )
