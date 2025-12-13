
add_library(simulator SHARED ${PROJECT_SOURCE_DIR}/src/simulator.cpp)

target_include_directories(simulator PUBLIC ${PROJECT_SOURCE_DIR}/include)
