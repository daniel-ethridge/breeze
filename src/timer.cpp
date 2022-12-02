#include "timer.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <sstream>

using namespace std::chrono;

namespace Breeze
{
  Timer::Timer(const char* title, int scoped, std::vector<TimerData> statsToCalc) :
      m_startTimePoint(),
      m_scoped(scoped),
      m_title(title),
      m_running(false),
      m_destroyed(false),
      m_statsToCalc(statsToCalc)
  {
    if (scoped)
      start();
  }

  Timer::~Timer()
  {
    m_destroyed = true;
    stop();
  }

  void Timer::start()
  {
    if (!m_running)
    {
      m_startTimePoint = high_resolution_clock::now();
      m_running = true;
    }
    else
    {
      std::cout << m_title << " - Timer already running.\n";
    }
  }

  void Timer::stop()
  {
    if (!m_scoped)
      std::cout << "---\n";

    // Make sure that if the timer is scoped, stop was called from the destructor
    if (m_scoped && !m_destroyed)
    {
      std::cout << m_title << " - Timer is scoped. It cannot be stopped manually.\n";
      return;
    }

    // Get the current time. Cast the start and stop times to numbers of ticks
    auto endTimePoint = high_resolution_clock::now();
    auto start = time_point_cast<microseconds>(m_startTimePoint).time_since_epoch().count();
    auto end = time_point_cast<microseconds>(endTimePoint).time_since_epoch().count();

    // Calculate the duration in milliseconds and save it to the vector
    double duration = (end - start) * 0.001;
    m_allTimes.push_back(duration);

    // create the prefix for messages
    std::stringstream prefix;
    prefix << m_title << " - ";
    if (!m_scoped)
    {
      prefix << " Iteration: " << m_allTimes.size() << " - ";
    }
    std::cout << prefix.str() << "Time elapsed: " << duration << "ms.\n";

    // Calculate the desired stats only if the timer is not scoped.
    // It doesn't make sense to calculate for a scoped timer.
    if (!m_scoped)
    {
      // Check if the mean should be calculated
      if (m_statsToCalc.hasFlag(TimerData::MEAN))
      {
        // Calculate the mean and print it to the console
        double mean = std::accumulate(m_allTimes.begin(), m_allTimes.end(), 0.0);
        mean /= m_allTimes.size();
        std::cout << prefix.str() << "Mean: " << mean << "ms.\n";
      }

      // Check if the mean should be calculated
      if (m_statsToCalc.hasFlag(TimerData::MEDIAN))
      {
        // Calculate the median and print it to the console
        auto temp = m_allTimes;
        std::sort(temp.begin(), temp.end());
        size_t tempSize = temp.size();
        if (tempSize % 2 == 0)
        {
          int idx = (tempSize) / 2;
          double median = (temp[idx] + temp[idx - 1]) / 2;
          std::cout << prefix.str() << "Median: " << median << "ms.\n";
        }
        else
        {
          int idx = (tempSize - 1) / 2;
          std::cout << prefix.str() << "Median: " << temp[idx] << "ms.\n";
        }
      }

      // Check if the mean should be calculated
      if (m_statsToCalc.hasFlag(TimerData::STD_DEV))
      {
        // Calculate the standard deviation and print it to the console
        double mean = std::accumulate(m_allTimes.begin(), m_allTimes.end(), 0.0);
        mean /= m_allTimes.size();

        std::vector<double> varVec(m_allTimes.size());
        for (auto e : m_allTimes)
          varVec.emplace_back(std::pow(e - mean, 2));

        double variance = std::accumulate(varVec.begin(), varVec.end(), 0.0);
        variance /= varVec.size();

        double stdDev = std::sqrt(variance);
        std::cout << prefix.str() << "Standard Deviation: " << stdDev << "ms.\n";
      }
    }

    if (!m_scoped)
      std::cout << "---\n";

    m_running = false;
  }

} // namespace Breeze