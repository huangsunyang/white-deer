#pragma once
#include <chrono>

#include "utils/common/singleton.h"

namespace WhiteDeer {
namespace Engine {

using namespace std::chrono;
using Utils::Singleton;

class TimeManager : public Singleton<TimeManager> {
  using GameClock = high_resolution_clock;
  using GameTimeUnit = milliseconds;
  using GameTimePoint = time_point<high_resolution_clock, GameTimeUnit>;

  friend class Singleton<TimeManager>;

 public:
  GameTimePoint GetTimePoint() {
    return time_point_cast<GameTimeUnit>(GameClock::now());
  }

  long long GetGameTime() {
    auto current = GetTimePoint();
    auto delta = current - m_startTime;
    return delta.count();
  }

 protected:
  TimeManager(){
    m_startTime = GetTimePoint();
  };


  GameTimePoint m_startTime;
};

}  // namespace Engine
}  // namespace WhiteDeer
