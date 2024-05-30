#ifndef SOUND_PLAYER_CPP_PLAYER_HPP_
#define SOUND_PLAYER_CPP_PLAYER_HPP_
#include <memory>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <SFML/Audio.hpp>

namespace sound_player_cpp
{

class Player : public sf::SoundStream
{
public:
  using InputType = std::vector<unsigned char>;

  Player(const Player&) = delete;
  Player& operator=(const Player&) = delete;

  static Player& GetInstance();

  void SetAudioData(const InputType& data);
  void Play();
  void Stop();

protected:
  bool onGetData(Chunk& data) override;
  void onSeek(sf::Time time_offset) override;

private:
  Player();
  ~Player();

  const size_t buffer_size_;
  const uint32_t sample_rate_;
  const uint32_t channel_count_;
  size_t current_sample_index_;
  bool is_playing_;

  std::vector<sf::Int16> audio_data_;
  std::mutex mutex_;
};

}  // namespace sound_player_cpp
#endif  // SOUND_PLAYER_CPP_PLAYER_HPP_