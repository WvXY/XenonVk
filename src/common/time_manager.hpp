#pragma once

#include <chrono>
#include <cmath>

namespace xev {

class TimeManager {
public:
  TimeManager()  = default;
  ~TimeManager() = default;

  void start() {
    frameCount  = 0;
    currentTime = std::chrono::high_resolution_clock::now();
    const auto startTime = currentTime;
    timeLag = 0.0f;
  }

  void init() { start(); }

  void end();

  void step() {
    auto newTime   = std::chrono::high_resolution_clock::now();
    frameTime = std::chrono::duration<float_t>(newTime - currentTime).count();
    currentTime    = newTime;
    timeLag += frameTime; // in seconds
    frameTime = std::min(frameTime, MAX_FRAME_TIME);
    frameCount++;
  }

  void pause();
  float getFps() const { return 1.f / frameTime; }

  int64_t frameCount = 0;
  std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
  std::chrono::time_point<std::chrono::high_resolution_clock> currentTime;
  float_t frameTime = MAX_FRAME_TIME;
  float_t timeLag;

  static constexpr float MAX_FRAME_TIME = 1.f / 60.f;
  static constexpr float FIXED_TIME_STEP = 1.f / 120.f;
};

} // namespace xev