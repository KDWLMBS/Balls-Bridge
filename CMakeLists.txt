cmake_minimum_required (VERSION 3.0)

set(SOURCE
   ${SOURCE}
   ${CMAKE_CURRENT_SOURCE_DIR}/src/motor.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/bridge.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/controller.cpp
)
set(HEADERS
   ${HEADERS}
   ${CMAKE_CURRENT_SOURCE_DIR}/src/motor.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/controller.hpp
)


add_executable(bridge ${SOURCE} ${HEADERS})
add_executable(bridge-rpi ${SOURCE} ${HEADERS})
target_compile_definitions(bridge-rpi PRIVATE RPI=true)
target_link_libraries(bridge-rpi wiringPi)
