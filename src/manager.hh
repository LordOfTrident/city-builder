#ifndef MANAGER_HH__HEADER_GUARD__
#define MANAGER_HH__HEADER_GUARD__

#include <unordered_map> // std::unordered_map

#include "utils.hh"

namespace CityBuilder {

template<typename T, typename Key>
class Manager {
public:
	T &Get(const Key &p_key) {
		try {
			return m_library.at(p_key);
		} catch (...) {
			Panic("Manager: Attempted to Get() non-existant key '", p_key, "'");
		}

		SILENCE_RETURN_WARNING();
	}

protected:
	void _Add(const Key &p_key, T &&p_val) {
		m_library.emplace(p_key, std::move(p_val));
	}

	void _Add(const Key &p_key, const T &p_val) {
		m_library.emplace(p_key, p_val);
	}

	std::unordered_map<Key, T> m_library;
};

}

#endif
