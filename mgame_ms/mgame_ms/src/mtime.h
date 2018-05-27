#ifndef _MTIME_H_
#define _MTIME_H_

#include <chrono>
#include <iostream>

namespace mulgame {

    class Time
    {
    public:
		Time(void)
		    : m_timeDelta(0.0)
		{
		}

		~Time(void)
		{
			std::cout << 1 / (m_timeDelta) << std::endl;
			//std::cin.get();
		}

		void StartTimer(void)
		{
			using std::chrono::high_resolution_clock;
			m_currentFrame = high_resolution_clock::now();
		}

		void Update(void)
		{
		    using std::chrono::high_resolution_clock;

			high_resolution_clock::time_point tp = high_resolution_clock::now();
			m_timeDelta = (static_cast<double>((tp - m_currentFrame).count()) / 1000000000.0);

		    m_currentFrame = high_resolution_clock::now();
		}

		inline
		float TimeDelta(void) const
		{
		    return static_cast<float>(m_timeDelta);
		}
    private:
		std::chrono::high_resolution_clock::time_point m_currentFrame;
		double m_timeDelta;
    };

}

#endif /* _MTIME_H_ */
