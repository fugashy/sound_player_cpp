#include "sound_player_cpp/player.hpp"

#include <iostream>


int main(int argc, char** argv)
{
  if (argc == 1)
  {
    std::cerr << "Usage: sound_player_cpp_sample FILE_PATH" << std::endl;
    return EXIT_FAILURE;
  }
  std::string file_path = argv[1];

  sound_player_cpp::Player::GetInstance().SetAudioData(file_path);
  sound_player_cpp::Player::GetInstance().Play();
  sound_player_cpp::Player::GetInstance().WaitForEnd();
  return EXIT_SUCCESS;
}
