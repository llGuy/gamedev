#ifndef _MTIME_H_
#define _MTIME_H_

#include <chrono>
#include <iostream>

namespace mulgame {

    class Time
    {
    public:
	Time(double div = 1000000000.0)
	    : m_timeDelta(0.0), m_div(div)
	    {
	    }

	~Time(void)
	    {
	    }

	void StartTimer(void)
	    {
		using std::chrono::high_resolution_clock;
		m_currentFrame = high_resolution_clock::now();
	    }

	// resets the timer
	void Update(void)
	    {
		using std::chrono::high_resolution_clock;
		high_resolution_clock::time_point tp = high_resolution_clock::now();
		m_timeDelta = (static_cast<double>((tp - m_currentFrame).count()) / m_div);
		m_currentFrame = high_resolution_clock::now();
	    }

	double Difference(void)
	    {
		using std::chrono::high_resolution_clock;
		high_resolution_clock::time_point tp = high_resolution_clock::now();
		return (static_cast<double>((tp - m_currentFrame).count()) / m_div);
	    }

	inline
	float TimeDelta(void) const
	    {
		return static_cast<float>(m_timeDelta);
	    }
    private:
	std::chrono::high_resolution_clock::time_point m_currentFrame;
	double m_timeDelta;
	// number that you divide difference in time (nanoseconds) by
	// to get the difference in time in desired unit
	double m_div;
    };

}

#endif /* _MTIME_H_ */
