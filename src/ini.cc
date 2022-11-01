#include "ini.hh"

namespace CityBuilder {

Error INI::ParseFile(const std::string &p_path) {
	std::ifstream file(p_path);
	if (not file.is_open())
		return Error::Make("Failed to open file '" + p_path + "'");

	std::string line, section;
	for (size_t lineNum = 1; std::getline(file, line); ++ lineNum) {
		if (line.empty())
			continue;

		switch (line.at(0)) {
		case ';': case '#': continue;

		case '[':
			{
				size_t end = line.find_first_of(']');

				if (end == std::string::npos)
					return Error::Make("Missing ']' at line ", lineNum);

				section = line.substr(1, end - 1);
			}

			break;

		default:
			{
				size_t assignIdx = 0;
				while (true) {
					assignIdx = line.find_first_of('=', assignIdx + 1);

					if (assignIdx == std::string::npos)
						return Error::Make("Missing '=' at line ", lineNum);

					if (assignIdx > 0) {
						if (line.at(assignIdx - 1) != '\\')
							break;
					}
				}

				auto ret = Unescape(Trim(line.substr(0, assignIdx)));
				if (not ret.Ok())
					return Error::Make(ret.Desc(), ", at line ", lineNum);

				std::string key = ret.Value();
				if (key.empty())
					return Error::Make("Empty key at line ", lineNum);

				ret = Unescape(Trim(line.substr(assignIdx + 1)));
				if (not ret.Ok())
					return Error::Make(ret.Desc(), ", at line ", lineNum);

				std::string value = ret.Value();

				m_sections[section][key] = value;
			}
		}
	}

	return Error::Fine();
}

bool INI::Has(const std::string &p_key, const std::string &p_section) {
	if (m_sections.count(p_section) == 0)
		return false;
	else
		return m_sections.at(p_section).count(p_key);
}

bool INI::Exists(const std::string &p_section) {
	return m_sections.count(p_section);
}

Maybe<bool> INI::Bool(const std::string &p_key, const std::string &p_section) {
	try {
		const std::string &val = m_sections.at(p_section).at(p_key);

		if (val == "true" or val == "1")
			return true;
		else if (val == "false" or val == "0")
			return false;
		else
			return Maybe<bool>::None();
	} catch (...) {
		Panic("INI: Attempt to index non-existant key '", p_section, "::", p_key, "'");
	}

	SILENCE_RETURN_WARNING();
}

Maybe<int64_t> INI::Int64(const std::string &p_key, const std::string &p_section) {
	try {
		return static_cast<int64_t>(std::stoll(m_sections.at(p_section).at(p_key)));
	} catch (...) {
		return Maybe<int64_t>::None();
	}
}

Maybe<size_t> INI::Size(const std::string &p_key, const std::string &p_section) {
	try {
		return static_cast<size_t>(std::stoull(m_sections.at(p_section).at(p_key)));
	} catch (...) {
		return Maybe<size_t>::None();
	}
}

Maybe<float> INI::Float(const std::string &p_key, const std::string &p_section) {
	try {
		return std::stof(m_sections.at(p_section).at(p_key));
	} catch (...) {
		return Maybe<float>::None();
	}
}

INI::Section &INI::operator [](const std::string &p_section) {
	return m_sections[p_section];
}

INI::Section &INI::Get(const std::string &p_section) {
	try {
		return m_sections.at(p_section);
	} catch (...) {
		Panic("INI: Attempt to index non-existant section '", p_section, "'");
	}

	SILENCE_RETURN_WARNING();
}

const INI::Section& INI::Get(const std::string &p_section) const {
	try {
		return m_sections.at(p_section);
	} catch (...) {
		Panic("INI: Attempt to index non-existant section '", p_section, "'");
	}

	SILENCE_RETURN_WARNING();
}

std::string &INI::At(const std::string &p_key, const std::string &p_section) {
	try {
		return m_sections.at(p_section).at(p_key);
	} catch (...) {
		Panic("INI: Attempt to index non-existant key '", p_section, "::", p_key, "'");
	}

	SILENCE_RETURN_WARNING();
}

const std::string &INI::At(const std::string &p_key, const std::string &p_section) const {
	try {
		return m_sections.at(p_section).at(p_key);
	} catch (...) {
		Panic("INI: Attempt to index non-existant key '", p_section, "::", p_key, "'");
	}

	SILENCE_RETURN_WARNING();
}

std::unordered_map<std::string, INI::Section> &INI::All() {
	return m_sections;
}

const std::unordered_map<std::string, INI::Section> &INI::All() const {
	return m_sections;
}

void INI::Clear() {
	m_sections.clear();
}

ErrorOr<std::string> INI::Unescape(const std::string &p_str) {
	std::string unescaped;

	bool escape = false;
	for (auto it = p_str.begin(); it < p_str.end(); ++ it) {
		switch (*it) {
		case '\\': escape = true; break;

		default:
			if (escape) {
				switch (*it) {
				case 'n':  unescaped += '\n';   break;
				case 'r':  unescaped += '\r';   break;
				case 't':  unescaped += '\t';   break;
				case 'f':  unescaped += '\f';   break;
				case 'b':  unescaped += '\b';   break;
				case 'a':  unescaped += '\a';   break;
				case 'e':  unescaped += '\x1b'; break;
				case '"':  unescaped += '"';    break;
				case '\\': unescaped += '\\';   break;

				default:
					size_t idx = static_cast<size_t>(p_str.begin() - it) + 1;

					return ErrorOr<std::string>::Make("Unknown escape sequence '\\", *it,
					                                  "' at char ", idx);
				}
			} else
				unescaped += *it;
		}
	}

	return ErrorOr<std::string>::Fine(unescaped);
}

}
