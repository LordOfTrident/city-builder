#ifndef MATH_HH__HEADER_GUARD__
#define MATH_HH__HEADER_GUARD__

#include <cmath>
#include <random> // std::mt19937, std::random_device, std::uniform_real_distribution,
                  // std::uniform_int_distribution

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

namespace CityBuilder {

template<typename T>
T Lerp(const T &p_a, const T &p_b, const T &p_t) {
	return p_a * (T(1.0) - p_t) + (p_b * p_t);
}

class Rand {
public:
	Rand():
		m_mt(m_rd())
	{}

	template<typename T>
	T Range(T p_min, T p_max) {
		if constexpr (std::is_floating_point<T>::value) {
			std::uniform_real_distribution<T> range(p_min, p_max);

			return range(m_mt);
		} else {
			std::uniform_int_distribution<T> range(p_min, p_max);

			return range(m_mt);
		}
	}

private:
	std::random_device m_rd;
	std::mt19937       m_mt;
};

}

#endif
