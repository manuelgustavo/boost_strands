cmake_minimum_required(VERSION 3.15.0)

# include(CTest)
enable_testing()

message("strands1")

set(HEADERS
)

add_executable(strands1
  ${HEADERS}
  src/strands1.cpp
)

target_link_libraries(strands1 PRIVATE
  boost_thread
)

target_include_directories(strands1
  PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include/secretarium>"
)

target_compile_definitions(strands1 PRIVATE ${SHARED_DEFINES})
target_compile_options(strands1 PRIVATE ${COMPILE_OPTIONS})


#############################################################

message("strands2")

set(HEADERS
)

add_executable(strands2
  ${HEADERS}
  src/strands2.cpp
)

target_link_libraries(strands2 PRIVATE
  boost_thread
)

target_include_directories(strands2
  PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include/secretarium>"
)

target_compile_definitions(strands2 PRIVATE ${SHARED_DEFINES})
target_compile_options(strands2 PRIVATE ${COMPILE_OPTIONS})
