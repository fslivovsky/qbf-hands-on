cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(DepQBFCC VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set PYBIND11_FINDPYTHON to ON to use the modern FindPython module
set(PYBIND11_FINDPYTHON ON)

# Find PyBind11
find_package(pybind11 REQUIRED)

set(DEPQBF_DIR ${PROJECT_SOURCE_DIR}/depqbf)

# Set the desired compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

# Pass the CFLAGS to the compile.sh script
add_custom_target(run_compile_sh ALL
    COMMAND ${CMAKE_COMMAND} -E echo "Running compile.sh with CFLAGS=${CMAKE_C_FLAGS}..."
    COMMAND ${CMAKE_COMMAND} -E chdir ${DEPQBF_DIR} env CFLAGS=${CMAKE_C_FLAGS} ./compile.sh
    WORKING_DIRECTORY ${DEPQBF_DIR}
    COMMENT "Compiling the qdpll library using compile.sh"
)

# Add the qdpll static library as an imported target
add_library(qdpll SHARED IMPORTED)
add_dependencies(qdpll run_compile_sh)

# Specify the location of the precompiled qdpll library
set_target_properties(qdpll PROPERTIES
  IMPORTED_LOCATION ${DEPQBF_DIR}/libqdpll.so.1.0
  INTERFACE_INCLUDE_DIRECTORIES ${DEPQBF_DIR}
  LINKER_LANGUAGE C
)

add_library(nenofex SHARED IMPORTED)
set_target_properties(nenofex PROPERTIES
  IMPORTED_LOCATION ${DEPQBF_DIR}/nenofex/libnenofex.so.1.1
  INTERFACE_INCLUDE_DIRECTORIES ${DEPQBF_DIR}/nenofex
  LINKER_LANGUAGE C
)

add_library(picosat STATIC IMPORTED)
set_target_properties(picosat PROPERTIES
  IMPORTED_LOCATION ${DEPQBF_DIR}/picosat/libpicosat.a
  INTERFACE_INCLUDE_DIRECTORIES ${DEPQBF_DIR}/picosat
  LINKER_LANGUAGE C
)

add_library(depqbf src/depqbf.h src/depqbf.cpp)
target_include_directories(depqbf PUBLIC ${DEPQBF_DIR})
target_link_libraries(depqbf PUBLIC qdpll nenofex picosat)

pybind11_add_module(depqbf_module src/depqbf_pybind.cpp)
target_link_libraries(depqbf_module PRIVATE depqbf)

# Embed the RPATH to ensure the Python module can find the libraries
set_target_properties(depqbf_module PROPERTIES
  BUILD_RPATH "${DEPQBF_DIR};${DEPQBF_DIR}/nenofex"
  INSTALL_RPATH "${DEPQBF_DIR};${DEPQBF_DIR}/nenofex"
  BUILD_WITH_INSTALL_RPATH TRUE
)

add_custom_command(TARGET depqbf POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${DEPQBF_DIR}/nenofex/libnenofex.so.1.1 ${DEPQBF_DIR}/nenofex/libnenofex.so.1
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${DEPQBF_DIR}/libqdpll.so.1.0 ${DEPQBF_DIR}/libqdpll.so.1
  COMMENT "Creating symlinks for ${LIB_NAME} shared library"
)

add_executable(main src/main.cpp)
target_link_libraries(main depqbf)
