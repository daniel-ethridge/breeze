#include "breeze/timer.h"
#include <iostream>
#include <thread>

using namespace Breeze;

int main(int argc, char* argv[])
{
  Timer timer("Testing", UNSCOPED,
              {Timer::TimerData::MEAN, Timer::TimerData::MEDIAN, Timer::TimerData::STD_DEV});
  for (int i = 1; i < 10; i++)
  {
    timer.start();
    std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(i * 100));
    timer.stop();
  }
}