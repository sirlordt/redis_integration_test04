cmake_minimum_required(VERSION 3.11-3.18)

project( REDIS_PLUS_PLUS )

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a") # <--- Change the preference to .a and not .so/.dll

# MAIN_ROOT_SOURCE_DIR come from the top most CMakeLists.txt file
message(STATUS "MAIN_ROOT_SOURCE_DIR=${MAIN_ROOT_SOURCE_DIR}")
#message(STATUS "CMAKE_SYSTEM_INCLUDE_PATH=${CMAKE_SYSTEM_INCLUDE_PATH}")

# Emulate the ExternalProject_Add but in config time.
# Because the Fetch_Declare, Fetch_Populate, Fetch_* no work. Never create the static libraries.
if ( NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/build" OR
     NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/install" OR
     FORCE_BUILD_INSTALL_SOCI_LIBRARY )

  if ( NOT EXISTS "${PROJECT_SOURCE_DIR}/git_repo/CMakeLists.txt" )

    message(STATUS "PROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}")
    execute_process( COMMAND git clone https://github.com/sewenew/redis-plus-plus ${PROJECT_SOURCE_DIR}/git_repo )

  endif()

  file( MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/install )
  file( MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/build )

  set(ENV{NO_COLOR} 1)
  execute_process( COMMAND cmake ${CMAKE_CURRENT_SOURCE_DIR}/git_repo -DREDIS_PLUS_PLUS_BUILD_TEST:BOOL=OFF
                                                                      -DCMAKE_COLOR_MAKEFILE:BOOL=OFF
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/build )

  execute_process( COMMAND cmake --build ${CMAKE_CURRENT_BINARY_DIR}/build )

  execute_process( COMMAND cmake --install ${CMAKE_CURRENT_BINARY_DIR}/build
                                 --prefix "${CMAKE_CURRENT_BINARY_DIR}/install" )

endif()

# Manual create the target
add_library( ${PROJECT_NAME} STATIC IMPORTED GLOBAL )
set_target_properties( ${PROJECT_NAME} PROPERTIES IMPORTED_LOCATION
                       ${CMAKE_CURRENT_BINARY_DIR}/install/lib/libredis++.a )

target_include_directories( ${PROJECT_NAME} INTERFACE
                            ${CMAKE_CURRENT_BINARY_DIR}/install/include )

target_link_libraries( ${PROJECT_NAME} INTERFACE hiredis )
