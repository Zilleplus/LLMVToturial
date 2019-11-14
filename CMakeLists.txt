### make sure the user is doing an our of source build:
if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR "In-source builds are not allowed. use cmake -H. -Bbuild instead to build in the build folder" )
endif()
###

#if(MSVC)
message("Enableling latst C++ features !")
add_compile_options("/std:c++latest")
#else()
#add_compile_options("-std=c++1z")
#endif()

cmake_minimum_required (VERSION 2.6)
project (LLVMDemo)

include_directories(include)
include_directories(lib/catch2)
include_directories(lib/llvm)

add_executable(main src/main.cpp)
add_executable(testMain tests/testMain.cpp)


#enable_testing()
#add_test(testMain testMain)
