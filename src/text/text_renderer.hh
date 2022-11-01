#ifndef TEXT_RENDERER_HH__HEADER_GUARD__
#define TEXT_RENDERER_HH__HEADER_GUARD__

#include <unordered_map> // std::unordered_map
#include <string>        // std::string
#include <cmath>         // std::ceil

#include <SDL2/SDL.h>

#include "font.hh"

#include "../utils.hh"
#include "../units.hh"
#include "../texture.hh"

#define MAX_TEXT_CACHE_SIZE 256

#define TEXT_LINE_CH_LIMIT_NONE 0
#define TEXT_LINES_PADDING      1

namespace CityBuilder {

namespace Text {
	class Renderer {
	public:
		Renderer();

		Renderer(const Renderer &p_copy) = delete;
		Renderer(Renderer &&p_move)      = delete;

		void ClearCache();
		ErrorOr<Texture*> Render(const std::string &p_text, Font &p_font,
	                             const Color4i &p_color = Color4i(255),
	                             size_t p_lineChLimit = TEXT_LINE_CH_LIMIT_NONE);

	private:
		std::unordered_map<std::string, Texture> m_cache;
	};
}

}

#endif
