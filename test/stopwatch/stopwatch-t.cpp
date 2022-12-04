#include "breeze/time/stopwatch.h"
#include <iostream>
#include <thread>

using namespace Breeze::Time;

int main(int argc, char* argv[])
{
  Stopwatch stopwatch("Testing", UNSCOPED,
                      {Stopwatch::StopwatchStats::MEAN, Stopwatch::StopwatchStats::MEDIAN,
                       Stopwatch::StopwatchStats::STD_DEV});
  for (int i = 1; i <= 3; i++)
  {
    std::cout << "Start Called: " << i << "\n";
    stopwatch.start();
    std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(i * 100));
    stopwatch.stop();
  }
}
