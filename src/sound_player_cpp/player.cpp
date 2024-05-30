#include "sound_player_cpp/player.hpp"

#include <chrono>
#include <iostream>
#include <fstream>
#include <thread>

namespace
{
constexpr size_t kBufferSize = 4096;
constexpr uint32_t kSampleRate = 12280;
constexpr uint32_t kChannelCount = 2;  // stereo
}


namespace sound_player_cpp
{

Player& Player::GetInstance()
{
  static Player instance;
  return instance;
}

Player::Player()
  : buffer_size_(kBufferSize),
    sample_rate_(kSampleRate),
    channel_count_(kChannelCount),
    current_sample_index_(0),
    is_playing_(false)
{
  this->initialize(channel_count_, sample_rate_);
  this->setPlayingOffset(sf::Time::Zero);
  this->setLoop(false);
  this->setVolume(100.f);
}

Player::~Player()
{
  this->stop();
}

bool Player::SetAudioData(const std::string& file_path)
{
  std::ifstream file(file_path, std::ios::binary);
  if (!file)
  {
    std::cerr << "Failed to open: " << file_path.c_str() << std::endl;
    return false;
  }

  // header
//char buffer[4];
//file.read(buffer, 4);
//if (std::strncmp(buffer, "RIFF", 4) != 0)
//{
//  std::cerr << "Not a valid WAV file: " << file_path << std::endl;
//  return 0;
//}
//std::cout << "file header: " << buffer << std::endl;

  InputType data(
      (std::istreambuf_iterator<char>(file)),
      std::istreambuf_iterator<char>());
  return this->SetAudioData(data);
}

bool Player::SetAudioData(const InputType& audio_data)
{
  if (audio_data.empty()) return false;

  std::lock_guard<std::mutex> lock(mutex_);
  audio_data_.clear();
  audio_data_.reserve(audio_data.size() / 2);
  for (size_t i = 0; i < audio_data.size(); i += 2)
  {
    const sf::Int16 sample = static_cast<sf::Int16>(
        (audio_data[i + 1] << 8) | audio_data[i]);
    audio_data_.push_back(sample);
  }
  current_sample_index_ = 0;
  return true;
}

void Player::Play()
{
  if (is_playing_) return;

  is_playing_ = true;
  this->play();
}


void Player::Stop()
{
  if (!is_playing_) return;

  is_playing_ = false;
  this->stop();
}

bool Player::onGetData(Chunk& data)
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (current_sample_index_ > audio_data_.size()) return false;

  const size_t sample_to_stream = std::min(
      buffer_size_, audio_data_.size() - current_sample_index_);
  data.samples = &audio_data_[current_sample_index_];
  data.sampleCount = sample_to_stream;
  current_sample_index_ += sample_to_stream;
  return true;
}

void Player::onSeek(sf::Time time_offset)
{
  std::lock_guard<std::mutex> lock(mutex_);
  current_sample_index_ = time_offset.asMilliseconds() * sample_rate_ / 1000 * channel_count_;
}

void Player::WaitForEnd() const
{
  while (this->getStatus() == sf::SoundStream::Playing)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
}

}  // namespace sound_player_cpp
