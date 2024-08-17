cmake_minimum_required(VERSION 3.5)

# idf_component_register(
#      SRCS src/adler32.c src/crc32.c src/inflate.c src/unzip.c src/infback.c
#      src/zutil.c src/inftrees.c src/inffast.c src/unzipLIB.cpp
#      INCLUDE_DIRS "src"
#      REQUIRES arduino
# )

# set_source_files_properties(
#      src/inflate.c src/infback.c
#     PROPERTIES COMPILE_FLAGS
#     -Wno-implicit-fallthrough
# )

project(test)
add_executable(test Linux/main.c)

add_subdirectory(src)

target_link_libraries(test unzipLIB)

project(test_cpp)
add_executable(test_cpp Linux/test_cpp.cpp)

target_link_libraries(test_cpp unzipLIB)