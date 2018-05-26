#ifndef _MTIME_H_
#define _MTIME_H_

#include <chrono>

namespace mulgame {

    class Time
    {
    public:
	Time(void)
	    : m_timeDelta(0.0)
	{
	}

	void Update(void)
	{
	    using std::chrono::high_resolution_clock;

	    if(m_timeDelta != 0.0)
	    {
		high_resolution_clock::time_point tp = high_resolution_clock::now();
		m_timeDelta = static_cast<double>((tp - m_currentFrame).count()) / 1000000000.0;
	    }

	    m_currentFrame = high_resolution_clock::now();
	}

	inline
	double TimeDelta(void) const
	{
	    return m_timeDelta;
	}
    private:
	std::chrono::high_resolution_clock::time_point m_currentFrame;
	double m_timeDelta;
    };

}

#endif /* _MTIME_H_ */
