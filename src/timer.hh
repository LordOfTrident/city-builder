#ifndef TIMER_HH__HEADER_GUARD__
#define TIMER_HH__HEADER_GUARD__

#include <functional> // std::function

#include "utils.hh"

namespace CityBuilder {

template<size_t Count>
class TimerHandler {
public:
	using EndEvent = std::function<void()>;
	using ID       = size_t;

	struct Timer {
		Timer(): now(0), time(0) {}

		size_t   now, time;
		EndEvent envEventHandler;
	};

	void Init(ID p_id, size_t p_time) {
		m_timers[p_id].time = p_time;
	}

	void Init(ID p_id, size_t p_time, const EndEvent &p_endEventHandler) {
		m_timers[p_id].time            = p_time;
		m_timers[p_id].envEventHandler = p_endEventHandler;
	}

	void Start(ID p_id) {
		m_timers[p_id].now = m_timers[p_id].time;
	}

	float GetUnit(ID p_id, bool p_reversed = false) {
		float tmp = p_reversed? m_timers[p_id].time - m_timers[p_id].now : m_timers[p_id].now;

		return tmp / m_timers[p_id].time;
	}

	size_t Get(ID p_id) {
		return m_timers[p_id].now;
	}

	bool Active(ID p_id) {
		return m_timers[p_id].now > 0;
	}

	void Update() {
		for (size_t i = 0; i < Count; ++ i) {
			if (m_timers[i].now > 0) {
				-- m_timers[i].now;

				if (m_timers[i].now == 0 and m_timers[i].envEventHandler)
					m_timers[i].envEventHandler();
			}
		}
	}

private:
	Timer m_timers[Count];
};

}

#endif
