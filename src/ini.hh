#ifndef INI_HH__HEADER_GUARD__
#define INI_HH__HEADER_GUARD__

#include <string>        // std::string, std::stof, std::stoll
#include <unordered_map> // std::unordered_map
#include <cstdint>       // std::int64_t
#include <fstream>       // std::ifstream, std::getline

#include "utils.hh"

namespace CityBuilder {

// Supported:
// - Both ; and # comments
// - Empty section
// - Any text after ']' in section declaration

// Unsupported:
// - Comments in the same line with an assignment

class INI {
public:
	using Section = std::unordered_map<std::string, std::string>;

	Error       ParseFile(const std::string &p_path);
	std::string Stringify() const; // TODO: Implement

	template <typename... Args>
	bool HasInSection(const std::string &p_section, Args... p_args) {
		size_t count = 0;

		((count += not Has(p_args, p_section)), ...);

		return not count;
	}

	bool Has(const std::string &p_key, const std::string &p_section = "");
	bool Exists(const std::string &p_section);

	Maybe<bool>    Bool(const std::string &p_key,   const std::string &p_section = "");
	Maybe<int64_t> Int64(const std::string & p_key, const std::string &p_section = "");
	Maybe<size_t>  Size(const std::string & p_key,  const std::string &p_section = "");
	Maybe<float>   Float(const std::string & p_key, const std::string &p_section = "");

	Section &operator [](const std::string &p_section);

	Section&       Get(const std::string &p_section);
	const Section& Get(const std::string &p_section) const;

	std::string       &At(const std::string &p_key, const std::string &p_section = "");
	const std::string &At(const std::string &p_key, const std::string &p_section = "") const;

	std::unordered_map<std::string, Section>       &All();
	const std::unordered_map<std::string, Section> &All() const;

	void Clear();

private:
	ErrorOr<std::string> Unescape(const std::string &p_str);

	std::unordered_map<std::string, Section> m_sections;
};

}

#endif
