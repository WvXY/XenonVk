#pragma once

#include <chrono>
#include <cmath>

namespace xev {

typedef std::chrono::time_point<std::chrono::high_resolution_clock> time_point;

class TimeManager {
public:
  TimeManager()  = default;
  ~TimeManager() = default;

  static constexpr float MAX_FRAME_TIME  = 1.f / 60.f;
  static constexpr float FIXED_TIME_STEP = 1.f / 120.f;

  void start() {
    if (isRunning) { return; }
    isRunning = true;
    init();
  }

  void end() { isRunning = false; };

  void step() {
    auto newTime = std::chrono::high_resolution_clock::now();
    frameTime    = std::chrono::duration<float>(newTime - currentTime).count();
    currentTime  = newTime;
    timeLag += frameTime; // in seconds
    frameTime = std::min(frameTime, MAX_FRAME_TIME);
    frameCount++;
  }

  void pause() {
    if (!isPaused) { return; }
    isPaused   = true;
    pausedTime = std::chrono::high_resolution_clock::now();
  }

  void resume() {
    if (!isPaused) { return; }
    isPaused = false;
    offset += duration(pausedTime, std::chrono::high_resolution_clock::now());
  }

  float& getTimeLag() { return timeLag; }
  float& getFrameTime() { return frameTime; }
  float getFps() const { return 1.f / frameTime; }
  int64_t& getFrameCount() { return frameCount; }
  float getTimeSinceStart() const { return duration(startTime, currentTime); }
  float getGameTimeSinceStart() const {
    return duration(startTime, currentTime) - offset;
  }

private:
  int64_t frameCount = 0;
  time_point startTime;
  time_point currentTime;
  time_point pausedTime;
  float frameTime = MAX_FRAME_TIME;
  float timeLag;
  float offset;

  bool isRunning = false;
  bool isPaused  = false;

  void init() {
    frameCount           = 0;
    currentTime          = std::chrono::high_resolution_clock::now();
    const auto startTime = currentTime;
    timeLag              = 0.0f;
  }

  static float duration(time_point start, time_point end) {
    return std::chrono::duration<float>(end - start).count();
  }
};

} // namespace xev