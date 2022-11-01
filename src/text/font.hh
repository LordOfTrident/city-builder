#ifndef FONT_HH__HEADER_GUARD__
#define FONT_HH__HEADER_GUARD__

#include <string>  // std::string
#include <utility> // std::pair, std::make_pair

#include <SDL2/SDL.h>

#include "../units.hh"
#include "../utils.hh"
#include "../ini.hh"
#include "../sdl_ext.hh"

namespace CityBuilder {

namespace Text {
	class Font {
	public:
		[[nodiscard]]
		static ErrorOr<Font> FromFile(const std::string &p_sheetPath, const std::string &p_infoPath);

		Font(SDL_Surface *p_sheet, size_t p_chW, size_t p_chH, size_t p_chsInARow);
		Font(Font &&p_font);
		Font(const Font &p_font) = delete;
		Font()                   = delete;

		~Font();

		void Free();

		size_t CharW() const;
		size_t CharH() const;

		Vec2i GetCharSheetPos(char p_ch) const;

		SDL_Surface *sheet;

	private:
		size_t m_chW, m_chH, m_chsInARow;
	};
}

}

#endif
