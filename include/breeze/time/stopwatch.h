#ifndef _BREEZE_STOPWATCH_H_
#define _BREEZE_STOPWATCH_H_

#include "breeze/bitflags.h"
#include <chrono>
#include <vector>

namespace Breeze
{
	namespace Time
	{
		class Stopwatch
		{

#define SCOPED 1
#define UNSCOPED 0

		public:
			enum class StopwatchStats
			{
				MEAN = 1 << 0,
				MEDIAN = 1 << 1,
				STD_DEV = 1 << 2,

				COUNT = 3
			};

			Stopwatch(const char* title, int scoped, std::vector<StopwatchStats> statsToCalc = {});
			~Stopwatch();

			void start();
			void stop();

		private:
			std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimePoint;
			const int m_scoped;
			const char* m_title;

			bool m_running;
			bool m_destroyed;

			BitFlags<StopwatchStats> m_statsToCalc;
			std::vector<double> m_allTimes;
		};
	}  // namespace Time
} // namespace Breeze

#endif // _BREEZE_STOPWATCH_H_
