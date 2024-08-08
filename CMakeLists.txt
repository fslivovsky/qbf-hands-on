cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(DepQBFCC VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(DEPQBF_DIR ${PROJECT_SOURCE_DIR}/depqbf)

# Add the qdpll static library as an imported target
add_library(qdpll STATIC IMPORTED)

# Specify the location of the precompiled qdpll library
set_target_properties(qdpll PROPERTIES
  IMPORTED_LOCATION ${DEPQBF_DIR}/libqdpll.a
  INTERFACE_INCLUDE_DIRECTORIES ${DEPQBF_DIR}
  LINKER_LANGUAGE C
)

add_library(depqbf src/depqbf.h src/depqbf.cpp)
target_include_directories(depqbf PUBLIC ${DEPQBF_DIR})
target_link_libraries(depqbf qdpll)

add_executable(main src/main.cpp)
target_link_libraries(main depqbf)
