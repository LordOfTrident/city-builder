#ifndef UTILS_HH__HEADER_GUARD__
#define UTILS_HH__HEADER_GUARD__

#include <iostream> // std::cerr, std::endl
#include <string>   // std::string
#include <cstring>  // std::strlen
#include <cstdlib>  // std::exit, EXIT_FAILURE, std::size_t
#include <fstream>  // std::ifstream, std::ofstream, std::getline
#include <ostream>  // std::ostream
#include <sstream>  // std::stringstream
#include <variant>  // std::variant, std::get
#include <cstdint>  // std::int8_t, std::uint8_t, std::int16_t, std::uint16_t,
                    // std::int32_t, std::uint32_t, std::int64_t, std::uint64_t

#include "main/config.hh"

#define UNREACHABLE() CityBuilder::Panic(__FILE__, ":", __LINE__, ": Unreachable")
#define TODO(...)     CityBuilder::Panic(__FILE__, ":", __LINE__, " TODO: ", __VA_ARGS__)
#define UNUSED(P_VAR) (void)P_VAR

#define __FUNC__ __func__

// To silence compiler errors complaining about no return after a Panic statement
#define SILENCE_RETURN_WARNING() std::exit(EXIT_FAILURE)

#define TO_STR(P_X)    __TO_STR(P_X)
#define __TO_STR(P_X)  #P_X
#define STR_LEN(P_STR) std::strlen(P_STR)

#define ARR_SIZE(P_ARR) (sizeof(P_ARR) / sizeof(P_ARR[0]))

namespace CityBuilder {

using std::size_t;

using std::int8_t;
using std::uint8_t;

using std::int16_t;
using std::uint16_t;

using std::int32_t;
using std::uint32_t;

using std::int64_t;
using std::uint64_t;

template<typename... Args>
std::string String(Args... p_args) {
	std::stringstream ss;

	((ss << p_args), ...);

	return ss.str();
}

template<typename... Args>
void Panic(Args... p_args) {
	std::cerr << "Panic: ";
	((std::cerr << p_args), ...);
	std::cerr << std::endl;

	std::exit(EXIT_FAILURE);
}

#ifdef CITY_BUILDER_LOG
extern std::ostream *g_logStream;

inline void LogInto(std::ostream &p_stream) {
	g_logStream = &p_stream;
}

template<typename... Args>
void Log(Args... p_args) {
	if (g_logStream == nullptr)
		Panic("Log() called, but log stream is nullptr");

	(*g_logStream) << "LOG: ";
	(((*g_logStream) << p_args), ...);
	(*g_logStream) << "\n";
}
#endif

struct Error {
	static inline Error Fine() {
		return Error();
	}

	template<typename... Args>
	static inline Error Make(Args... p_args) {
		std::stringstream ss;

		((ss << p_args), ...);

		return Error(ss.str());
	}

	const std::string &Desc() const {
		return m_desc;
	}

	bool Ok() const {
		return m_ok;
	}

private:
	Error():                          m_ok(true),  m_desc("")     {}
	Error(const std::string &p_desc): m_ok(false), m_desc(p_desc) {}

	bool m_ok;
	std::string m_desc;
};

inline void Panic(const Error &p_err) {
	Panic(p_err.Desc());
}

template<typename T>
struct _ErrorOrInternal {
	const std::string &Desc() const {
		return std::get<std::string>(m_internal);
	}

	T &Value() {
		return std::get<T>(m_internal);
	}

protected:
	_ErrorOrInternal() {}
	_ErrorOrInternal(const std::string &p_str): m_internal(p_str) {}

	_ErrorOrInternal(const T &p_val): m_internal(p_val) {}
	_ErrorOrInternal(T      &&p_val): m_internal(std::move(p_val)) {}

	std::variant<std::string, T> m_internal;
};

template<>
struct _ErrorOrInternal<std::string> {
	const std::string &Desc() const {
		return m_internal;
	}

	std::string &Value() {
		return m_internal;
	}

protected:
	_ErrorOrInternal() {}
	_ErrorOrInternal(const std::string &p_str): m_internal(p_str) {}

	std::string m_internal;
};

template<typename T>
struct ErrorOr : public _ErrorOrInternal<T> {
	static inline ErrorOr Fine(const T &p_val) {
		return ErrorOr(p_val);
	}

	static inline ErrorOr Fine(T &&p_val) {
		return ErrorOr(std::move(p_val));
	}

	template<typename... Args>
	static inline ErrorOr Make(Args... p_args) {
		std::stringstream ss;

		((ss << p_args), ...);

		ErrorOr ret;
		ret.m_internal = ss.str();

		return ret;
	}

	bool Ok() const {
		return m_ok;
	}

private:
	ErrorOr(): m_ok(false) {}

	ErrorOr(const T &p_val):  _ErrorOrInternal<T>(p_val),            m_ok(true) {}
	ErrorOr(T       &&p_val): _ErrorOrInternal<T>(std::move(p_val)), m_ok(true) {}

	bool m_ok;
};

template<typename T>
inline void Panic(const ErrorOr<T> &p_err) {
	Panic(p_err.Desc());
}

template<typename T>
struct Maybe {
	static inline Maybe None() {
		return Maybe();
	}

	Maybe(): none(true) {}
	Maybe(const T &p_value): none(false), unwrap(p_value) {}
	Maybe(T &&p_value):      none(false), unwrap(p_value) {}

	bool none;
	T    unwrap;

	bool operator ==(const Maybe &p_maybe) {
		return none or p_maybe.none? none == p_maybe.none : unwrap == p_maybe.unwrap;
	}

	bool operator !=(const Maybe &p_maybe) {
		return !(*this == p_maybe);
	}

	bool operator ==(const T &p_val) {
		if (none)
			return false;

		return p_val == unwrap;
	}

	bool operator !=(const T &p_val) {
		return !(*this == p_val);
	}

	template<typename... Args>
	T &UnwrapOrPanic(Args... p_args) {
		if (none)
			Panic(p_args...);

		return unwrap;
	}

	template<typename... Args>
	const T &UnwrapOrPanic(Args... p_args) const {
		if (none)
			Panic(p_args...);
		else
			return unwrap;
	}
};

Maybe<std::string> ReadFile(const std::string &p_path);
Error WriteFile(const std::string &p_path, const std::string &p_content);

std::string Trim(const std::string &p_str);

}

#endif
