#ifndef _BREEZE_TIMER_H_
#define _BREEZE_TIMER_H_

#include "breeze/bitflags.h"
#include <chrono>
#include <vector>

namespace Breeze
{
  class Timer
  {

#define SCOPED 1
#define UNSCOPED 0

  public:
    enum class TimerData
    {
      MEAN = 1 << 0,
      MEDIAN = 1 << 1,
      STD_DEV = 1 << 2,

      COUNT = 3
    };

    Timer(const char* title, int scoped, std::vector<TimerData> statsToCalc = {});
    ~Timer();

    void start();
    void stop();

  private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimePoint;
    const int m_scoped;
    const char* m_title;

    bool m_running;
    bool m_destroyed;

    BitFlags<TimerData> m_statsToCalc;
    std::vector<double> m_allTimes;
  };
} // namespace Breeze

#endif // _BREEZE_TIMER_H_