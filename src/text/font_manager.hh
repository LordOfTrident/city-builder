#ifndef FONT_MANAGER_HH__HEADER_GUARD__
#define FONT_MANAGER_HH__HEADER_GUARD__

#include <string> // std::string

#include <SDL2/SDL.h>

#include "font.hh"

#include "../utils.hh"
#include "../manager.hh"

namespace CityBuilder {

namespace Text {
	class FontManager : public Manager<Font, std::string> {
	public:
		Error FromFile(const std::string &p_key,
		               const std::string &p_sheetPath, const std::string &p_infoPath);

		void Clear();
	};
}

}

#endif
