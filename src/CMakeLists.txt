project(unzipLIB)
add_library(unzipLIB STATIC)

file(GLOB LIB_C_SRC  *.c *.cpp)
target_sources(unzipLIB PRIVATE ${LIB_C_SRC})
target_include_directories(unzipLIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)