include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(hist INTERFACE)
target_sources(hist INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/histogram.hpp)
target_include_directories(hist INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
add_library(utils INTERFACE)
target_sources(utils INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/utils.hpp)
target_include_directories(utils INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

