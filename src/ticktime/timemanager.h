#pragma once
#include <chrono>
#include <log/log.h>
#include "application/application.h"
#include "utils/common/singleton.h"

namespace WhiteDeer {
namespace Engine {

using namespace std::chrono;
using Utils::Singleton;

class TimeManager : public Singleton<TimeManager> {
  using GameClock = high_resolution_clock;
  using GameTimeUnit = microseconds;
  using GameTimePoint = time_point<high_resolution_clock, GameTimeUnit>;

  friend class Singleton<TimeManager>;
  friend class Application;

 public:
  GameTimePoint GetTimePoint() {
    return time_point_cast<GameTimeUnit>(GameClock::now());
  }


  // internal unit is microseconds, outer unit is milliseconds
  double GetGameTime() {
    auto current = GetTimePoint();
    auto delta = current - m_startTime;
    return delta.count() / 1000.0;
  }

 protected:
  TimeManager() { m_startTime = GetTimePoint(); };

  void Tick() {
    auto lastFrameTime = m_curFrameTime;
    m_curFrameTime = GetTimePoint();
    if (++m_frameCount == 1) {
        return;
    }

    m_delta = (m_curFrameTime - lastFrameTime).count() / 1000.0;
    // todo: call registry func
  }

  GameTimePoint m_startTime;
  GameTimePoint m_curFrameTime;
  double m_delta = 0;
  long long m_frameCount = 0;
};

}  // namespace Engine
}  // namespace WhiteDeer
