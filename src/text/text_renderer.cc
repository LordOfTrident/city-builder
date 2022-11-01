#include "text_renderer.hh"

#include "../main/game.hh"

namespace CityBuilder {

namespace Text {
	Renderer::Renderer() {}

	void Renderer::ClearCache() {
		m_cache.clear();
	}

	ErrorOr<Texture*> Renderer::Render(const std::string &p_text, Font &p_font,
	                                   const Color4i &p_color, size_t p_lineChLimit) {
		if (m_cache.count(p_text) != 0) {
			m_cache.at(p_text).SetColor(p_color);

			return ErrorOr<Texture*>::Fine(&m_cache.at(p_text));
		}

		size_t lines   = 1;
		size_t lineLen = p_text.length();
		if (p_lineChLimit != TEXT_LINE_CH_LIMIT_NONE) {
			lines   = std::ceil(static_cast<double>(p_text.length() / p_lineChLimit));
			lineLen = p_lineChLimit;
		}

		SDL_Surface *surface = SDL_CreateRGBSurface(0, lineLen * p_font.CharW(),
		                                            lines * (p_font.CharH() + TEXT_LINES_PADDING),
		                                            32, 0, 0, 0, 0);
		if (surface == nullptr)
			return ErrorOr<Texture*>::Make("Failed to create text surface: ", SDL_GetError());

		uint32_t colorKey = SDL_MapRGB(surface->format, 0, 0, 0);
		SDL_SetColorKey(surface, true, colorKey);

		SDL_Rect src;
		src.w = p_font.CharW();
		src.h = p_font.CharH();

		SDL_Rect dest = src;
		dest.x = 0;
		dest.y = 0;

		size_t x = 0, y = 0;
		for (size_t i = 0; i < p_text.length(); ++ i) {
			if (x >= lineLen) {
				++ y;
				x = 0;
			}

			dest.x = x * dest.w;
			dest.y = y * (dest.h + TEXT_LINES_PADDING);

			Vec2i pos = p_font.GetCharSheetPos(p_text[i]);
			src.x = pos.x;
			src.y = pos.y;

			SDL_BlitSurface(p_font.sheet, &src, surface, &dest);

			++ x;
		}

		SDL_Texture *texture = SDL_CreateTextureFromSurface(Game::Get().renderer, surface);
		if (texture == nullptr)
			return ErrorOr<Texture*>::Make("Failed to create text texture: ", SDL_GetError());

		if (m_cache.size() + 1 >= MAX_TEXT_CACHE_SIZE)
			ClearCache();

		m_cache.emplace(p_text, std::move(Texture(texture)));
		m_cache.at(p_text).SetColor(p_color);

		return ErrorOr<Texture*>::Fine(&m_cache.at(p_text));
	}
}

}
