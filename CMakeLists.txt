cmake_minimum_required(VERSION 3.10)

project(sound_player_cpp LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.6 REQUIRED
  COMPONENTS
    audio)

include_directories(
  include
)

add_library(${PROJECT_NAME} SHARED
  src/${PROJECT_NAME}/player.cpp
)
target_link_libraries(${PROJECT_NAME}
  PUBLIC
    sfml-audio
)
install(
  TARGETS
    ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)
install(
  DIRECTORY include/
  DESTINATION include)


add_executable(${PROJECT_NAME}_sample
  src/${PROJECT_NAME}/sample.cpp
)
target_link_libraries(${PROJECT_NAME}_sample
  ${PROJECT_NAME}
)

install(EXPORT ${PROJECT_NAME}
  FILE ${PROJECT_NAME}-config.cmake
  DESTINATION share/cmake/${PROJECT_NAME}/
  EXPORT_LINK_INTERFACE_LIBRARIES)
