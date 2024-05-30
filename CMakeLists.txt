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
  sfml-audio
)
