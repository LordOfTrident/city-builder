#include "utils.hh"

namespace CityBuilder {

std::ostream *g_logStream = &std::cerr;

Maybe<std::string> ReadFile(const std::string &p_path) {
	std::ifstream file(p_path);

	if (not file.is_open())
		return Maybe<std::string>::None();

	std::string content, line;
	while (std::getline(file, line))
		content += line + '\n';

	return content;
}

Error WriteFile(const std::string &p_path, const std::string &p_content) {
	std::ofstream file(p_path);

	if (not file.is_open())
		return Error::Make("Failed to open file '", p_path, "'");

	file << p_content;

	return Error::Fine();
}

std::string Trim(const std::string &p_str) {
	size_t start = p_str.find_first_not_of(" \t\v");

	if (start == std::string::npos)
		return "";

	size_t end = p_str.find_last_not_of(" \t\v");

	return p_str.substr(start, end + 1 - start);
}

}
